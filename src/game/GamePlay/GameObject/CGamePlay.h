#if !defined(_H_CGAME_PLAY)
#define _H_CGAME_PLAY

#define MAX_COLOR_DES		100

struct  Circle
{
	float _x, _y;
	int _opacity;
	float _radius,_oldRaidus, _upRadius, _downRadius;	
	COLOR _color;
};
struct ScoreTime
{
	int _time, _score;
	int _state;
};
#define MAX_SCORE_TIME		200
class CGamePlay
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	CGamePlay() {};
 
	void Init();
	void Render(float x, float y);
	void Update();
	void UpdateTouch();

	void NewLocation(bool isStart = false);
	int GetLevelDes(int score);
	COLOR ConvertRGBtoHEX(int R, int G, int B);
	void ConvertHEXtoRGR(COLOR color, int &R, int &G, int &B);

	COLOR _colorDes[MAX_COLOR_DES];
	void InitColorDes();
	void SetNewColor();

	int GetIndexColor();
	int GetIndexColor(COLOR color);
	int GetIndexColor(int rand, bool foreGand = false);

	bool _isNewScore;
	COLOR _bgColor;
	Circle _circle,_newCircle;
	int _indexColor;
	int _score, _level, _rangeDes,_rangeRate;
	BYTE _stateGame;
	bool _beginGame;		

	float _maxRadiusGlow,_percentGlow;
	float _percent;
	float _maxRadius;
	float GetMaxRange(float x, float y, float w, float h, int cX, int cY);
	
	enum enumStateGame
	{
		GAME_START,
		GAME_START_NEW,
		GAME_DOWN_RADIUS,
		GAME_UP_RADIUS,
		GAME_OVER,
		GAME_END,
		GAME_BONUS_SCORE,
	};
	void SetState(BYTE state);
	bool isMoveToTarget(float &x, float &y, float targetX, float targetY, float speed);
	bool CheckRectOverlapCircle(int x, int y, int w, int h, int cX, int cY, int R);
	bool CheckCircleOverlapCircle(int cX1, int cY1, int R1, int cX2, int cY2, int R2);
	bool CheckCircleFillFullRect(int x, int y, int w, int h, int cX, int cY, int R, int over);

	void UpdateTutorial();
	void RenderTutorial(float x, float y);
	void SetTutorial(int step);
	//touch Processing
	int _touchID, _touchIDOld;

	float ReadDesParam(int rule, int level, int param);
	int _ruleDes;
	enum enumRuleCircle
	{
		CIRCLE_SIZE_MAX,
		CIRCLE_SIZE_NORMAL,
		CIRCLE_SIZE_MIN,
	};
	
	static  const COLOR GetColorDes(int i, int param);

	////
	int _stateBonus;
	float _zoomBonus;
	int _opaBonus;
	int _nextScore;

	int _stateScore;
	int _opacityScore;
	float _speedMove ;
	float _speedRadius;
	int _timeTouch,_timeDelay ;

	////////
	int _step;
	int _stateTutorial;
	float _offTutorial;

	ScoreTime _scoreTime[MAX_SCORE_TIME];
	void InitScoreTime();
	void AddScoreTime(int score);
	void UpdateScore();
	int GetScoreTime();

	int _tapNoScore;
	void GetNewPosision(int oldX, int oldY);
};
#endif
