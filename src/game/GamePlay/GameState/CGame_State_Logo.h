#if !defined(_H_CGAME_STATE_LOGO)
#define _H_CGAME_STATE_LOGO

#define MAX_LOADING_STEP				30

class CGameStateLogo :public CGameState
{
public:

	CGameStateLogo(){};

	void InitGameState();
	void Render(float x, float y);
	void Update();
	bool UpdateTouchButton();
	void UpdateTouch();
	bool UpdateTransition();
	void SetTransition();

	int _loadingStep;
	bool _autoLoading;
	enum enumStepLoading
	{
        STEP_NONE,
		STEP_START,		
		STEP_NETWORK_ERROR,
		//end loading resource
		STEP_LOAD_RESOURCE,		
	};

    bool _antialias, _trueColor;
};

#endif