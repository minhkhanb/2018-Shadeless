/**
*	will be included in CGame.h
*	specific vars and methods for gameplay
*/

public:
//Save System.
void GameDataLoad();
void GameDataSave();
void GameDataDefault();
void GameDataToBuffer();
void GameLoadDataFromBuffer();

BYTE red, green, blue, alpha;
#define GAME_FILE	"pinerecord.dat"

#define GAME_VERSION				100

#define GAME_DATA_VERSION			(0)

#define GAME_DATA_LENGTH				(GAME_DATA_VERSION + 4)



char _buffer_save[GAME_DATA_LENGTH];
int _game_version;
int _rate;

//PineHighScore _pineHighScore;

 long _sys_begin_second;
 long _sys_millisecond;
 long _game_millisecond;
 long _sys_second;
void UnloadGameBackground();
bool LoadGameForceground();
bool _needresume_when_playing;
void PurchaseIAP(const char* stringID, enumIAPResponseStatus IsSucess);


int  _frame_auto_stop;
PineImage _image;

//Port
PinePoint _point_o, _point_right_bottom;
float _screen_width;
float _screen_height;
float _port_scale_x;
float _port_scale_y;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// for game
void RenderGameState();
void UpdateGameState();
bool CanUseTouch();
void ProcessingTag(int tag, int indexButton);

CGameStateLogo _gameStateLogo;
CGameStateGamePlay _gameStateGamePlay;

#ifdef MAC_OS
PineSocial *_gcSocial;
#endif
CUser _user;
CAchievement _achievement;
bool _canTouch,_hasAction;

int _idTouchProcessing;
bool _isAdsProcessing;
bool CheckTouchProcessing(int i = 0);
void SetTouchProcessing(int value = -1) { _idTouchProcessing = value; };
void ClearTouchProcessing() { _idTouchProcessing = -1; };
CAnimObject _animLoading;
int _soundButtonID;
void PlaySoundButton();
void AddNotify();
INT64 _timeOut;
int _numGameArow,_oldScore;
int _numShowAds,_timesShowAdsMod,_stateShowPineAds,_numOpenApp,_timesShowAdsVideo;
int _showPineAds;

//--------------------PORT GAME--------------------//
PinePoint _pointLeftTop, _pointRightBottom;
void MeasureButton(Button* button);
void MeasureButton(Button* button, int anchor);
void MeasureButtonX(float posx, Button* button, BYTE anchor_horizonal);
void MeasureButtonY(float posy, Button* button, BYTE anchor_vertical);
PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid);
void MeasureAnim(CAnimObject* obj);
PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, float scalex, float scaley);
PinePoint MeasureFrame(CSprite* sprite, UINT32 frameid, BYTE anchor);
float _screenWidth;
float _screenHeight;
//////////////////////PORT GAME/////////

#if defined (ANDROID_OS)
CAsyncTasks _async_task_manager;
BoardScoreItem		_gameBoardScoreItem;
#endif

//Achievement
void CheckCompleteAchievement();
void LoadAchievement();
int GetPercent(int index);
bool CheckAchieveComplete(int index);


enum enumAchieveStatus {
	ACHIEVEMENT_NONE,
	ACHIEVEMENT_PROCESS,
	ACHIEVEMENT_COMPLETED
};


int maxAchieve[TOTAL_ACHIEVEMENT];
int percentAchieve[TOTAL_ACHIEVEMENT];
int currentAchieve[TOTAL_ACHIEVEMENT];
BYTE isCompleteAchieve[TOTAL_ACHIEVEMENT];

void SubmitAchievement(int index);
void UpdateStatusAchievement();
void SetAchievementSubmitCompleted(int index);
void SubmitHighScore();

enum enumAchievement
{
	Achievement_0,
	Achievement_1,
	Achievement_2,
	Achievement_3,
	Achievement_4,
	Achievement_5,
	Achievement_6,
	Achievement_7,
	Achievement_8,
	Achievement_9,
	Achievement_10,
	Achievement_11,
	Achievement_12,
	Achievement_13,
	Achievement_14,
	Achievement_15,
	Achievement_16,
	Achievement_17,
	Achievement_18,
	Achievement_19,
	Achievement_20,
	Achievement_21,
	Achievement_22,
	Achievement_23,
	Achievement_24,
	Achievement_25,
	Achievement_26
};

//admod
bool show_ads_interrupt;
int timeCountShowAdsInterrupt;
#define TIME_SHOW_ADS	1*60

//Update rate function
enum enumStarRateStatus {
	RATE_STATE_NONE,
	RATE_STATE_HATED,
	RATE_STATE_DISLIKED,
	RATE_STATE_OK,
	RATE_STATE_LIKED,
	RATE_STATE_LOVED
};

int stateRateGame;
bool isRated;

//CDialog _dialog;

void InitRating();
void RenderRating(float x, float y, int opac);
void UpdateRating();
bool UpdateTouchRating();

bool _isWaiting;
INT_TIME _timeOutGame;



//Google service
void GGPlayShowLeaderBoard();
void GGPlaySynDriver();
void GGPlayShowAchievement();
void GGPlaySubmitAchievement(const char ggPlayID_ACHIEVEMENT[128]);

#if !defined (MAC_OS)
bool			_isBackkeyPress;
int				_waitForProcessBackkeyCount;
#endif

int countShowRateUs;

//firebase
void LogsPlaySession(int count);
void LogsScoreSession(int count);


//admod
bool InitAds();
void ShowAds();
void ShowBannerAds();
void HideBannerAds();
void UpdateAds();
void PrepareAndShowInterstitial();
void PrepareAndShowVideoAds();

int _count_frame_show_ads;
int _time_delay_show_ads;
bool _show_ads_interrupt;
int _time_count_for_show_ads_banner;
int _admob_play_count;
bool _has_show_ads_banner;
#if defined (ANDROID_OS)
PineTapdaqAsync _tapdaq_manager;
bool _on_show_video_ads = false;
bool _on_video_ads_reward = false;
bool _is_waiting_for_reward = false;
#endif

//NDT Topebox ads
TopeBox_PromoGame _topebox_game;
float _topebox_game_scrollEff;

char gameName[128];
char type[128];
char linkStore[128];
char linkIcon[256];
int revison;
int num_play_to_show_topebox_ads;
bool closeTopeboxAds;
bool disableTopeboxAds;
bool configDownloaded;

int time_get_remote_config;