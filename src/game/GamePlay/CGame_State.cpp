#include "pch.h"
#include "../GameCore/CGame.h"
#include "../GameCore/Os_Functions.h"
#include <stdio.h>
extern "C" void ServerADVERTStart();
extern "C" void PurchaseProduct(const char *indentify);
extern "C" bool isChartboostAvailable();
void CGame::OnScreenChange(PineSize size)
{
	///////////////////////////////////////////
	// INIT CONTEXT
	///////////////////////////////////////////

	//define support version
#if defined(_WINDOWS_PHONE8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1280_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 }
	};
	int support_version_num = 2;
#elif defined(_WIN8)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1366_768, Orientation::Landscape_90 }
	};
	int support_version_num = 1;
#elif defined(MAC_OS)
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait },

	};
	int support_version_num = 1;
#elif defined(ANDROID_OS)
	/*ResVersion support_version[] = {
		{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait }
	};*/
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait },

	};
	int support_version_num = 1;
#else //windows
	ResVersion support_version[] = {
		{ SupportVersion::RESVER_1334_750, Orientation::Portrait },

	};
	int support_version_num = 1;
#endif
	//detect device version
	{
		ResVersion device_support_version[] = {
			{ SupportVersion::RESVER_960_640, Orientation::Landscape_90 },
			{ SupportVersion::RESVER_1024_768, Orientation::Landscape_90 },
			{ SupportVersion::RESVER_1136_640, Orientation::Landscape_90 },
			{ SupportVersion::RESVER_800_480, Orientation::Landscape_90 },
		};
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		_device._version = (GetAppropriateVersion(dvsize, device_support_version, 4)).Version;
	}

	if (support_version_num > 0) {
		PineSize dvsize = { _device.ScreenSize.W, _device.ScreenSize.H };
		ResVersion curr_ver = GetAppropriateVersion(dvsize, support_version, support_version_num);
		Alignment game_align = { HorizontalAlign::Center, VerticalAlign::Middle };
		AlignmentContext(game_align, dvsize, curr_ver, true, _HCENTER);

		switch (curr_ver.Version)
		{
		case SupportVersion::RESVER_800_480:
			//_current_version = VERSION_480x800;
			break;

		case SupportVersion::RESVER_960_640:
			//_current_version = VERSION_640x960;
			break;

		case SupportVersion::RESVER_1024_768:
			//_current_version = VERSION_768x1024;
			break;

		case SupportVersion::RESVER_1136_640:
			//_current_version = VERSION_750x1334;
			break;

		case SupportVersion::RESVER_1366_768:
			//_current_version = VERSION_1366x768;
			break;
		}
	}

	//port game
	_pointLeftTop = _game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	_pointRightBottom = _game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	_screenWidth = _pointRightBottom.X - _pointLeftTop.X;
	_screenHeight = _pointRightBottom.Y - _pointLeftTop.Y;
	printf("\n _pointLeftTop.X: %f, _pointLeftTop.Y: %f, _pointRightBottom.X: %f, _pointRightBottom.Y: %f", _pointLeftTop.X, _pointLeftTop.Y, _pointRightBottom.X, _pointRightBottom.Y);
	printf("\n ScaleOutX: %f, ScaleOutY: %f, scaleX: %f, scaleY: %f, ScaleOut: %f, TranslateX: %f, TranslateY: %f", GAME()->_game_context.ScaleOutX, GAME()->_game_context.ScaleOutY, GAME()->_game_context.ScaleX, GAME()->_game_context.ScaleY, GAME()->_game_context.ScaleOut, GAME()->_game_context.TranslateX, GAME()->_game_context.TranslateY);

	//end port
}

void CGame::InitEx()
{
	InitBuff(UNZIP_BUFFER_LENGTH_HIGH);


	_GameState = k_GS_LOGO;
	_GameSubState = k_INIT;

	_user.SetGame();
	_idTouchProcessing = -1;
	_canTouch = true;
	_soundButtonID = 0;
	_gameStateGamePlay._mainMenu._shareScore._stateShare = 0;


	_isAdsProcessing = false;
	_numShowAds = 0;
	_showPineAds = 0;
	_timesShowAdsMod = 3;
	_timesShowAdsVideo = 3;
	_stateShowPineAds = 0;
	_hasAction = 0;
	_numOpenApp = 0;
	_numGameArow = 0;
	_oldScore = -1;

#ifdef MAC_OS
#if ADD_ADVERT
	ServerADVERTStart();
#endif
#endif

#if !defined (MAC_OS)
	_isBackkeyPress = false;
	_waitForProcessBackkeyCount = 0;
#endif

#if ANDROID_OS
	GAME()->_gameBoardScoreItem.AccountType = PineAccountType::GooglePlay;
	sprintf(GAME()->_gameBoardScoreItem.BoardId, "%s", GP_LEADERBOARD_ID);
#endif
	countShowRateUs = TIME_SHOW_RATE_US;

	_count_frame_show_ads = 0;
	_time_delay_show_ads = (TIME_DELAY_SHOW_ADS * 60);
	_show_ads_interrupt = true;
	PDEBUG("\n #################################Game init");

	configDownloaded = false;
	num_play_to_show_topebox_ads = 0;
	closeTopeboxAds = false;
	disableTopeboxAds = false;
	time_get_remote_config = 0;

	_point_o = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_LEFT | ANCHOR_TOP);
	_point_right_bottom = GAME()->_game_context.MeasureAnchor(0, 0, ANCHOR_RIGHT | ANCHOR_BOTTOM);
	_screen_width = _point_right_bottom.X - _point_o.X;
	_screen_height = _point_right_bottom.Y - _point_o.Y;
	_port_scale_x = _screen_width / k_SCREEN_WIDTH;
	_port_scale_y = _screen_height / k_SCREEN_HEIGHT;
}

void CGame::ReleaseEx()
{

}

void CGame::PauseEx()
{
	_timeOut = time(NULL);
	AddNotify();
	_numOpenApp++;
	if (!_hasAction)
	{
		_numGameArow = 0;
	}
}

void CGame::ResumeEx()
{
	INT_TIME timeOut = time(NULL) - _timeOut;
	if (_GameState > k_GS_LOGO)
	{
		if (timeOut >= 4 * 60 * 60)
		{
			SetGameState(k_GS_LOGO, false);
		}
		else
		{
			if (_gameStateGamePlay._mainMenu._shareScore._type == CShareScore::enumTypeRender::RATE_US)
			{
				_isAdsProcessing = false;
			}

			if (_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_MENU)
			{
				if (_advert.IsAvailable() && !_hasAction && !_isAdsProcessing)
				{
					if (_numOpenApp >= 1)
					{
						_stateShowPineAds = 1;
					}
				}
				_hasAction = false;
			}
		}
		PDEBUG("\n Resume => _show_ads_interrupt:%d", _show_ads_interrupt);
		if (_show_ads_interrupt && _GameState != k_GS_LOGO)
		{
			_count_frame_show_ads = 30;
		}
		if (!_show_ads_interrupt)
		{
			//_show_ads_interrupt = true;
		}
	}
}
bool CGame::OnBackKeyPress() {
	//Implement function when 
#if !defined (MAC_OS)
	CGame::_this->_isBackkeyPress = true;
	CGame::_this->_waitForProcessBackkeyCount = 0;
	return true;
#else
	return false;
#endif
}

void CGame::UnloadGameBackground()
{
	//printf("\nUnload Game Back ground");
	if (!_compete_backup_stock)
	{
		_step_force_loading = 0;
		BackupAnimStock();
		for (int i = 0; i < TOTAL_SPRITE_INGAME; i++)
		{
			if (_sprite_loaded[i])
			{
				//printf("\nUnload SpriteID = %d",i);
				UnloadSpriteVer(i);
				_sprite_loaded[i] = 1;
			}
		}
		_compete_backup_stock = true;
		printf("\nUnload background");
	}

}
bool CGame::LoadGameForceground()
{

	if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
	{
		return true;
	}

	_compete_backup_stock = false;
	//printf("\nloading force ground = %d",_step_force_loading);
	//G()->SetColor(0);
	//G()->FillRect(0,0,k_SCREEN_WIDTH,k_SCREEN_HEIGHT);


	if (_step_force_loading >= 10)
	{
		for (int i = 0; i < 100; i++)
		{

			if (_sprite_loaded[_step_force_loading - 10])
			{
				//printf("\n_step_force_loading-10 = %d",(_step_force_loading-10));
				LoadSpriteCurrentVer(_step_force_loading - 10, _sprite_antialias_loaded[_step_force_loading - 10], _sprite_optimized_loaded[_step_force_loading - 10]);
			}
			_step_force_loading++;
			if (_step_force_loading >= TOTAL_SPRITE_INGAME + 10)
			{
				RestoreAnimStock();

				return true;
			}

		}
	}
	else
	{
		_step_force_loading++;
	}
	return false;
}
void CGame::UpdateTouch(int xx, int yy, int touch_state, int touch_id, int tap_count)
{
	/*switch (_GameState)
	{

	}*/
}
bool CGame::CheckTouchProcessing(int i)
{
	if (_idTouchProcessing != -1)
	{
		if (_idTouchProcessing != i)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
#ifdef _WINDOWS
void CGame::ProcessCommandLine(const char* cmd)
{

}
#endif
//port game
void CGame::MeasureButton(Button* button)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();
	float btn_width = button->TZ_GetWidth();
	float btn_height = button->TZ_GetHeight();
	btpos = GAME()->_game_context.MeasureDensity(btpos, btn_width, btn_height);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}
void CGame::MeasureButton(Button* button, int anchor)
{
	PinePoint btpos;
	btpos.X = button->_posX + button->TZ_GetX();
	btpos.Y = button->_posY + button->TZ_GetY();


	if ((anchor & ANCHOR_RIGHT) != 0) {
		btpos.X = btpos.X - k_SCREEN_WIDTH;
	}

	if ((anchor & ANCHOR_BOTTOM) != 0) {
		btpos.Y = btpos.Y - k_SCREEN_HEIGHT;
	}

	btpos = GAME()->_game_context.MeasureAnchor(btpos, anchor);
	button->_posX = btpos.X - button->TZ_GetX();
	button->_posY = btpos.Y - button->TZ_GetY();
}
void CGame::MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posx, 0, anchor_horizonal);
	float button_fixed_x = button->GetFixX();
	button->_posX = btpos.X - button_fixed_x;
}
void CGame::MeasureButtonY(float posy, Button* button, BYTE anchor_vertical) {
	PinePoint btpos;
	btpos = GAME()->_game_context.MeasureAnchor(posy, 0, anchor_vertical);
	float button_fixed_y = button->GetFixY();
	button->_posY = btpos.Y - button_fixed_y;
}
PinePoint CGame::MeasureFrame(CSprite* sprite, UINT32 frameid) {
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	return GAME()->_game_context.MeasureDensity(framex, framey, framew, frameh);
}
void CGame::MeasureAnim(CAnimObject* obj)
{
	obj->CalculateCurrentAFrame();
	int fid = obj->_anim_sprite->GetFrameIDOfAFrame(obj->_current_aframes);
	PineRectangle bound = obj->_anim_sprite->GetFmoduleBound(fid, 0);
	PinePoint anim_pos = obj->_anim_sprite->GetAFramePos(obj->_current_aframes);
	PinePoint point = GAME()->_game_context.MeasureDensity(bound.X + anim_pos.X, bound.Y + anim_pos.Y, bound.W, bound.H);
	obj->_x = obj->_x + point.X - bound.X - anim_pos.X;
	obj->_y = obj->_y + point.Y - bound.Y - anim_pos.Y;

}
PinePoint CGame::MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley)
{
	float framex = sprite->GetFModuleX(frameid, 0);
	float framey = sprite->GetFModuleY(frameid, 0);
	float framew = sprite->GetFModuleWidth(frameid, 0);
	float frameh = sprite->GetFModuleHeight(frameid, 0);
	float scale = scalex > scaley ? scalex : scaley;
	PinePoint point = GAME()->_game_context.MeasureDensity(framex - framex * (scale - 1), framey - framey * (scale - 1), framew * scale, frameh * scale);
	point.X -= framex;
	point.Y -= framey;
	return point;
}
PinePoint CGame::MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor)
{
	PineRectangle bound = sprite->GetFrameBound(frameid);
	PinePoint btpos;
	if (anchor == ANCHOR_TOP)
	{
		btpos = GAME()->_game_context.MeasureAnchor(0, bound.Y, anchor);


	}

	return btpos;
}
//end port

///for game state
void CGame::UpdateAndRender()
{
	G()->ClearClip();
	G()->SetOpacity(100);
	if (!LoadGameForceground())
	{
		return;
	}
	//Utils::Loga("\nUpdate And Render");
	UpdateAds();
	RenderGameState();
	if (_gameStateGamePlay._mainMenu._shareScore.UpdateShare())
	{
		return;
	}
	if (_isAdsProcessing)
	{
		G()->SetColor(0x88000000);
		G()->FillFullScreen(true);
		_animLoading.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_animLoading.DrawAnimObject(G());
		_animLoading.UpdateAnimObject();
		return;
	}
	UpdateGameState();

	//PDEBUG("\n _game->_user._isUnlockColor:%d", _user._isUnlockColor);

#if defined (ANDROID_OS)
	_async_task_manager.Run();
#endif

#if !defined (MAC_OS)
	if (CGame::_this->_isBackkeyPress)
	{
		if (CGame::_this->_waitForProcessBackkeyCount >= 1)
		{
			CGame::_this->_isBackkeyPress = false;
			CGame::_this->_waitForProcessBackkeyCount = 0;
			OS_Exit(false);
		}
		CGame::_this->_waitForProcessBackkeyCount++;
	}
#endif
	
}
void CGame::RenderGameState()
{
	switch (_GameState)
	{
	case k_GS_LOGO:
		_gameStateLogo.Render(0, 0);
		break;

	case k_GS_GAME_PLAY:
		_gameStateGamePlay.Render(0, 0);
		break;
	}

	if (_advert.HasPopup())
	{
		_advert.Render();
		_advert.Update();
	}
}
void CGame::UpdateGameState()
{
	switch (_GameState)
	{
	case k_GS_LOGO:
		_gameStateLogo.Update();
		break;

	case k_GS_GAME_PLAY:
		_gameStateGamePlay.Update();
		break;
	}

	if (_stateShowPineAds == 1)
	{
		_advert.Init();
		_stateShowPineAds = 2;
	}
	else if (_stateShowPineAds == 2)
	{
		if (CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_DONE)
		{
			CGame::_this->_advert.SetAppInstallState(PINEADS_INSTALL_STATE_NONE);
			CGame::_this->_advert.SetDownloadCompete();
			_stateShowPineAds = 0;
		}
		else if (CGame::_this->_advert.GetAppInstallState() == PINEADS_INSTALL_STATE_DOWNLOAD_CANCEL)
		{
			CGame::_this->_advert.SetAppInstallState(PINEADS_INSTALL_STATE_NONE);
			_stateShowPineAds = 0;
			_numOpenApp = 0;
		}
	}
	if (_GameState >= k_GS_LOGO)
	{
		if (time_get_remote_config > 0)
		{
			time_get_remote_config--;
			if (time_get_remote_config <= 0)
			{
				time_get_remote_config = 0;
				_topebox_game.GetTopeboxAdsInfoRemoteConfig();
			}

		}
	}
	if (_GameState > k_GS_LOGO)
	{
		//PDEBUG("\n time_get_remote_config:%d", time_get_remote_config);
		//PDEBUG("\n _game->_topebox_game.topebox_banner_opac:%d -- _game->disableTopeboxAds: %d -- _canTouch:%d", _topebox_game.topebox_banner_opac, disableTopeboxAds, _canTouch);
		
		_achievement.UpdateAchievement();
		//PDEBUG("\n _user._removeAds:%d", _user._removeAds);
		if (!_user._removeAds)
		{
			if (_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_MENU)
			{
				if (_time_delay_show_ads <= 0)
				{
					_time_delay_show_ads = TIME_DELAY_SHOW_ADS * 60;
					GAME()->PrepareAndShowInterstitial();
				}
			}

			if (_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_GAME)
			{
				if (_time_delay_show_ads > 0)
				{
					_time_delay_show_ads--;
				}
			}

			if (_count_frame_show_ads > 0)
			{
				_count_frame_show_ads--;
				if (_count_frame_show_ads <= 0)
				{
					_count_frame_show_ads = 0;
					_time_delay_show_ads = TIME_DELAY_SHOW_ADS * 60;
					PDEBUG("\n _topebox_game.show_banner_topebox_active:%d", _topebox_game.show_banner_topebox_active);
					if (_topebox_game.show_banner_topebox_active)
					{
						//show topebox ads
						if (_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER)
						{
							_topebox_game.count_show_close_banner_topebox = 0;
							_topebox_game._is_close_ads = false;
							_topebox_game.render_banner_topebox = true;
							_canTouch = false;
						}
					}
					else
					{
						GAME()->PrepareAndShowInterstitial();
					}
				}
			}
		}
#pragma region TOPEBOX_PROMO_GAME
		if (_user._game_promo_has_show)
		{

#if defined (ANDROID_OS)
			if (_topebox_game._is_close_ads)
			{
				//_topebox_game.render_banner_topebox = false;
				_topebox_game.render_banner_topebox = false;
				if (_topebox_game.count_skip_banner >= 3)
				{
					_user._game_promo_has_show = false;
					_user._game_promo_delay_show = time(NULL) + TIME_DELAY_SHOW_TOPEBOX_ADS;
					_topebox_game.show_banner_topebox_active = false;
					_topebox_game.count_skip_banner = 3;
					_user.DataSave();
					PDEBUG("\n ###########Skip 3 lan");
				}
			}
#endif
			//PDEBUG("\n _topebox_game.GamePromoHasTouchDown():%d -- _canTouch:%d", _topebox_game.GamePromoHasTouchDown(), _canTouch);
			if (_topebox_game.count_show_close_banner_topebox > 0)
			{
				_topebox_game.count_show_close_banner_topebox--;
				//PDEBUG("\n --------------------->_topebox_game.count_show_close_banner_topebox:%d", _topebox_game.count_show_close_banner_topebox);
				if (_topebox_game.count_show_close_banner_topebox <= 0)
				{
					_topebox_game.count_show_close_banner_topebox = -10;
				}
			}
			if (_topebox_game.GamePromoHasTouchDown())
			{
				PDEBUG("\n _topebox_game.GamePromoHasTouchDown():%d -- _canTouch:%d", _topebox_game.GamePromoHasTouchDown(), _canTouch);
#if !defined (_WINDOWS)
				if (Device::IsNetworkAvailable())
#endif
				{
					_show_ads_interrupt = false;
					_user._game_promo_has_show = false;
					_topebox_game.show_banner_topebox_active = false;
					_topebox_game.render_banner_topebox = false;
					_user._game_promo_order++;
					_user._game_promo_delay_show = time(NULL) + TIME_DELAY_SHOW_TOPEBOX_ADS;
					_canTouch = true;
					//_topebox_game.ResponseRequest();
					PDEBUG("\n ##### Open URL:_link_store = %s \n", _topebox_game._link_store);
#if defined (ANDROID_OS) || defined (MAC_OS)
					PDEBUG("\n ##### Open URL:linkStore = %s \n", _topebox_game._link_store);
					PineSocial::OpenURL(_topebox_game._link_store);


#endif
					/*char title[128];
					char decs[1024];
					sprintf(title, "%s Click", "Topebox-Ads");
					sprintf(decs, "%s Click", "Topebox-Ads");
					char type[128];
					sprintf(type, "%s", "topebox_ads");
#if defined (MAC_OS) || (ANDROID_OS)
					PDEBUG("\n logContent title:%s --- decs:%s --- type:%s", title, decs, type);
					Device::GA_LogContents(title, decs, type);
#endif*/
					_user.DataSave();
			}
		}
	}
		//if (!(_menu_button_active || _menu_button_scrolling))
		{
			if (_user._game_promo_has_show)
			{
				if (_topebox_game.UpdateTouchGamePromo())
				{
					//return true;
				}
			}
		}

#pragma endregion
}

}
void CGame::PurchaseIAP(const char* stringID, enumIAPResponseStatus IsSucess)
{
	if (IsSucess == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS
		|| IsSucess == enumIAPResponseStatus::IAP_RESPONSE_STATUS_SUCCESS_RESTORE)
	{
		PDEBUG("\n PurchaseIAP");
		if (strcmp(stringID, "Shadeless_NoAds") == 0)
		{
			_user._removeAds = true;
			_user._isUnlockColor = true;
			_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_Colorful);
			_gameStateGamePlay._mainMenu._setting._settingItems[CMainMenu::enumMenuItems::ITEM_REMOVEADS]._button.SetState(BUTTON_STATE_LOCK);
			_user.DataSave();
		}
#if ANDROID_OS
		PDEBUG("\n sucessed %d", IsSucess);
		if (strcmp(stringID, "shadeless_noads_1_99") == 0)
		{
			PDEBUG("\n valid noads id %s", stringID);
			_gameStateGamePlay._mainMenu._menuItems[CMainMenu::enumMenuItems::ITEM_REMOVEADS]._button.SetState(BUTTON_STATE_LOCK);
			_user._removeAds = true;
			_user._isUnlockColor = true;
			_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_Colorful);
			_user.DataSave();
		}
#endif
	}
	_isAdsProcessing = false;
	}
bool CGame::CanUseTouch()
{
	for (int i = 0; i < _num_touches; i++)
	{
		if (_touches[i]->_flag)
		{
			if (_touches[i]->_state != TOUCH_UP)
			{
				return false;
			}
		}
	}
	return true;
}

CGameState::CGameState()
{
	_game = NULL;
	_gp = NULL;
}
CGameState::~CGameState()
{

}

void CGameState::InitGameState()
{
	_game = GAME();
	_gp = G();
	_game->_GameSubState = k_LOOP;
}

void CGame::ProcessingTag(int tag, int indexButton)
{
	switch (tag)
	{
	case ALERT_TAG_CONFRIM_SYNC_ICOUND:
		if (indexButton == 1)//touch ok
		{
			_user._isUseIcloud = CUser::enumUseICloud::ICLOUD_USE;
			_user.DataSave();
		}
		else if (indexButton == 0)//cancel
		{
			_user._isUseIcloud = CUser::enumUseICloud::ICLOUD_DONT_USE;
			_user.DataSave();
		}
		break;

	case ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE:
		if (indexButton == 1)//touch ok
		{
			_user._isUseIcloud = CUser::enumUseICloud::ICLOUD_USE;
			_user.LoadFromICloud();
			_user.DataSave();
			if (_GameState > k_GS_LOGO)
			{
				SetGameState(k_GS_LOGO, false);
			}
		}
		break;

	case ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE:
		if (indexButton == 0)//touch ok
		{
			//_user.LoadFromICloud();
			//_user.DataSave();
		}
		break;
	}
	_isAdsProcessing = false;
}

void CGame::PlaySoundButton()
{
	PlaySFX(SFX_BUTTON_1 + _soundButtonID);
	_soundButtonID++;
	if (_soundButtonID >= 3)
	{
		_soundButtonID = 0;
	}
}
void CGame::AddNotify()
{
#ifdef MAC_OS
	if (_notificationEnable)
	{
		Device::_current_num_notification = 0;
		Device::AddNotification("Do you want to train your eyes?", 24 * 60 * 60, false);
		Device::AddNotification("Ding Ding!", 7 * 24 * 60 * 60, false);
	}
#endif
}

void CGame::GGPlayShowLeaderBoard()
{
#if ANDROID_OS
	if (_async_task_manager._is_login_google)
	{
		PDEBUG("\nGAME()->_async_task_manager._on_process_show_leaderBoard: %d", _async_task_manager._on_process_show_leaderBoard);
		if (!_async_task_manager._on_process_show_leaderBoard)
		{

			//GAME()->_gameBoardScoreItem.Score = game->_hight_score;
			//GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
			_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_LEADERBOARD);
			_async_task_manager._on_process_show_leaderBoard = true;
		}
	}
	else
	{
		_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		_async_task_manager._is_force_login_google_play = true;
		_async_task_manager._on_process_show_leaderBoard = true;
	}
#endif
}
void CGame::GGPlaySynDriver()
{

#if ANDROID_OS
	if (!_async_task_manager._on_process_sync_cloud)
	{
		_user.CheckLocalAndCloudSaveFile(false);
	}
#endif
}

void CGame::GGPlayShowAchievement()
{
#if ANDROID_OS
	if (_async_task_manager._is_login_google)
	{
		if (!_async_task_manager._on_process_show_achievement)
		{
			_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_SHOW_ACHIEVEMENTS);
			_async_task_manager._on_process_show_achievement = true;
		}
	}
	else
	{
		_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		_async_task_manager._is_force_login_google_play = true;
		_async_task_manager._on_process_show_achievement = true;
	}
#endif
}
void CGame::GGPlaySubmitAchievement(const char ggPlayID_ACHIEVEMENT[128])
{
#if ANDROID_OS	
	if (_async_task_manager._is_login_google)
	{
		_async_task_manager._need_check_unlock_achievement = true;
		PDEBUG("\n############## ggPlayID_ACHIEVEMENT gggg: %s", ggPlayID_ACHIEVEMENT);
		sprintf(_async_task_manager._achivement_id, "%s", ggPlayID_ACHIEVEMENT);
		_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
	}

#endif
}
void CGame::LoadAchievement() {
	for (int i = 0; i < TOTAL_ACHIEVEMENT; i++)
	{
		//maxAchieve[i] = GDS_Exporter::achievement_value[i];
		percentAchieve[i] = 0;
		isCompleteAchieve[i] = enumAchieveStatus::ACHIEVEMENT_NONE;

	}
}
int CGame::GetPercent(int index)
{
	int res = 0;
	res = currentAchieve[index] / (float)maxAchieve[index] * 100;
	if (res >= 100)
	{
		res = 100;
	}
	return res;
}
bool CGame::CheckAchieveComplete(int index)
{
	if (percentAchieve[index] >= 100)
	{
		return true;
	}
	return false;
}
void CGame::CheckCompleteAchievement() {
	for (int i = 0; i < TOTAL_ACHIEVEMENT; i++)
	{
		if (isCompleteAchieve[i] == enumAchieveStatus::ACHIEVEMENT_NONE)
		{
			int percent;

			switch (i)
			{
			case Achievement_0:
			case Achievement_1:
			case Achievement_2:
			case Achievement_3:
			case Achievement_4:
			case Achievement_5:
			case Achievement_6:
				//currentAchieve[i] = num_play_game;
				break;

			case Achievement_7:
			case Achievement_8:
			case Achievement_9:
			case Achievement_10:
			case Achievement_11:
				//currentAchieve[i] = _user.reachScorePoint;
				break;

			case Achievement_12:
			case Achievement_13:
			case Achievement_14:
			case Achievement_15:
			case Achievement_16:
				//currentAchieve[i] = _user.awesomeTitles;
				break;

			case Achievement_17:
			case Achievement_18:
			case Achievement_19:
			case Achievement_20:
			case Achievement_21:
				//currentAchieve[i] = _user.greatTitles;
				break;

			case Achievement_22:
			case Achievement_23:
			case Achievement_24:
			case Achievement_25:
			case Achievement_26:
				//currentAchieve[i] = _user.niceTitles;
				break;

			default:
				break;
			}

			percent = GetPercent(i);

			if (percent > percentAchieve[i])
			{
				percentAchieve[i] = percent;
				if (CheckAchieveComplete(i))
				{
					isCompleteAchieve[i] = enumAchieveStatus::ACHIEVEMENT_COMPLETED;
					//GGPlaySubmitAchievement(GDS_Exporter::achievement_id[i]);
					SubmitAchievement(i);
				}
			}
		}
		//PDEBUG("\n achievement %d: %s  --- %d", i, GDS_Exporter::achievement_id[i], percentAchieve[i]);
	}
}

void CGame::SubmitAchievement(int index)
{

	if (index < 0)
	{
		return;
	}
	_user._achievement_status[index] = CUser::ACHIEVEMENT_STATE_COMPLETED;
	GGPlaySubmitAchievement(GDS_Exporter::achievement_gg_id[index]);
}

void CGame::UpdateStatusAchievement()
{
	bool issubmit = false;
	for (int i = 0; i < TOTAL_ACHIEVEMENT; i++)
	{
		if (_user._achievement_status[i] == CUser::ACHIEVEMENT_STATE_COMPLETED)
		{
			issubmit = true;
			_user._achievement_status[i] = CUser::ACHIEVEMENT_STATE_SUBMITTING;
#if defined(ANDROID_OS)
			_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
#endif
		}
		}
	if (issubmit)
	{
#if defined(ANDROID_OS)
		PDEBUG("\n---------------------------------------------------------UpdateStatusAchievement");
		_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
#endif
	}
	}

void CGame::SetAchievementSubmitCompleted(int index)
{
	if (index < 0)
	{
		return;
	}
	_user._achievement_status[index] = CUser::ACHIEVEMENT_STATE_SUBMITED_SUCCESS;
	_user.DataSave();
}

void CGame::SubmitHighScore() {

#if defined(ANDROID_OS)
	if (_async_task_manager._is_login_google)
	{
		_gameBoardScoreItem.Score = _user._highScore;
		_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_POST_SCORE);
	}
#endif

}

void CGame::LogsPlaySession(int count) {

	char title[128];
	char decs[1024];
	char type[128];

	const char*  name;
	if (count <= 0)
	{
		return;
	}
	if (count <= 5)
	{
		name = "play_lower_5_games";
	}
	else if (count <= 10)
	{
		name = "play_lower_10_games";
	}
	else {
		name = "play_more_10_games";
	}

	sprintf(title, "%s", name);
	sprintf(decs, "%s", name);
	sprintf(type, "%s", "session_logs");
#if defined (ANDROID_OS)	
	PDEBUG("\n session_logs title:%s --- decs:%s --- type:%s", title, decs, type);
	Device::GA_LogContents(title, decs, type);
#elif _WINDOWS
	PDEBUG("\n session_logs title:%s --- decs:%s --- type:%s", title, decs, type);
#endif
}
void CGame::LogsScoreSession(int count) {

	char title[128];
	char decs[1024];
	char type[128];

	const char*  name;
	if (count <= 0)
	{
		return;
	}
	if (count <= 8)
	{
		name = "score_lower_8";
	}
	else if (count <= 20)
	{
		name = "score_lower_20";
	}
	else {
		name = "score_higher_20";
	}

	sprintf(title, "%s", name);
	sprintf(decs, "%s", name);
	sprintf(type, "%s", "score_logs");
#if defined (ANDROID_OS)	
	PDEBUG("\n score_logs title:%s --- decs:%s --- type:%s", title, decs, type);
	Device::GA_LogContents(title, decs, type);
#elif _WINDOWS
	PDEBUG("\n score_logs title:%s --- decs:%s --- type:%s", title, decs, type);
#endif
}

bool CGame::InitAds()
{
#if defined (ANDROID_OS)
	PDEBUG("INIT ADS");
	_tapdaq_manager.setUnitId("DONT_NEED");
	_tapdaq_manager.prepareInterstitial();
	//_tapdaq_manager.prepareBanner();
	//_tapdaq_manager.prepareRewardedVideo();
	//_unityads_manager.prepareAds();
	//_unityads_manager.prepareVideo();
	//_adsVungle_manager.prepare();
	//_unityads_manager.prepareSkippableVideo();
	_on_show_video_ads = false;
	_admob_play_count = 0;
	_time_count_for_show_ads_banner = 0;
	return true;
#endif
	return false;
}
void CGame::ShowAds()
{
#if defined (ANDROID_OS)
	PDEBUG("\n call ShowAds");
	//_on_show_video_ads = true;
	//_is_waiting_for_reward = true;
	//_on_video_ads_reward = false;
	/*if (_unityads_manager.isVideoReady())
	{
	PDEBUG("SHOW UNITY VIDEO");
	_unityads_manager.showVideo();
	_admob_play_count = 0;
	}
	else if (_adsVungle_manager.isReady())
	{
	PDEBUG("SHOW VUNGLE ADS");
	_admob_play_count = 0;
	_adsVungle_manager.show();
	}
	else */if (_tapdaq_manager.isInterstitialReady())
	{
		PDEBUG("SHOW _admob showInterstitial");
		_tapdaq_manager.showInterstitial();
	}
#endif

}

void CGame::ShowBannerAds()
{
#if defined (ANDROID_OS)
	PDEBUG(" call _admob banner: %d %d", _tapdaq_manager.isBannerReady(), _tapdaq_manager.isShowBanner());
	if (_tapdaq_manager.isBannerReady() && !_tapdaq_manager.isShowBanner())
	{
		PDEBUG("SHOW _admob banner");
		_tapdaq_manager.showBanner();
		_has_show_ads_banner = true;
	}
	else {
		_time_count_for_show_ads_banner = 2 * FPS_MAX;
	}
#endif
}
void CGame::HideBannerAds()
{
#if defined (ANDROID_OS)
	_has_show_ads_banner = false;
	_tapdaq_manager.hideBanner();
#endif
}
void CGame::UpdateAds()
{
#if defined (ANDROID_OS)
	if (OS_IsNetworkAvailable())
	{
		//Admob
		PineTapdaqAsync::enumTapdaqUpdateResult result = _tapdaq_manager.update();
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		//PDEBUG("\n enumAdmobUpdateResult: %d", result);
		switch (result)
		{
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_VIDEO_SUCCESS:
			if (_is_waiting_for_reward)
			{
				PDEBUG("\n Show Tapdaq success");
				_on_video_ads_reward = true;
				_is_waiting_for_reward = false;
				_isAdsProcessing = false;
			}
			GAME()->_show_ads_interrupt = false;
			break;
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_ADS_SUCCESS:
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_ADS_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::PREPARE_ADS_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::SHOW_VIDEO_FAIL:
		case PineTapdaqAsync::enumTapdaqUpdateResult::PREPARE_VIDEO_FAIL:
			PDEBUG("\n Show Tapdaq false");
			_is_waiting_for_reward = false;
			_isAdsProcessing = false;
			break;
		default:
			break;
}
	}
	else {
		if (_is_waiting_for_reward)
		{
			_on_video_ads_reward = true;
			_is_waiting_for_reward = false;
			_isAdsProcessing = false;
		}
	}
#endif
}

void CGame::PrepareAndShowInterstitial()
{
#if defined (ANDROID_OS)
	_tapdaq_manager.prepareInterstitial(true);
#endif
}

void CGame::PrepareAndShowVideoAds()
{
#if defined (ANDROID_OS)
	_is_waiting_for_reward = true;
	_on_video_ads_reward = false;
	_isAdsProcessing = true;
	_tapdaq_manager.prepareRewardedVideo(true);
#endif
}