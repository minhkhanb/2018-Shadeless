#if !CGGSERVICES
#define CGGSERVICES 1


#define CGGSERVICES_ITEMS_NUMS			4

class CGGServices
{
private:
	CGame *_game;
	Graphic *_gp;

	COffsetScroll _offsetScroll;
	float _beginAngleScroll, _offAngleScroll;

public:
	CGGServices() {};

	void Init();
	void Update();
	void Render(float x, float y);

	bool _isTouchDrag;
	int _state, _stateRotaItem, _stateRotaIcon, _timeRoteItems;
	float _offRotaItem, _speedRotaItem;
	int _stateMoveTarget;
	//1: theo chieu dong ho, 2: nguoc chieu
	int _moveDirection;

	enum enumGGItems
	{
		GGSERVICES_LEADERBOARD,
		GGSERVICES_ACHIEVEMENT,
		GGSERVICES_SYNC,
		GGSERVICES_BACK
	};
	Items _ggplayItems[CGGSERVICES_ITEMS_NUMS];
	void RenderItems(float x, float y);
	void UpdateItems();
	float _touchDragX, _touchDragY;
	float _offsetAngle, _touchAngle;
	int _currentItems, _targetItems;
	float _speedMove;
	int GetCurrentItems();
	bool UpdateTouchButton();
	void UpdateTouch();
	int GetIndexTouch(float x, float y);
	int _touchID, _touchIDOld;

	float _offsetDragX;
	int _timeMove;
	int _opacityItems;


};

#endif