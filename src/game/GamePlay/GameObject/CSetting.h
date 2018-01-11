#if !CSETTING
#define CSETTING 1


#define SETTING_ITEMS_NUMS			10

struct Items
{
	float _x, _y;
	float _angle, _angleCheck, _rotaIcon;
	int _frame, _opacity;
	int _state;
	Button _button;
};

class CSetting
{
private:
	CGame *_game;
	Graphic *_gp;

	COffsetScroll _offsetScroll;
	float _beginAngleScroll, _offAngleScroll;

public:
	CSetting()	{};

	void Init();
	void Update();
	void Render(float x, float y);

	bool _isTouchDrag;
	int _state, _stateRotaItem, _stateRotaIcon,_timeRoteItems;
	float _offRotaItem, _speedRotaItem;
	int _stateMoveTarget;
	//1: theo chieu dong ho, 2: nguoc chieu
	int _moveDirection;

	enum enumSettingItems
	{		
		SETTING_SILENCE,
		SETTING_CREDIT,
		SETTING_CONTACT,
		SETTING_ICLOUD,			//3
		SETTING_BACK,
		SETTING_RESTORE,		//5
		SETTING_COLOR,
		SETTING_REMOVE_ADS,		//7
		SETTING_NOTIFI,
		

		BACK_CREDIT,
	};
	Items _settingItems[SETTING_ITEMS_NUMS];
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
	
	//CScrollBar _scrollBarX;

	int _stateCredit;
	bool _isRenderCredit;
	float _creditAngle;
	void UpdateCredit();
	void RenderCredit(float x, float y);
	void OpenCredit();
	void CloseCredit();

	float _offsetDragX ;
	int _timeMove ;
	int _opacityItems ;


};

#endif