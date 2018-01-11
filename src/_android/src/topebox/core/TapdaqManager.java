package topebox.core;
import java.util.*;

import android.os.Bundle;
import android.os.Handler;
import 	android.util.Log;

import topebox.core.Actions.ActionArg;
import topebox.core.Actions.ActionTapdaqRewardedVideoAdsShow;
import topebox.core.Actions.ActionTapdaqRewardedVideoAdsShowArg;
import topebox.core.Tapdaq.InterstitialListener;
import topebox.core.Tapdaq.RewardedVideoListener;
import topebox.core.Tapdaq.TapdaqInitListener;

import android.app.Activity;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.reward.RewardedVideoAd;
import com.tapdaq.sdk.CreativeType;
import com.tapdaq.sdk.Tapdaq;
import com.tapdaq.sdk.TapdaqConfig;
import com.tapdaq.sdk.adnetworks.TMMediationNetworks;
import com.tapdaq.sdk.ads.TapdaqPlacement;
import com.tapdaq.sdk.helpers.TLog;
import com.tapdaq.sdk.helpers.TLogLevel;

public class TapdaqManager{
	static private ArrayList<ActionArg> _lst_waiting_args = new ArrayList<ActionArg>();
	static private ArrayList<ActionArg> _lst_waiting_video_args = new ArrayList<ActionArg>();
	static private InterstitialAd _interstitialAd;
	static private AdView _adView;
	static private ActionArg _waiting_banner_arg;
	static private boolean _adBannerLoaded = false;
	static private RewardedVideoAd _videoAds;
	static private AdRequest _rewardedVideoRequest;
	static private Activity CurrentActivity;
	static private boolean _is_onWaitForVideoRewardAdsShow;
	static public long _time_wait_for_show_ads = 10 * 1000;
	static private long _time_start_waiting_for_show_ads = 0;
	static private boolean _is_onWaitForVideoRewardReinitAds;
	static public long _time_wait_for_reinit_ads = 60 * 1000;
	static private long _time_start_waiting_for_reinit_ads;

	static public boolean _is_init_successful;
	static public RewardedVideoListener _rewarded_video_listener;
	static public InterstitialListener _interstitial_listener;
	
	static public void init(Activity activity){
		CurrentActivity = activity;
		TapdaqConfig config = new TapdaqConfig(CurrentActivity);

		//config.registerTestDevices(TMMediationNetworks.AD_MOB, Arrays.asList("E902412DD2B12241F8FD98D43F7F05A3"));

		List<TapdaqPlacement> enabledPlacements = new ArrayList<TapdaqPlacement>();
		enabledPlacements.add(TapdaqPlacement.createPlacement(Arrays.asList(CreativeType.INTERSTITIAL_PORTRAIT), "mainMenu"));
		enabledPlacements.add(TapdaqPlacement.createPlacement(Arrays.asList(CreativeType.REWARDED_VIDEO_INTERSTITIAL), "rewardVideo"));

		config.withPlacementTagSupport(enabledPlacements.toArray(new TapdaqPlacement[enabledPlacements.size()]));
		Tapdaq.getInstance().initialize(CurrentActivity, AppConfig.StringTapdaqAppId, AppConfig.StringTapdaqClientKey, config, new TapdaqInitListener());
		_rewarded_video_listener = new RewardedVideoListener();
		_interstitial_listener = new InterstitialListener();
	}

	static public void onInitSuccess()
	{
		_is_init_successful = true;
		//TLog.setLoggingLevel(TLogLevel.INFO);
		//Tapdaq.getInstance().startTestActivity(CurrentActivity);
	}

	public static void onInterstitialLoaded(boolean success) {
		for (int i = 0; i < _lst_waiting_args.size(); i++) {
			if (success) {
				_lst_waiting_args.get(i).onDone();
			} else {
				_lst_waiting_args.get(i).onCancel();
			}
			_lst_waiting_args.remove(i);
		}
	}

	static public boolean isPrepareInterstitialCompleted(){
		return Tapdaq.getInstance().isInterstitialReady(CurrentActivity, "mainMenu");
	}

	static public void prepareInterstitial(ActionArg arg){
		if (_is_init_successful) {
			if (!isPrepareInterstitialCompleted()) {
				Log.i("DEBUG", "Java tapdaq interstitial prepare");
				_lst_waiting_args.add(arg);
				Tapdaq.getInstance().loadInterstitial(CurrentActivity, "mainMenu", _interstitial_listener);
			}
			else
			{
				Log.i("DEBUG", "Java tapdaq interstitial already loaded!");
				arg.onDone();
			}
		}
		else
		{
			arg.onCancel();
			Log.i("DEBUG", "Tapdaq doesnot init yet!");
		}
	}
	
	static public boolean showInterstitial(ActionArg arg){
		if (isPrepareInterstitialCompleted())
		{
			_lst_waiting_args.add(arg);
			Tapdaq.getInstance().showInterstitial(CurrentActivity, "mainMenu", _interstitial_listener);
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}
	}

	static public void onInterstitialClose(boolean isSuccess)
	{
		for (int i = 0; i < _lst_waiting_args.size(); i++)
		{
			_lst_waiting_args.get(i).onDone();
			_lst_waiting_args.remove(i);
		}
	}


	static public void initTapdaqBanner (AdView ad_view)
	{
		Log.i("DEBUG", "Init Tapdaq banner!");
		/*_adView = ad_view;
		_adView.setAdUnitId(AppConfig.StringAdmobBannerUnitId);
		_adView.setAdSize(AdSize.SMART_BANNER);
		_adView.setVisibility(View.GONE);*/
	}

	static public void prepareBannerAds(ActionArg arg)
	{
		/*if (_adView != null) {
			_waiting_banner_arg = arg;
			AdRequest adRequest = new AdRequest.Builder().build();
			_adView.setAdListener(new AdListener() {
				@Override
				public void onAdClosed() {
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = false;
					Log.i("DEBUG", "admob banner onAdClosed");
					super.onAdClosed();
				}

				@Override
				public void onAdFailedToLoad(int i) {
					_adBannerLoaded = false;
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onCancel();
						_waiting_banner_arg = null;
					}
					Log.i("DEBUG", "admob banner onAdFailedToLoad");
					super.onAdFailedToLoad(i);
				}

				@Override
				public void onAdOpened() {
					Log.i("DEBUG", "admob banner onAdOpened");
					super.onAdOpened();
				}

				@Override
				public void onAdLeftApplication() {
					Log.i("DEBUG", "admob banner onAdLeftApplication");
					super.onAdLeftApplication();
				}

				@Override
				public void onAdLoaded() {
					Log.i("DEBUG", "admob banner onAdLoaded");
					if (_waiting_banner_arg != null)
					{
						_waiting_banner_arg.onDone();
						_waiting_banner_arg = null;
					}
					_adBannerLoaded = true;
					super.onAdLoaded();
				}
			});
			_adView.loadAd(adRequest);
		}
		else
		{
			Log.i("DEBUG", "AdView for admob banner is null");
			arg.onCancel();
		}*/
	}

	static public boolean showBannerAds(ActionArg arg){
		/*if (_adView != null && _adBannerLoaded)
		{
			_adView.setVisibility(View.VISIBLE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}*/
		return false;
	}

	static public boolean hideBannerAds(ActionArg arg){
		/*if (_adView != null)
		{
			_adView.setVisibility(View.GONE);
			arg.onDone();
			return true;
		}
		else {
			arg.onCancel();
			return false;
		}*/
		return false;
	}

	static public void onPause()
	{
		Tapdaq.getInstance().onPause(CurrentActivity);
	}

	static public void onResume()
	{
		Tapdaq.getInstance().onResume(CurrentActivity);
	}

	static public void GameUpdate()
	{
		if (_is_onWaitForVideoRewardAdsShow)
		{
			if (System.currentTimeMillis() - _time_start_waiting_for_show_ads >= _time_wait_for_show_ads)
			{
				onVideoCompleted(false, false);
				_is_onWaitForVideoRewardAdsShow = false;
			}
		}
		if (_is_onWaitForVideoRewardReinitAds)
		{
			if (System.currentTimeMillis() - _time_start_waiting_for_reinit_ads >= _time_wait_for_reinit_ads)
			{
				initTapdaqRewardedVideo();
				_is_onWaitForVideoRewardReinitAds = false;
			}
		}
	}

	//rewarded video
	static public void initTapdaqRewardedVideo()
	{
		LoadRewardedVideo();
	}

	static public void LoadRewardedVideo()
	{
		if (_is_init_successful) {
			boolean isReady = Tapdaq.getInstance().isRewardedVideoReady(CurrentActivity, "rewardVideo");
			if (!isReady) {
				Tapdaq.getInstance().loadRewardedVideo(CurrentActivity,  "rewardVideo", _rewarded_video_listener);
			}
		}
		else
		{
			Log.i("DEBUG", "tapdaq ads not Init");
		}
	}

	static public boolean isPrepareRewardedVideoCompleted(){
		return Tapdaq.getInstance().isVideoReady(CurrentActivity, "rewardVideo");
	}

	static public void prepareRewardedVideo(ActionArg arg){
		if (_is_init_successful) {
			if (!isPrepareRewardedVideoCompleted()) {
				Log.i("DEBUG", "Java tapdaq video prepare");
				_lst_waiting_video_args.add(arg);
				Tapdaq.getInstance().loadVideo(CurrentActivity, "rewardVideo", _rewarded_video_listener);
			}
			else
			{
				Log.i("DEBUG", "Java tapdaq video already loaded!");
				arg.onDone();
			}
		}
		else
		{
			arg.onCancel();
			Log.i("DEBUG", "Tapdaq doesnot init yet!");
		}
	}

	public static void onVideoLoaded(boolean success) {
		for (int i = 0; i < _lst_waiting_video_args.size(); i++) {
			if (success) {
				_lst_waiting_video_args.get(i).onDone();
			} else {
				_lst_waiting_video_args.get(i).onCancel();
			}
			_lst_waiting_video_args.remove(i);
		}
	}

	static public void ShowRewardedVideo(ActionTapdaqRewardedVideoAdsShowArg arg)
	{
		_lst_waiting_video_args.add(arg);
		ActionTapdaqRewardedVideoAdsShow act_tapdaq_show_rewarded_video = new ActionTapdaqRewardedVideoAdsShow(arg, CurrentActivity);
		act_tapdaq_show_rewarded_video.act();
		_is_onWaitForVideoRewardAdsShow = true;
		_time_start_waiting_for_show_ads = System.currentTimeMillis();
	}

	static public void onVideoCompleted(boolean success, boolean needWait)
	{
		if (!needWait) {
			if (!success) {
				LoadRewardedVideo();
			}
		}else
		{
			_is_onWaitForVideoRewardReinitAds = true;
			_time_start_waiting_for_reinit_ads = System.currentTimeMillis();
		}
		ArrayList<ActionArg> tmp = new ArrayList<ActionArg>();

		for(int s = 0; s< _lst_waiting_video_args.size(); s++)
		{
			ActionArg arg = _lst_waiting_video_args.get(s);
			if(arg instanceof ActionTapdaqRewardedVideoAdsShowArg)
			{
				tmp.add(arg);
			}
		}

		if(tmp.size() > 0) Log.i("DEBUG", "Has pending ActionTapdaqRewardedVideoAdsShowArg");

		for(int i = 0; i< tmp.size(); i++){
			ActionArg arg = tmp.get(i);
			_lst_waiting_video_args.remove(arg);
			if (success){
				arg.onDone();
			}
			else {
				arg.onCancel();
			}
		}
	}
}