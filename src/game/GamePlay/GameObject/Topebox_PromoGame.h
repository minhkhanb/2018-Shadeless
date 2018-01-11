#ifndef _TOPEBOX_PROMOGAME_H_
#define _TOPEBOX_PROMOGAME_H_



#if defined(MAC_OS)
#define PROVIDER_ID				"d08cf3d1-7e6d-4856-835b-b829ebf8e816"
#define PROVIDER_SECRET_KEY		"C55DDDCEA3313FC3A2D50954A0E9007281CDCFDF8DD20CE693AEA7EC1D401AE4"
#elif defined (ANDROID_OS)
#define PROVIDER_ID				"8c1b2b76-3489-4e47-a684-3c31d6bdb766"
#define PROVIDER_SECRET_KEY		"E263761657F929460E48E200C4D801B9D86544E5435B0D5AA539CF6CAD6DC87E"
#else
#define PROVIDER_ID				"8c1b2b76-3489-4e47-a684-3c31d6bdb766"
#define PROVIDER_SECRET_KEY		"E263761657F929460E48E200C4D801B9D86544E5435B0D5AA539CF6CAD6DC87E"
#endif

#define SERVER_TOPEBOX_URL				"http://ads.topebox.com/ads/v2/delivery"
#define SERVER_TOPEBOX_URL_RESPONSE		"http://ads.topebox.com/ads/v1/response"



#define GAME_ICON_FILE_NAME		"gameIcon.png"

#define GAME_PROMO_RENDER_X		k_SCREEN_WIDTH
#define GAME_PROMO_RENDER_Y		(136)


#include <string>

class TopeBox_PromoGame
{
	class SHA256
	{
	protected:
		typedef unsigned char uint8;
		typedef unsigned int uint32;
		typedef unsigned long long uint64;

		const static uint32 sha256_k[];
		static const unsigned int SHA224_256_BLOCK_SIZE = (512 / 8);
	public:
		void init();
		void update(const unsigned char *message, unsigned int len);
		void final(unsigned char *digest);
		static const unsigned int DIGEST_SIZE = (256 / 8);

	protected:
		void transform(const unsigned char *message, unsigned int block_nb);
		unsigned int m_tot_len;
		unsigned int m_len;
		unsigned char m_block[2 * SHA224_256_BLOCK_SIZE];
		uint32 m_h[8];
	};

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
std::string base64_decode(std::string const& encoded_string);

std::string CreateSHA256(std::string input);



#define SHA2_SHFR(x, n)    (x >> n)
#define SHA2_ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define SHA2_ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define SHA2_CH(x, y, z)  ((x & y) ^ (~x & z))
#define SHA2_MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_F1(x) (SHA2_ROTR(x,  2) ^ SHA2_ROTR(x, 13) ^ SHA2_ROTR(x, 22))
#define SHA256_F2(x) (SHA2_ROTR(x,  6) ^ SHA2_ROTR(x, 11) ^ SHA2_ROTR(x, 25))
#define SHA256_F3(x) (SHA2_ROTR(x,  7) ^ SHA2_ROTR(x, 18) ^ SHA2_SHFR(x,  3))
#define SHA256_F4(x) (SHA2_ROTR(x, 17) ^ SHA2_ROTR(x, 19) ^ SHA2_SHFR(x, 10))
#define SHA2_UNPACK32(x, str)                 \
{                                             \
    *((str) + 3) = (uint8) ((x)      );       \
    *((str) + 2) = (uint8) ((x) >>  8);       \
    *((str) + 1) = (uint8) ((x) >> 16);       \
    *((str) + 0) = (uint8) ((x) >> 24);       \
}
#define SHA2_PACK32(str, x)                   \
{                                             \
    *(x) =   ((uint32) *((str) + 3)      )    \
           | ((uint32) *((str) + 2) <<  8)    \
           | ((uint32) *((str) + 1) << 16)    \
           | ((uint32) *((str) + 0) << 24);   \
}

public:
	TopeBox_PromoGame();
	~TopeBox_PromoGame();

	enum enumTypeTopeboxAds
	{
		TOPEBOX_ADS_ICON,
		TOPEBOX_ADS_BANNER
	};

	struct ButtonClose
	{
		int x;
		int y;
		int w;
		int h;
	};


	enumTypeTopeboxAds typeAds;

	char _sign_str[256];

	bool _reGetTopeboxInfo;
	bool _isRedirect;
	char _linkRedirect[512];
	char _responseRedirect[512];
	
	void CheckServerTopeboxPromo(int revision, int order);
	void GetTopeboxPromoGameInfo(int revision, int order);
	void GetTopeboxPromoGameInforFromConfig();
	void GenerateSaltString();
	void InitGamePromo(float x, float y, int w, int h, enumTypeTopeboxAds type_ads);
	void RenderGamePromo(float x, float y);
	bool UpdateTouchGamePromo();
	bool GamePromoIsReady();
	bool GamePromoHasTouchDown();
	void ResetGamePromoInfo();
	void DisableGamePromo();
	void GetSubString(std::string str, std::string substr, char c_cut, char*str_output);

	char _game_name[64], _game_id[128], _link_store[128], _link_icon[256], _des[128], _revision[64];
	void SetTopeBoxPromoGameInfo(std::string dataContent);
	void SetTopeBoxPromoGameInforFromConfig();
	void DownloadImageURL();
	void SaveImageToLocal();
	void LoadImageToLocal();
	bool _is_need_reload;
	int _icon_w, _icon_h;

	void GenerateDeviceIDString();
	void SetDeviceID(char* strID);
	void ResponseRequest();

	void GetTopeboxAdsInfoRemoteConfig();
	void GetDataRemoteConfig(char url_download[256]);
	void DowloadRemoteConfigFile();

	bool _is_close_ads;


	char save_file_config[256];
	bool show_banner_topebox_active;
	int count_show_close_banner_topebox;
	bool is_process_show_topebox;
	int count_skip_banner;
	bool render_banner_topebox;
	bool is_process_download;
	
	
private:

	char _salt_str[64];
	char _device_id[128];
	char _ads_id[128];
	PineImage _game_icon;
	bool _is_ready;
	float _renderx, _rendery;
	int _touch_id;
	bool _has_download;

	float _icon_scaleEff, _icon_scaleEff_speed, _icon_scale_size;
	int _icon_scale_time, _icon_scale_state;
	char remoteConfigUrl[256];

	ButtonClose _closeBtn;
	
};


#endif

