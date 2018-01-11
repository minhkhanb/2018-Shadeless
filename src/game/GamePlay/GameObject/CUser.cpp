#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
#include <PineEngine.h>
PE_USING_ENGINE;


extern "C" void ShowAlertMessage_OKCancel(const char* feed, const char* title, const char* tag);
extern "C" void ShowAlertMessage_OK(const char* feed, const char* title, const char* tag);
extern "C" void OS_ShowMessageBox(const char * title, const char * message, int type);
void CUser::SetGame()
{
	_game = GAME();
	_gp = G();
}
void CUser::DataDefault()
{	
	for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
	{
		_buffer[i] = 0;
	}
	_versionFormat = CURRENT_SAVE_VERSION;
	_highScore = 0;
	numPlayGame = 0;
	_numPlayRate = 0;
	_game->SetMusicEnable(true);
	_game->SetSoundEnable(true);
	_game->_notificationEnable  = true;

	_removeAds = false;
	_isUseIcloud = 0;
	_hadRate = false;
	
	_hadLoginFB = false;
	_isUnlockColor = false;
	_curColorUse = 0;
	_stepTutorial = 0;

	_dayPlayGame = 1;
	Pine_Date_Time _time;
	_time.setDateByLocalDate();
	_dayResetGame = _time.getDay();
	_game->_achievement.SetDefaultValue();

	//ver3
	sprintf(_game_promo_icon_id, "%s", "gameID");
	sprintf(_game_promo_link_store, "%s", "linkStore");
	sprintf(_game_promo_deviceID, "%s", "deviceID");
	_game_promo_revision = 0;
	_game_promo_has_show = true;
	_game_promo_delay_show = time(NULL);
	_game_promo_order = 0;
	_has_touch_promo = false;

	SynForNewVersion(_versionFormat);	
}

void CUser::SynForNewVersion(INT_TIME fromVersion)
{
	for (INT_TIME i = fromVersion; i <= CURRENT_SAVE_VERSION; i++)
	{
		NewDataDefault(i);
	}
	_versionFormat = CURRENT_SAVE_VERSION;
}

void CUser::NewDataDefault(INT_TIME version)
{
	switch (version)
	{
	case SAVE_GAME_VERSION_0: 
		break;

	case SAVE_GAME_VERSION_1:
		{
			_dayPlayGame = 1;
			Pine_Date_Time time;
			time.setDateByLocalDate();
			_dayResetGame = time.getDay();
			_game->_achievement.SetDefaultValue();
		}
		break;
	case SAVE_GAME_VERSION_2:
		
		break;

	case SAVE_GAME_VERSION_3:
		_game_promo_revision = 0;
		_game_promo_has_show = true;
		_game_promo_delay_show = time(NULL);
		_game_promo_order = 0;
		_has_touch_promo = false;
		break;
	}

}

void CUser::DataSave()
{
	
	DataToBuffer();
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
	if (_isUseIcloud == CUser::enumUseICloud::ICLOUD_USE)
	{
		SaveToICloud();
	}
}

void CUser::DataLoad()
{
	
	int saved = 0;
	Buffer* buff = OS_LoadAppData(SAVE_FILE_NAME);
	if (buff != NULL)
	{
		saved = buff->Length();
		memcpy(_buffer, buff->Data(), saved);
		delete buff;
	}
    
	if (saved == 0)
	{
        //ResetValueICloud();        
		DataDefault();
        DefaultICloud();
		DataSave();
	}
	else
	{
		DataFromBuffer();
		if (_isUseIcloud == CUser::enumUseICloud::ICLOUD_USE)
		{
			CheckICloud();
		}
	}
}

void CUser::DataToBuffer()
{
	printf("\n ========= BEGIN SAVE version %d ==========", _versionFormat);

	//for version  0
	_game->SetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION, _versionFormat);	
	
	_game->SetInt16At(_buffer, SAVE_FORMAT_NUM_PLAY_RATE, _numPlayRate);
	_game->SetInt64At(_buffer, SAVE_FORMAT_NUM_PLAY, numPlayGame);
	PDEBUG("\n Save numPlayGame:%lld", numPlayGame);
	_game->SetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE, _highScore);
    
	_game->SetByteAt(_buffer, SAVE_FORMAT_MUSIC, _game->IsEnableMusic());
	_game->SetByteAt(_buffer, SAVE_FORMAT_SOUND, _game->IsEnableSound());
	_game->SetByteAt(_buffer, SAVE_FORMAT_NOTIFY, _game->_notificationEnable);
    
	_game->SetByteAt(_buffer, SAVE_FORMAT_REMOVE_ADS, _removeAds);
	_game->SetByteAt(_buffer, SAVE_FORMAT_HAD_LOGIN_FB, _hadLoginFB);
	_game->SetByteAt(_buffer, SAVE_FORMAT_HAD_RATE,_hadRate);

	_game->SetByteAt(_buffer, SAVE_FORMAT_IS_UNLOCK_COLOR, _isUnlockColor);
	_game->SetByteAt(_buffer, SAVE_FORMAT_CUR_COLOR_USE, _curColorUse);
	_game->SetInt16At(_buffer, SAVE_FORMAT_STEP_TUTORIAL, _stepTutorial);

	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{
		_game->SetByteAt(_buffer, SAVE_FORMAT_ACHIEVE_STATE + 1 * i, _game->_achievement._achievement[i]._stateComplete);
		_game->SetInt16At(_buffer, SAVE_FORMAT_ACHIEVE_CURR_VALUE + 2 * i, _game->_achievement._achievement[i]._currentValue);
	}

	for (int i = 0; i < TOTAL_COLOR_DES_ROW; i++)
	{
		_game->SetInt16At(_buffer, SAVE_FORMAT_SCORE_COLOR + 2 * i, _scoreColor[i]);
	}

	_game->SetInt16At(_buffer, SAVE_FORMAT_DAY_PLAY, _dayPlayGame);
	_game->SetInt16At(_buffer, SAVE_FORMAT_DAY_RESET, _dayResetGame);

	//ver3
	for (int i = 0; i<128; i++)
	{
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID + i, _game_promo_deviceID[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID + i, _game_promo_icon_id[i]);
		GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE + i, _game_promo_link_store[i]);
	}
	GAME()->SetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW, _game_promo_delay_show);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW, _game_promo_has_show);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER, _game_promo_order);
	GAME()->SetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION, _game_promo_revision);
	GAME()->SetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO, _has_touch_promo);

	PDEBUG("\n ========= END SAVE ========== \n");	
}

void CUser::DataFromBuffer()
{    
	_versionFormat = _game->GetInt64At(_buffer, SAVE_FORMAT_CURRENT_VERSION);
	printf("\n ========= LOAD SAVE version %d ==========", _versionFormat);

	//for version  0
	_highScore = _game->GetInt32At(_buffer, SAVE_FORMAT_HIGHT_SCORE);
	numPlayGame = _game->GetInt64At(_buffer, SAVE_FORMAT_NUM_PLAY);
	PDEBUG("\n Load numPlayGame:%lld", numPlayGame);
	_numPlayRate = _game->GetInt16At(_buffer, SAVE_FORMAT_NUM_PLAY_RATE);

	_game->SetMusicEnable(_game->GetByteAt(_buffer, SAVE_FORMAT_MUSIC));
	_game->SetSoundEnable(_game->GetByteAt(_buffer, SAVE_FORMAT_SOUND));
	_game->_notificationEnable = _game->GetByteAt(_buffer, SAVE_FORMAT_NOTIFY);	
	
    _removeAds= _game->GetByteAt(_buffer, SAVE_FORMAT_REMOVE_ADS);
	_hadLoginFB = _game->GetByteAt(_buffer, SAVE_FORMAT_HAD_LOGIN_FB);
	_hadRate = _game->GetByteAt(_buffer, SAVE_FORMAT_HAD_RATE);
	_isUseIcloud = _game->GetByteAt(_buffer, SAVE_FORMAT_USE_ICLOUD);

	_isUnlockColor = _game->GetByteAt(_buffer, SAVE_FORMAT_IS_UNLOCK_COLOR);
	_curColorUse = _game->GetByteAt(_buffer, SAVE_FORMAT_CUR_COLOR_USE);
	_stepTutorial = _game->GetInt16At(_buffer, SAVE_FORMAT_STEP_TUTORIAL);

	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{
		_game->_achievement._achievement[i]._stateComplete = _game->GetByteAt(_buffer, SAVE_FORMAT_ACHIEVE_STATE + 1 * i);
		_game->_achievement._achievement[i]._currentValue = _game->GetInt16At(_buffer, SAVE_FORMAT_ACHIEVE_CURR_VALUE + 2 * i);
	}

	for (int i = 0; i < TOTAL_COLOR_DES_ROW; i++)
	{
		_scoreColor[i] = _game->GetInt16At(_buffer, SAVE_FORMAT_SCORE_COLOR + 2 * i);
	}

	_dayPlayGame = _game->GetInt16At(_buffer, SAVE_FORMAT_DAY_PLAY);
	_dayResetGame = _game->GetInt16At(_buffer, SAVE_FORMAT_DAY_RESET);

	//ver3
	for (int i = 0; i<128; i++)
	{
		_game_promo_deviceID[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID);
		_game_promo_icon_id[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ICON_ID);
		_game_promo_link_store[i] = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE);
	}
	_game_promo_delay_show = GAME()->GetInt64At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW);
	_game_promo_has_show = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW);
	_game_promo_order = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_ORDER);
	_game_promo_revision = GAME()->GetInt16At(_buffer, SAVE_FORMAT_USER_GAME_PROMO_REVISION);
	_has_touch_promo = GAME()->GetByteAt(_buffer, SAVE_FORMAT_USER_HAS_TOUCH_PROMO);

	if (_versionFormat < CURRENT_SAVE_VERSION)
	{
		SynForNewVersion(_versionFormat + 1);
		DataSave();
	}
	printf("\n ========= END LOAD ========== \n");	
}


//check syn icloud
int _icloudStatus;
void callback(ECloudKeyValueChangeReason reason, std::vector<std::string> keys)
{
    switch (reason)
    {
        case ECloudKeyValueChangeReason::AccountChange:
            PDEBUG("ICloud Change reason Account Change\n");
            _icloudStatus = 1;
            break;
        case ECloudKeyValueChangeReason::InitialSyncChange:
            PDEBUG("ICloud Change reason InitialSyncChange\n");
            _icloudStatus = 2;
            break;
        case ECloudKeyValueChangeReason::QuotaViolationChange:
            PDEBUG("ICloud Change reason QuotaViolationChange\n");
            _icloudStatus = 3;
            break;
        case ECloudKeyValueChangeReason::ServerChange:
            PDEBUG("ICloud Change reason ServerChange\n");
            _icloudStatus = 4;
            break;
    }
    
    for (int i = 0; i< keys.size(); i++)
    {
        PDEBUG("key change:%s\n", keys[i].c_str());
    }
}
void CUser::ResetValueICloud()
{
#if defined(MAC_OS) || defined(_WINDOWS)
    std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
    if (service == nullptr)
    {
        PDEBUG("icloud fail \n");
    }
    else
    {
        PDEBUG("icloud success\n");
        if (_icloudStatus == 0)
        {
            service->synchronize();
            std::vector<BYTE> data = service->readBytes("data");
            {
                DataDefault();
                DataToBuffer();
                service->write("data", _buffer, SAVE_FORMAT_END);
                PDEBUG("\n\nicloud write data success\n\n");
            }
        }
    }
#endif
}
bool CUser::SaveToICloud()
{
    bool ret = false;
#if defined(MAC_OS)|| defined(_WINDOWS)
    std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
    if (service == nullptr)
    {
        PDEBUG("icloud fail \n");      
    }
    else
    {        
        if (_icloudStatus == 0)
        {
            service->synchronize();
            int server_state = CheckCanPushDataSave();
            if (server_state == ICLOUD_STATE_READY_SYNS || server_state == ICLOUD_STATE_OLD_VERSION ||
                server_state == ICLOUD_STATE_NODATA)
            {
                DataToBuffer();
                service->write("data", _buffer, SAVE_FORMAT_END);
                PDEBUG("\n\nicloud Save data success\n\n");
                ret = true;
            }
        }
    }
#endif
    return ret;
}
int CUser::CheckCanPushDataSave()
{
#if defined(MAC_OS)|| defined(_WINDOWS)
    int state = -1;
    if (!Device::IsNetworkAvailable())
    {
        state = CUser::enumStateiCloud::ICLOUD_STATE_DISCONECT;
    }
    else
    {
        std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
        if (service == nullptr)
        {
            PDEBUG("icloud fail \n");
        }
        else
        {
            
            if (_icloudStatus == 0)
            {
                service->synchronize();
                std::vector<BYTE> data = service->readBytes("data");
                if (data.size() > 0)
                {
					char buffer[MAX_SAVE_BUFFER_CACHE];
					sprintf(buffer, "");
					for (int i = 0; i < SAVE_FORMAT_END; i++)
					{
						buffer[i] = data[i];
					}
					int highScore = GAME()->GetInt32At(buffer, SAVE_FORMAT_HIGHT_SCORE);
					INT_TIME gameVersion = GAME()->GetInt64At(buffer, SAVE_FORMAT_CURRENT_VERSION);
					INT_TIME numPlay = GAME()->GetInt64At(buffer, SAVE_FORMAT_NUM_PLAY);

					printf("\n gameVersion = %lld, highScore = %lld", gameVersion, highScore);
					if (gameVersion < _versionFormat)
					{
						state = CUser::enumStateiCloud::ICLOUD_STATE_OLD_VERSION;
					}
					else
					{
						if (highScore < _highScore)
						{
							state = CUser::enumStateiCloud::ICLOUD_STATE_OLD_VERSION;
						}
						else if (highScore == _highScore)
						{
							if (numPlay < numPlayGame)
							{
								state = CUser::enumStateiCloud::ICLOUD_STATE_OLD_VERSION;
							}
							else if (numPlay == numPlayGame)
							{
								state = CUser::enumStateiCloud::ICLOUD_STATE_READY_SYNS;
							}
							else if (numPlay > numPlayGame)
							{
								state = CUser::enumStateiCloud::ICLOUD_STATE_NEW_VERSION;
							}
						}
						else if (highScore > _highScore)
						{
							state = CUser::enumStateiCloud::ICLOUD_STATE_NEW_VERSION;
						}
					}
                }
                else
                {
                    state = CUser::enumStateiCloud::ICLOUD_STATE_NODATA;
                }
            }
        }
    }
    return state;
#else
    return -1;
#endif
}
void CUser::LoadFromICloud()
{
#if defined(MAC_OS)|| defined(_WINDOWS)
    std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
    if (service == nullptr)
    {
        PDEBUG("icloud fail \n");
    }
    else
    {
        if (_icloudStatus == 0)
        {
            service->synchronize();
            std::vector<BYTE> data = service->readBytes("data");
            if (data.size() > 0)
            {
                PDEBUG("\n\n icloud load data success\n\n");
                for (int i = 0; i < SAVE_FORMAT_END; i++)
                {
                    _buffer[i] = data[i];
                }
                DataFromBuffer();
            }
        }
    }
#endif
}
void CUser::DefaultICloud()
{
#if defined(MAC_OS) || defined(_WINDOWS)
    std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
    if (service == nullptr)
    {
        PDEBUG("icloud fail \n");
    }
    else
	{        
        if (_icloudStatus == 0)
        {
            service->synchronize();
            std::vector<BYTE> data = service->readBytes("data");
            _game->_isAdsProcessing=true;
            if (data.size() > 0)
            {
				char tag[10];
				sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE);
				ShowAlertMessage_OKCancel(GDS_Exporter::string_des[1 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[1 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
                
            }
            else
            {
				char tag[10];
				sprintf(tag,"%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
				ShowAlertMessage_OKCancel(GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
            }
        }
    }
#endif
}
void CUser::CheckICloud()
{
#if defined(MAC_OS) || defined(_WINDOWS)
    std::shared_ptr<CCloudKeyValueService> service = CCloud::regKeyValueService(ECloudProvider::AppleICloud, &callback);
    
    if (service == nullptr)
    {
        PDEBUG("icloud fail \n");
    }
    else
    {
        if (_icloudStatus == 0)
        {
            service->synchronize();
            int server_state = CheckCanPushDataSave();
            PDEBUG("icloud success server_state = %d\n",server_state);
			if (server_state == CUser::enumStateiCloud::ICLOUD_STATE_NODATA)
			{
				char tag[10];
				sprintf(tag, "%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
				ShowAlertMessage_OKCancel(GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
			}
			else if (server_state == CUser::enumStateiCloud::ICLOUD_STATE_NEW_VERSION)
            {				
               if(_game->_GameState >k_GS_LOGO && _game->_gameStateGamePlay._mainMenu._isRenderSetting)
                {
					char tag[10];
					sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE);
					ShowAlertMessage_OKCancel(GDS_Exporter::string_des[1 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[1 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
                }
                else
                {
                    LoadFromICloud();
					DataSave();
                }
            }
            else
            {
                if (_isUseIcloud == CUser::enumUseICloud::ICLOUD_USE)
                {
                    if(SaveToICloud())
                    {
                        if(_game->_GameState >k_GS_LOGO && _game->_gameStateGamePlay._mainMenu._isRenderSetting)
                        {
                            char tag[10];
                            sprintf(tag, "%d", ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE);
                            ShowAlertMessage_OK(GDS_Exporter::string_des[2 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[2 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
                        }
                    }
                }
               else
               {
                   char tag[10];
                   sprintf(tag, "%d", ALERT_TAG_CONFRIM_SYNC_ICOUND);
				   ShowAlertMessage_OKCancel(GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[0 * TOTAL_STRING_DES + STRING_DES_TEXT_TITLE], tag);
               }
            }
        }
    }
#endif
}

void CUser::NetWorkError()
{
	char tag[10];
	sprintf(tag, "%d", ALERT_TAG_CONFRIM_NETWORK_ERROR);
	ShowAlertMessage_OK(GDS_Exporter::string_des[3 * TOTAL_STRING_DES + STRING_DES_TEXT_BODY], GDS_Exporter::string_des[3*TOTAL_STRING_DES+STRING_DES_TEXT_TITLE] , tag);
}

void CUser::CheckLocalAndCloudSaveFile(bool isauto)
{
#if ANDROID_OS
	if (!Device::IsNetworkAvailable())
	{
		if (!isauto)
		{
			OS_ShowMessageBox("Error!", "Connection is not available!");
		}
		return;
	}

	if (!GAME()->_async_task_manager._is_login_google)
	{
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
		GAME()->_async_task_manager._is_force_login_google_play = true;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		return;
	}
	Buffer * buffsv = OS_LoadAppData(CLOUD_SAVE_FILE_NAME);
	if (buffsv != NULL)
	{
		char* server_buff = (char*)buffsv->Data();
		if (!isauto)
		{
			GAME()->_async_task_manager._on_process_sync_cloud = true;
		}
		int svScore_cur = GAME()->GetInt64At(server_buff, SAVE_FORMAT_HIGHT_SCORE);
		int svNumPlay = GAME()->GetInt16At(server_buff, SAVE_FORMAT_NUM_PLAY);
		PDEBUG("\nfrom cloud battle_cur = %d", svScore_cur);
		int ver = (int)GAME()->GetInt64At(server_buff, SAVE_FORMAT_CURRENT_VERSION);
		PDEBUG("\n #icloud save_version = %d, svScore_cur = %d, _highScore:%d", ver, svScore_cur, _highScore);
		PDEBUG("\n svNumPlay:%d   ---  numPlayGame:%d", svNumPlay, numPlayGame);
		PDEBUG("\n CURRENT_SAVE_VERSION:%d   --- ver:%d", CURRENT_SAVE_VERSION, ver);
		if (svScore_cur < _highScore)
		{
			ConfirmSyncSaveToCloud(isauto);
		}
		else if (svScore_cur == _highScore)
		{
			if (!isauto)
			{
				if (svNumPlay > numPlayGame)
				{
					ConfirmSyncSaveFromCloud();
				}
				else
				{
					OS_ShowMessageBox("Congratulations!", "Your data has been saved!");
					GAME()->_async_task_manager._on_process_sync_cloud = false;
				}
			}
		}
		else if (svScore_cur > _highScore)
		{
			if (!isauto)
			{
				ConfirmSyncSaveFromCloud();
			}
		}
		if (ver < CURRENT_SAVE_VERSION)
		{
			if (svScore_cur > _highScore)
			{
				if (!isauto)
				{
					ConfirmSyncSaveFromCloud();
				}
			}
			else
			{
				ConfirmSyncSaveToCloud(isauto);
			}

		}

		buffsv->Release();
	}
	else
	{
		ConfirmSyncSaveToCloud(isauto);
	}
#endif
}
void CUser::UpdateLocalSaveFile(Buffer* buffer)
{

#if ANDROID_OS
	PDEBUG("UpdateLocalSaveFile with cloud");
	memcpy(_buffer, buffer->Data(), SAVE_FORMAT_END);
	DataFromBuffer();
	DataToBuffer();
	CheckUnlockAllAchievements();
	OS_SaveAppData(SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
	GAME()->SetGameState(k_GS_LOGO, false);
#endif
}

void CUser::CheckUnlockAllAchievements()
{
#if ANDROID_OS
	bool has_achievement_need_unlock = false;
	//int total = GAME()->_poker_flat_game->_game_design.leveldesign.SheetAchievement.int32Value._totalData;
	if (GAME()->_async_task_manager._need_check_unlock_all_achievement)
	{
		GAME()->_async_task_manager._need_check_unlock_all_achievement = false;
		/*if (GAME()->num_big_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[20] = true;
		}
		else if(GAME()->num_big_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[19] = true;
		}
		if (GAME()->num_large_flower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[22] = true;
		}
		else if (GAME()->num_large_flower >= 500)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[21] = true;
		}
		if (GAME()->num_lily >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[4] = true;
		}
		else if (GAME()->num_lily >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[5] = true;
		}
		else if(GAME()->num_lily >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[6] = true;
		}

		if (GAME()->num_sunflower >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[3] = true;
		}
		else if (GAME()->num_sunflower >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[2] = true;
		}
		else if (GAME()->num_sunflower >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[1] = true;
		}

		if (GAME()->num_violet >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[9] = true;
		}
		else if (GAME()->num_violet >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[8] = true;
		}
		else if (GAME()->num_violet >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[7] = true;
		}

		if (GAME()->num_lotus >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[12] = true;
		}
		else if (GAME()->num_lotus >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[11] = true;
		}
		else if (GAME()->num_lotus >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[10] = true;
		}

		if (GAME()->num_rose >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[15] = true;
		}
		else if (GAME()->num_rose >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[14] = true;
		}
		else if (GAME()->num_rose >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[13] = true;
		}

		if (GAME()->num_orchild >= 10000000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[18] = true;
		}
		else if (GAME()->num_orchild >= 500000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[17] = true;
		}
		else if (GAME()->num_orchild >= 5000)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[16] = true;
		}*/

		/*for (int i = 0; i < game->_hight_score; i++)
		{
		has_achievement_need_unlock = true;
		GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
		}*/
	}
	if (has_achievement_need_unlock && GAME()->_async_task_manager._is_login_google)
	{
		PDEBUG("\n --------------------------->check unlock all achievement");
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
	}
#endif
}
void CUser::ConfirmSyncSaveToCloud(bool isauto)
{
#if ANDROID_OS
	if (isauto)
	{
		UpdateCloudSaveFile();
		GAME()->_async_task_manager._is_force_sync_save_to_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_UPDATE_CLOUND_SAVE);
	}
	else
	{
		sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive sync");
		sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to safeguard your progress to Google Drive?");
		GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_LOCAL;
		GAME()->_async_task_manager._on_process_sync_cloud = true;
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
	}
#endif
}
void CUser::ConfirmSyncSaveFromCloud()
{
#if ANDROID_OS
	sprintf(GAME()->_async_task_manager._message_box_title, "%s", "Google Drive backup");
	sprintf(GAME()->_async_task_manager._message_box_message, "%s", "Do you want to play with your latest Google Drive Backup?");
	GAME()->_async_task_manager._message_box_confirm_type = CAsyncTasks::enumCloudSaveConfirmType::USE_SAVE_FROM_CLOUD;
	GAME()->_async_task_manager._on_process_sync_cloud = true;
	GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHOW_MESSAGE_BOX);
#endif
}

void CUser::UpdateCloudSaveFile()
{
#if ANDROID_OS
	PDEBUG("UpdateCloudSaveFile with local");
	OS_SaveAppData(CLOUD_SAVE_FILE_NAME, _buffer, MAX_SAVE_BUFFER_CACHE, false);
#endif
}