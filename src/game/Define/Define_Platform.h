#define MAX_TOUCHES 					5

// hardware type

#define DEVICE_IPHONE				0
#define DEVICE_IPHONE_3G			1
#define DEVICE_IPOD					2
#define DEVICE_IPHONE_3GS			3
#define DEVICE_IPOD_4				4
#define DEVICE_IPHONE_4G			5
#define DEVICE_IPAD					6
#define DEVICE_SIMULATOR			7

#define UNIVERSAL_BUILD				0

#define DEFAULT_SCR_W				480
#define DEFAULT_SCR_H				320
#define DEFAULT_DIV_W				480
#define DEFAULT_DIV_H				320

#define D4_SCR_W					480
#define D4_SCR_H					320
#define D4_DIV_W					480
#define D4_DIV_H					320

#define IPAD_SCR_W					1024
#define IPAD_SCR_H					768
#define IPAD_DIV_W					1024
#define IPAD_DIV_H					768
#define ADD_ADVERT	0
#define FREE_GAME_OF_THEDAY         0
#define ADD_PICK_IMAGE              1

#define PICK_STATE_NONE             0
#define PICK_STATE_TAKE_PICTURE     1
#define PICK_STATE_CHOICE_PICTURE   2
#define ENABLE_SERVER_PUSH_NOTIFICATION 1
#define IS_DEVELOPMENT_VERSION 1

#define USE_GL_SCALE

#define APPLE_ID 1124099261
#define ADVERT_APPLE_ID APPLE_ID

#define GAME_CENTER_FRIEND_NUM 1

#define CA_CERT_PATH        (GetBundlePath())
#if defined(IS_DEVELOPMENT_VERSION)

#define RSA_CLIENT_CERT     GetRealFilePath("apns-dev-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-dev-key.pem")


#define APPLE_HOST          "gateway.sandbox.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.sandbox.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#else

#define RSA_CLIENT_CERT     GetRealFilePath("apns-production-cert.pem")
#define RSA_CLIENT_KEY      GetRealFilePath("apns-production-key.pem")


#define APPLE_HOST          "gateway.push.apple.com"
#define APPLE_PORT          2195

#define APPLE_FEEDBACK_HOST "feedback.push.apple.com"
#define APPLE_FEEDBACK_PORT 2196
#endif

#define USE_NEW_SYSTEM_VIEW_IOS_7_8 1
#define IOS8_MASTER_W (750.0f)
#define IOS8_MASTER_H (1334.0f)

#define ADD_CHARTBOOST 0
#define ADD_ADVERT  1
#define ADD_IADVERT 1
#define USE_ADBOX   1
#define ADD_ADMOB   1

#define ADBOX_CONFIG_FILE "adbox.config"
#define CHARTBOOST_CONFIG_FILE "chartboost.config"
#define TAG_LINE "#Shadeless"

#define USE_PINESOCIAL  1
#define ADD_GAMECENTER  1
#define ADD_TWITTER     0
#define ADD_FACEBOOK    0
#define ADD_IAP         1

#define FACEBOOK_TRANSFER_IAP       (0)
#define FACEBOOK_API                (20)

#define FACEBOOK_PERMISSION [@"email",@"basic_info"]
#define FACEBOOK_PERMISSION_FOR_STREAM [@"email",@"basic_info",@"publish_actions"]
#define FACEBOOOKAUDIENCE ACFacebookAudienceEveryone

#define PURCHASE_FUNC(x,y) {CGame::_this->PurchaseIAP(x,y);}

#define ADD_SCREEN_CAPTUREVIEW 1
#define FPS_VIDEO_RECORD 30
#define CAPTURE_VIDEO_W 480
#define CAPTURE_VIDEO_H 640

#define PROCESSING_TAG(a,b)     {CGame::_this->ProcessingTag(a,b);}

#ifdef MAC_OS
#define NOTIFICATION_MAX        50
#define NOTIFY_RANGE_NUM        17
#define NOTIFY_RANGE_VALUE     {6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22}
#define ADD_PINE_ADVERT 1
#endif


#define DEFINE_MASTER_IOS "i6"

#define ADMOB_ID1_DEFAULT "6305464951596521"
#define ADMOB_ID2_DEFAULT "4047328291"

#define CHARTBOOST_ID1_DEFAULT "54648ad443150f4c5999a14e"
#define CHARTBOOST_ID2_DEFAULT "4def37641437c3d65b0967f667fbb51040a074f9"

#define UNITY_ADS_ID_DEFAULT "118353"
#define ADD_UNITYADS            1

#define ADD_VUNGLEADS           1
#define VUNGLE_ID_DEFAULT       "576444291b10f2b71400005e"

#define ADS_CONFIG_URL      "https://datancus.blob.core.windows.net/pinecrossingads/1124099261/ads.config"
#define ADS_CONFIG_FILE     "ads.config"

#define USE_IPAD_RETINA         1
#define USE_BLEND_MODE

#define ADS_VERSION   ""

#define TOTAL_ACHIEVEMENT		(50)
#define GP_LEADERBOARD_ID		"CgkI6KbY4qUTEAIQDg"

#define TIME_SHOW_RATE_US				(3*60*60)

#if !_WINDOWS
#define ADD_GOOGLE_ANALYTICS	1
#endif

#define TIME_DELAY_SHOW_ADS					3.0f*60

#define ADD_REMOTE_CONFIG           1

#define TIME_DELAY_SHOW_TOPEBOX_ADS			8*60*60;