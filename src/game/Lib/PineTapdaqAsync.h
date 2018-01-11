#if !defined(_H_TAPDAQ_MANAGER)
#define _H_TAPDAQ_MANAGER
#include "DataType/CType.h"

#define COUNT_FOR_PREPARE_REWARDED_VIDEO_ADS 100
class PineTapdaqAsync{
private:
	PineServiceHandle _svc_interstitial_prepare, _svc_interstitial_show,
					_svc_banner_prepare, _svc_banner_show, _svc_banner_hide,
					_svc_rewarded_video_prepare, _svc_rewarded_video_show;
	bool _is_request_prepare;
	bool _is_need_show_video_after_load;
	bool _is_need_show_interstitial_after_load;
	char _curr_unit_id [255];
public :
	enum enumTapdaqUpdateResult {
		NOTHING = 0,
		PREPARE_ADS_SUCCESS,
		PREPARE_ADS_FAIL,
		PREPARE_VIDEO_SUCCESS,
		PREPARE_VIDEO_FAIL,
		SHOW_ADS_SUCCESS,
		SHOW_ADS_FAIL,
		SHOW_VIDEO_SUCCESS,
		SHOW_VIDEO_FAIL,
	};
	bool _need_prepare_rewarded_video_ads;
	int _count_frame_for_prepare_rewarded_video_ads;
	PineTapdaqAsync();
	void setUnitId(const char* ad_unit_id);
	void prepareInterstitial(bool autoShow = false);
	void showInterstitial();
	void prepareBanner();
	void showBanner();
	void hideBanner();
	void prepareRewardedVideo(bool autoShow = false);
	void showRewardedVideo();
	bool isInterstitialReady();
	bool isRewardedVideoReady();
	bool isBannerReady();
	bool isShowBanner();
	bool isTurnoff();
	enumTapdaqUpdateResult update();
};
#endif