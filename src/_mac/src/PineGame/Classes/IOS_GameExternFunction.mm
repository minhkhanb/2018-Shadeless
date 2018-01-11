#import "CGame.h"
#import "PineGameDelegate.h"

extern "C" int GetSizeFile(const char *file_name);
extern "C" int ReadAppDataBinary(const char* filename, BYTE* readed_data);

extern "C" int getConfigAds()
{
    if(Device::IsNetworkAvailable() && PineSocial::PingToHost(ADS_CONFIG_URL,5))
    {
        if (Device::DownloadFile(ADS_CONFIG_URL, ADS_CONFIG_FILE, 5))
        {
            int len =GetSizeFile(ADS_CONFIG_FILE);
            if (len > 0)
            {
                BYTE *infomation = new BYTE[len+1];
                ReadAppDataBinary(ADS_CONFIG_FILE,infomation);
                infomation[len]='\0';
                
                char version[128];
                sprintf(version,"%d",0);
                for (int i  = 0; i < len; i ++)
                {
                    version[i] = infomation[i];
                }
                version[len]='\0';
                delete[] infomation;
                return atoi(version);
            }
        }
    }
    return 3;
}