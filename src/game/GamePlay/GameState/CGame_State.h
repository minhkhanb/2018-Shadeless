#if !defined(_H_CGAME_STATE)
#define _H_CGAME_STATE


class CGameState
{
protected:
	CGame *_game;
	Graphic *_gp;

public:

	CGameState();
	~CGameState();
	virtual void InitGameState();	
	virtual void Render(float x, float y) = 0;
	virtual void Update() = 0;
	virtual bool UpdateTouchButton() = 0;
	virtual void UpdateTouch() = 0;
	
	
	void SetStateGameOld(int state){ _gameStateOld = state; };
	int _gameStateOld;
	int _touchID;
	
};

#endif
