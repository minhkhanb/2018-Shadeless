#if !CSHARE_SCORE
#define CSHARE_SCORE 1


#define SHARE_SCORE_BUTTON_NUMS			2

class CShareScore
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	CShareScore()	{};

	int _type;
	enum enumTypeRender
	{
		TYPE_NONE,
		HIGHT_SCORE,
		RATE_US,
		CONFIRM_RATE_US
	};

	
	int _hightScore;
	void Init(int score,int type);
	void UpdateHighScore();
	void RenderHighScore(float x, float y);

	bool _isTouchDrag;
	int _stateHightScore, _stateRotaItem, _stateRotaIcon,_timeRoteItems;
	float _offRotaItem, _speedRotaItem;
	int _moveDirection,_stateMoveTarget;

	enum enumShareButton
	{
		SHARE_NEXT,
		SHARE_YEAH,
	};

	Button _shareButton[SHARE_SCORE_BUTTON_NUMS];
	bool UpdateTouchButton();
	//
	void AddShare();
	void RenderShare(float x, float y);
	bool UpdateShare();


	int _stateShare;
	int _opacityShare ;

	int _opacityTitle ;
	int _stateRota ;
	float _rotaTile ;
	float _speedRota ;
    
    char _stringFeed[1024];

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
	bool isCompleteRate;
	
};

#endif