//FPS
#define FPS_MAX						60
#define FPS_DELAY_TIME				(1000/CGame::_this->FPS_LIMIT)

//Game State
#define k_GS_LOGO					1
#define k_GS_GAME_PLAY				2



//Game Sub State
#define k_INIT						1
#define k_FIRST_TIME				2
#define k_LOOP						3
#define k_DESTROY					4
#define k_EFFECT_UNLOAD				5

#define k_MACHINE_FAKE_OUT			5
#define k_MACHINE_WIN				6

#define SAVE_CHARACTER_NAME_LENGTH_CATCH			32

#define POS_X(a)   (_game->_game_context.ScaleOutX*a)
#define POS_Y(a)   (_game->_game_context.ScaleOutY*a)

#define FIREBASE_CONFIG_SAVE_FILE_NAME	"android.config"

#define FIREBASE_BUTKET_URL  "gs://topeboxads-fe304.appspot.com"

#define FIREBASE_REQUEST_URL  "https://firebasestorage.googleapis.com/v0/b/topeboxads-fe304.appspot.com/o/TopeAds%2FSkyDancer%2Fandroid.config?alt=media&token=a4f0d4d5-e744-4d6f-a568-741ab6bd0f77"

#define TIME_REMOTE_CONFIG		(5*60)
#define TIME_SHOW_CLOSE_BANNER_TOPEBOX		(3*60)	//2 seconds