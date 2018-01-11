/**
*	will be included in CGame.h
*	for include specific *.h
*/
//#include "some_classes.h"

#include "../GamePlay/GameObject/MenuSystem.h"
#include "../GamePlay/GameObject/SpriteList.h"
#include "../GamePlay/GameObject/WorldGame.h"
#include "../GamePlay/GameObject/Jumper.h"

#include "../Lib/PineHighScore.h"
#include "../Lib/DataType/BufferStream.h"
//include game object
#include "../Lib/PineAdBox.h"

//game object
#include "../GamePlay/GameObject/Pine_Date_Time.h"
#include "../GamePlay/GameObject/COffsetScroll.h"
#include "../GamePlay/GameObject/CScrollBar.h"
#include "../GamePlay/GameObject/GDS_Exporter.h"
#include "../GamePlay/GameObject/CAchievement.h"
#include "../GamePlay/GameObject/CUser.h"
//#include "../GamePlay/GameObject/CServerThread.h"
//#include "../GamePlay/GameObject/CFriendManager.h"

#if defined (ANDROID_OS) || _WINDOWS
#include "../GamePlay/GameObject/CSetting_update.h"
#else
#include "../GamePlay/GameObject/CSetting.h"
#endif
#include "../GamePlay/GameObject/CGGServices.h"
#include "../GamePlay/GameObject/CShareScore.h"
#include "../GamePlay/GameObject/CMainMenu.h"
#include "../GamePlay/GameObject/CGamePlay.h"
#include "../GamePlay/GameObject/Topebox_PromoGame.h"


//game play
#include "../GamePlay/GameState/CGame_State.h"
#include "../GamePlay/GameState/CGame_State_Logo.h"
#include "../GamePlay/GameState/CGame_State_Game_Play.h"



//////////

//Port 
#if defined (ANDROID_OS)
#include "../Lib/PineTapdaqAsync.h"
#include "../GamePlay/GameObject/CAsyncTasks.h"
#endif


