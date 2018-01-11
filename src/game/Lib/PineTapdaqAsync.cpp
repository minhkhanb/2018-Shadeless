#include "pch.h"
#include "PineTapdaqAsync.h"

extern "C" void OS_AsynTapdaqInterstitialPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynTapdaqInterstitialShow(PineServiceHandle* hsvc);

extern "C" void OS_AsynTapdaqBannerPrepare(PineServiceHandle* svc, bool auto_show, const char* ad_id);
extern "C" void OS_AsynTapdaqBannerShow(PineServiceHandle* hsvc);
extern "C" void OS_AsynTapdaqBannerHide(PineServiceHandle* hsvc);

extern "C" void OS_AsynTapdaqRewardedVideoPrepare(PineServiceHandle* svc);
extern "C" void OS_AsynTapdaqRewardedVideoShow(PineServiceHandle* hsvc);

PineTapdaqAsync::PineTapdaqAsync(){
	_svc_interstitial_prepare.reset();
	_svc_banner_prepare.reset();
	_svc_rewarded_video_prepare.reset();
	_need_prepare_rewarded_video_ads = false;
	_is_need_show_video_after_load = false;
}

void PineTapdaqAsync::setUnitId(const char* ad_unit_id){
	sprintf(_curr_unit_id,"%s",ad_unit_id); 
}

PineTapdaqAsync::enumTapdaqUpdateResult PineTapdaqAsync::update(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::WaitingResponse){
		OS_AsynTapdaqInterstitialPrepare(&_svc_interstitial_prepare, false, _curr_unit_id);
		if(_svc_interstitial_prepare.Status == PineServiceResult::Responsed)
		{			
			if (_svc_interstitial_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare tapdaq ads successful!");
				if (_is_need_show_interstitial_after_load)
				{
					showInterstitial();
				}
			}
			else
			{
				PDEBUG("prepare tapdaq ads fail!");
				if (_is_need_show_interstitial_after_load)
				{
					_is_need_show_interstitial_after_load = false;
					return enumTapdaqUpdateResult::PREPARE_ADS_FAIL;
				}
			}
			//_svc_interstitial_prepare.reset();
		}
	}
	
	if(_svc_interstitial_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynTapdaqInterstitialShow(&_svc_interstitial_show);
		if(_svc_interstitial_show.Status == PineServiceResult::Responsed)
		{	
			prepareInterstitial();	
			if (_svc_interstitial_show.ResponseStatus == 0)
			{
				PDEBUG("Show tapdaq ads successful!");
				_svc_interstitial_show.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show tapdaq ads fail!");
				_svc_interstitial_show.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}

	if (_svc_banner_prepare.Status == PineServiceResult::WaitingResponse) {
		OS_AsynTapdaqBannerPrepare(&_svc_banner_prepare, false, _curr_unit_id);
		if (_svc_banner_prepare.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare Tapdaq banner successful!");
			}
			else
			{
				PDEBUG("prepare Tapdaq banner fail!");
			}
			//_svc_interstitial_prepare.reset();
		}
	}

	if (_svc_banner_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynTapdaqBannerShow(&_svc_banner_show);
		if (_svc_banner_show.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_show.ResponseStatus == 0)
			{
				PDEBUG("Show Tapdaq banner successful!");
				return enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show Tapdaq banner fail!");
				_svc_banner_show.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	if (_svc_banner_hide.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynTapdaqBannerHide(&_svc_banner_hide);
		if (_svc_banner_hide.Status == PineServiceResult::Responsed)
		{
			if (_svc_banner_hide.ResponseStatus == 0)
			{
				PDEBUG("Show Tapdaq banner successful!");
				_svc_banner_show.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show Tapdaq banner fail!");
				_svc_banner_hide.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::WaitingResponse) {
		OS_AsynTapdaqRewardedVideoPrepare(&_svc_rewarded_video_prepare);
		if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed)
		{
			if (_svc_rewarded_video_prepare.ResponseStatus == 0)
			{
				PDEBUG("prepare Tapdaq rewarded video successful!");
				if (_is_need_show_video_after_load)
				{
					showRewardedVideo();
				}
			}
			else
			{
				PDEBUG("prepare Tapdaq rewarded video fail!");
				if (_is_need_show_video_after_load)
				{
					_is_need_show_video_after_load = false;
					return enumTapdaqUpdateResult::PREPARE_VIDEO_FAIL;
				}
				//if (_svc_rewarded_video_prepare.ResponseStatus == 1)
				{
					//_count_frame_for_prepare_rewarded_video_ads = COUNT_FOR_PREPARE_REWARDED_VIDEO_ADS;
					//_need_prepare_rewarded_video_ads = true;
				}
			}
		}
	}
	
	if (_need_prepare_rewarded_video_ads)
	{
		if (_count_frame_for_prepare_rewarded_video_ads == 0)
		{
			prepareRewardedVideo();
		}
		else if (_count_frame_for_prepare_rewarded_video_ads > 0)
		{
			_count_frame_for_prepare_rewarded_video_ads--;
		}
		_need_prepare_rewarded_video_ads = false;
	}

	if (_svc_rewarded_video_show.Status == PineServiceResult::WaitingResponse)
	{
		OS_AsynTapdaqRewardedVideoShow(&_svc_rewarded_video_show);
		if (_svc_rewarded_video_show.Status == PineServiceResult::Responsed)
		{
			if (!_is_need_show_video_after_load)
			{
				prepareRewardedVideo();
			}
			else 
			{
				_is_need_show_video_after_load = false;
			}
			if (_svc_rewarded_video_show.ResponseStatus == 0)
			{
				PDEBUG("Show Tapdaq rewarded video successful!");
				return enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS;
			}
			else
			{
				PDEBUG("Show Tapdaq rewarded video fail!");
				_svc_rewarded_video_show.reset();
				return enumTapdaqUpdateResult::SHOW_VIDEO_FAIL;
			}
		}
	}
	return enumTapdaqUpdateResult::NOTHING;
}
void PineTapdaqAsync::prepareInterstitial(bool isAutoShow){
	_is_need_show_interstitial_after_load = isAutoShow;
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse){
		if (_is_need_show_interstitial_after_load && isInterstitialReady())
		{
			PDEBUG("CPP Tapdaq interstitial ready call show");
			showInterstitial();
			_is_need_show_interstitial_after_load = false;
		}
		else
		{
			PDEBUG("Tapdaq prepare\n");
			_svc_interstitial_prepare.reset();
			OS_AsynTapdaqInterstitialPrepare(&_svc_interstitial_prepare, false, _curr_unit_id);
		}
	}
}

void PineTapdaqAsync::showInterstitial()
{	
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0){
		_svc_interstitial_show.reset();
		OS_AsynTapdaqInterstitialShow(&_svc_interstitial_show);
		PDEBUG("CALL SHOW");
	}else{
		PDEBUG("Some BUG");
	}
}

bool PineTapdaqAsync::isInterstitialReady() {
	PDEBUG("\n _svc_interstitial_prepare.Status:%d -- _svc_interstitial_prepare.ResponseStatus:%d",_svc_interstitial_prepare.Status,_svc_interstitial_prepare.ResponseStatus);
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		if (_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse && !_is_need_show_interstitial_after_load)
		{
			prepareInterstitial();
		}
		return false;
	}
}

void PineTapdaqAsync::prepareBanner() {
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_banner_prepare.Status != PineServiceResult::WaitingResponse) {
		_svc_banner_prepare.reset();
		OS_AsynTapdaqBannerPrepare(&_svc_banner_prepare, false, _curr_unit_id);
		PDEBUG("Tapdaq banner prepare\n");
	}
}

void PineTapdaqAsync::showBanner()
{
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 0) {
		_svc_banner_show.reset();
		OS_AsynTapdaqBannerShow(&_svc_banner_show);
		PDEBUG("CALL SHOW BANNER");
	}
	else {
		PDEBUG("SHOW BANNER ERROR");
	}
}

void PineTapdaqAsync::hideBanner()
{
	if (_svc_banner_show.Status == PineServiceResult::Responsed && _svc_banner_show.ResponseStatus == 0) {
		PDEBUG("HIDE BANNER");
		_svc_banner_hide.reset();
		OS_AsynTapdaqBannerHide(&_svc_banner_hide);
	}
	else {		
		PDEBUG("HIDE BANNER ERROR");
	}
}

bool PineTapdaqAsync::isShowBanner() {
	
	if (_svc_banner_show.Status == PineServiceResult::Responsed && _svc_banner_show.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool PineTapdaqAsync::isBannerReady() {
	PDEBUG("\n _svc_banner_show.Status:%d -- _svc_banner_show.ResponseStatus:%d",_svc_banner_show.Status,_svc_banner_show.ResponseStatus);
	if (_svc_banner_prepare.Status == PineServiceResult::Responsed && _svc_banner_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PineTapdaqAsync::prepareRewardedVideo(bool isAutoshow) {
	_is_need_show_video_after_load = isAutoshow;
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 2) //Turn off in java code
	{
		return;
	}
	if (_svc_rewarded_video_prepare.Status != PineServiceResult::WaitingResponse) {
		if (_is_need_show_video_after_load && isRewardedVideoReady())
		{
			PDEBUG("tapdaq rewarded video ready call show\n");
			showRewardedVideo();
		}
		else
		{
			PDEBUG("tapdaq rewarded video prepare\n");
			_svc_rewarded_video_prepare.reset();
			OS_AsynTapdaqRewardedVideoPrepare(&_svc_rewarded_video_prepare);
		}
	}
}

void PineTapdaqAsync::showRewardedVideo()
{
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 0) {
		PDEBUG("CALL SHOW REWARDED VIDEO");
		_svc_rewarded_video_show.reset();
		OS_AsynTapdaqRewardedVideoShow(&_svc_rewarded_video_show);
	}
	else {
		PDEBUG("SHOW REWARDED VIDEO ERROR");
	}
}

bool PineTapdaqAsync::isRewardedVideoReady() {
	PDEBUG("\n _svc_rewarded_video_prepare.Status:%d -- _svc_rewarded_video_prepare.ResponseStatus:%d",_svc_rewarded_video_prepare.Status,_svc_rewarded_video_prepare.ResponseStatus);
	if (_svc_rewarded_video_prepare.Status == PineServiceResult::Responsed && _svc_rewarded_video_prepare.ResponseStatus == 0)
	{
		return true;
	}
	else
	{
		if (_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse && !_is_need_show_video_after_load)
		{
			prepareRewardedVideo();
		}
		return false;
	}
}



bool PineTapdaqAsync::isTurnoff(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 2)
	{
		return true;
	}
	else
	{
		return false;
	}	
}
