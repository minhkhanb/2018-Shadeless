#include "pch.h"
#include "../GameCore/CGame.h"
#if defined(_WINDOWS)
extern "C" void OS_AdBoxDownloadFile() {}
extern "C" void ShowAlertMessage_OKCancel(const char* feed, const char* title, const char* tag){}
extern "C" void ShowAlertMessage_OK(const char* feed, const char* title, const char* tag){}
extern "C" void OpenAppStore(INT64 appleID)
{}
#endif

#if defined(_WINDOWS_PHONE8)
extern "C" void OS_AdBoxDownloadFile() {}
#endif