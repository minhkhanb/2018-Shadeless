#if !defined(_H_CGAME_STATE_PLAY)
#define _H_CGAME_STATE_PLAY

class CGameStateGamePlay :public CGameState
{
public:

	CGameStateGamePlay() {};

	void InitGameState();
	void Render(float x, float y);
	void Update();
	bool UpdateTouchButton();
	void UpdateTouch();
	BYTE _status;
	enum enumStatusState
	{
		STATE_MENU,
		STATE_GAME,
	};
	CMainMenu _mainMenu;
	CGamePlay _gamePlay;
	

	int _stateTransition;
};


#endif