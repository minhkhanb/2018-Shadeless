#if !CMAIN_MENU
#define CMAIN_MENU 1


#if defined (ANDROID_OS) || _WINDOWS
	#define MENU_ITEMS_NUMS			7
#else
	#define MENU_ITEMS_NUMS			5
#endif
class CMainMenu
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	CMainMenu()	{};

	void Init();
	void Update();
	void Render(float x, float y);

	bool _isTouchDrag;
	int _stateGoPlay, _stateRotaItem, _timeRoteItems;
	float _offRotaItem, _speedRotaItem;
	int _moveDirection,_stateMoveTarget;
	void GoToPlay(int state);
	void UpdateGoPlay();

	enum enumMenuItems
	{
		ITEM_ACHIEVEMENT,
		ITEM_LEADERBOARD,
#if defined (ANDROID_OS) || _WINDOWS
		ITEM_GGSERVICES,
		ITEM_REMOVEADS,
#endif
		ITEM_PLAY,
		ITEM_SHARE,
		ITEM_SETTING
	};
	Items _menuItems[MENU_ITEMS_NUMS];
	void RenderItems(float x, float y);
	void UpdateItems();
	float _touchDragX, _touchDragY;
	float _offsetAngle,_touchAngle;
	int _currentItems,_targetItems;
	float _speedMove;
	int GetCurrentItems();
	bool UpdateTouchButton();
	void UpdateTouch();
	int GetIndexTouch(float x, float y);
	int _touchID, _touchIDOld;

	bool _isNewScore;

	//logo;
	int _stateLogo;
	float _logoY;

	int _stateHighScore, _highScore;
	float _highScoreY;

	int _stateBestScore, _bestScore;
	float _bestScoreY;
	void InitResult_Logo(int highScoe, bool isStart);
	void RenderResult_Logo(float x, float y);
	void UpdateResult_Logo();
	void InitMenu();
	CScrollBar _scrollBarX;
	float _offsetDragX ;
	int _timeMove ;
	int _opacityItems;
	CShareScore _shareScore;
	CSetting _setting;
	CGGServices _ggservices;
	bool _isRenderGGServices;
	bool _isRenderSetting;
	int _stateSeting;
	int _stateGGServices;
	void UpdateStateSetting();
	void UpdateStateGGServices();
	void OpenSetting();
	void OpenGGServices();
	void CloseSetting();
	void CloseGGServices();
};

#endif