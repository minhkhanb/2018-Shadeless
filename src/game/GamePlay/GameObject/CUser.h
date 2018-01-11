#if !CUSER_H
#define CUSER_H 1

#define MAX_SAVE_BUFFER_CACHE					10000

#define SAVE_GAME_VERSION_0						0 //version 1.0.0
#define SAVE_GAME_VERSION_1						1 //version 1.0.0
#define SAVE_GAME_VERSION_2						2	//update 18/12/2017
#define SAVE_GAME_VERSION_3						3	//update 26/12/2017

#define CURRENT_SAVE_VERSION					SAVE_GAME_VERSION_2 //version 1.0.0

#define CLOUD_SAVE_FILE_NAME "clouddata.pine"


#define SAVE_FILE_NAME "data.pine"


#define SAVE_FORMAT_CURRENT_VERSION				(0)

#define SAVE_FORMAT_NUM_PLAY					(SAVE_FORMAT_CURRENT_VERSION + 8) //INT_TIME
#define SAVE_FORMAT_HIGHT_SCORE					(SAVE_FORMAT_NUM_PLAY + 8) //INT32

#define SAVE_FORMAT_MUSIC						(SAVE_FORMAT_HIGHT_SCORE + 4)//BYTE
#define SAVE_FORMAT_SOUND						(SAVE_FORMAT_MUSIC + 1)//BYTE
#define SAVE_FORMAT_NOTIFY						(SAVE_FORMAT_SOUND + 1)//BYTE

#define SAVE_FORMAT_REMOVE_ADS					(SAVE_FORMAT_NOTIFY + 1)
#define SAVE_FORMAT_HAD_RATE					(SAVE_FORMAT_REMOVE_ADS +1)
#define SAVE_FORMAT_HAD_LOGIN_FB				(SAVE_FORMAT_HAD_RATE +1)
#define SAVE_FORMAT_USE_ICLOUD					(SAVE_FORMAT_HAD_LOGIN_FB +1)

#define SAVE_FORMAT_IS_UNLOCK_COLOR				(SAVE_FORMAT_USE_ICLOUD + 1)
#define SAVE_FORMAT_CUR_COLOR_USE				(SAVE_FORMAT_IS_UNLOCK_COLOR +1)

#define SAVE_FORMAT_NUM_PLAY_RATE				(SAVE_FORMAT_CUR_COLOR_USE +1)
#define SAVE_FORMAT_STEP_TUTORIAL				(SAVE_FORMAT_NUM_PLAY_RATE +2)

#define SAVE_FORMAT_ACHIEVE_CURR_VALUE			(SAVE_FORMAT_STEP_TUTORIAL +2)
#define SAVE_FORMAT_ACHIEVE_STATE				(SAVE_FORMAT_ACHIEVE_CURR_VALUE +2*MAX_ACHIEVEMENT_VER_0)

#define SAVE_FORMAT_SCORE_COLOR						(SAVE_FORMAT_ACHIEVE_STATE +1*MAX_ACHIEVEMENT_VER_0)
#define SAVE_FORMAT_DAY_PLAY							(SAVE_FORMAT_SCORE_COLOR +1*TOTAL_COLOR_DES_ROW)
#define SAVE_FORMAT_DAY_RESET							(SAVE_FORMAT_DAY_PLAY + 2)

#define SAVE_FORMAT_USER_HAS_TOUCH_PROMO					(SAVE_FORMAT_DAY_RESET + 2)	
#define SAVE_FORMAT_USER_GAME_PROMO_ICON_ID				(SAVE_FORMAT_USER_HAS_TOUCH_PROMO + 256)
#define SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE			(SAVE_FORMAT_USER_GAME_PROMO_ICON_ID + 128)
#define SAVE_FORMAT_USER_GAME_PROMO_REVISION			(SAVE_FORMAT_USER_GAME_PROMO_LINK_STORE + 128)
#define SAVE_FORMAT_USER_GAME_PROMO_ORDER				(SAVE_FORMAT_USER_GAME_PROMO_REVISION + 2)
#define SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW			(SAVE_FORMAT_USER_GAME_PROMO_ORDER + 2)
#define SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID			(SAVE_FORMAT_USER_GAME_PROMO_HAS_SHOW + 1)
#define SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW			(SAVE_FORMAT_USER_GAME_PROMO_DEVICE_ID + 128)

#define SAVE_FORMAT_END							(SAVE_FORMAT_USER_GAME_PROMO_DELAY_SHOW + 2)



//icloud
#define BEGIN_TAG								10
#define ALERT_TAG_CONFRIM_SYNC_ICOUND			(BEGIN_TAG + 0)
#define ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE		(ALERT_TAG_CONFRIM_SYNC_ICOUND + 1)
#define ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE	(ALERT_TAG_CONFRIM_ICOUND_LOAD_SAVE + 1)
#define ALERT_TAG_CONFRIM_NETWORK_ERROR			(ALERT_TAG_CONFRIM_ICOUND_SYNC_COMPLETE + 1)
class CUser
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	CUser()
	{
        
	};

	INT32 _highScore;
	bool _removeAds;    
	INT_TIME numPlayGame;
	
	int _dayPlayGame;
	int _dayResetGame;

	int _numPlayRate;
	bool _hadRate;
	bool _hadLoginFB;

	bool _isUnlockColor;
	BYTE _curColorUse;
	int _stepTutorial;

	void SetGame();
	//save game
	INT_TIME _versionFormat;
	char _buffer[MAX_SAVE_BUFFER_CACHE];

	void DataDefault();
	void DataSave();
	void DataLoad();
	void DataFromBuffer();
	void DataToBuffer();
	void SynForNewVersion(INT_TIME fromVersion);
	void NewDataDefault(INT_TIME version);
    
    //check syn icloud  18/May/2016    
    void ResetValueICloud();
    bool SaveToICloud();
    int CheckCanPushDataSave();
    void LoadFromICloud();
    void DefaultICloud();
    void CheckICloud();
    int _icloudStatus;
	BYTE _isUseIcloud;

	enum enumUseICloud
	{
		ICLOUD_NONE,
		ICLOUD_USE,
		ICLOUD_DONT_USE,
	};
    enum enumStateiCloud
    {
        ICLOUD_STATE_NODATA,
        ICLOUD_STATE_DISCONECT,
        ICLOUD_STATE_OLD_VERSION,
        ICLOUD_STATE_NEW_VERSION,
        ICLOUD_STATE_READY_SYNS,
    };
    
	char _fbID[SOCIAL_ID_STRING_LENGTH];
	INT32 _userNameUnicode[SOCIAL_ID_STRING_LENGTH];
	int _userNameUnicodeLenght;
	int _avatarState;
	char _avatarURL[124];
	void NetWorkError();

	int _scoreColor[TOTAL_COLOR_DES_ROW];

	enum achieveStatus
	{
		ACHIEVEMENT_STATE_NONE,
		ACHIEVEMENT_STATE_COMPLETED,
		ACHIEVEMENT_STATE_SUBMITTING,
		ACHIEVEMENT_STATE_SUBMITED_SUCCESS,
	};

	int _achievement_status[TOTAL_ACHIEVEMENT];

	void ConfirmSyncSaveFromCloud();
	void ConfirmSyncSaveToCloud(bool isauto);
	void CheckLocalAndCloudSaveFile(bool isauto);
	void UpdateCloudSaveFile();
	void UpdateLocalSaveFile(Buffer * buffer);
	void CheckUnlockAllAchievements();

	//Update 26/12/2017
	bool _has_touch_promo;
	char _game_promo_icon_id[256], _game_promo_link_store[128];
	int _game_promo_revision;
	bool _game_promo_has_show;
	int _game_promo_order;
	char _game_promo_deviceID[128];
	INT_TIME _game_promo_delay_show;
};

#endif