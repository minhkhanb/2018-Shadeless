#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
extern "C" int getConfigAds();
int _opacity, _stateTransit;
float _posShadelessY, _speed;

void CGameStateLogo::InitGameState()
{
	CGameState::InitGameState();
	_stateTransit = 0;
	_opacity = 100;
	_posShadelessY = 0;

	_antialias = true;
	_trueColor = true;
	if (_game->_device._version != SupportVersion::RESVER_1334_750)
	{
		_antialias = false;
		_trueColor = false;
	}

	///////
	_loadingStep = CGameStateLogo::enumStepLoading::STEP_START;
	_autoLoading = false;
	_game->LoadSpriteCurrentVer(SPRITEID_OBJECT_LOADING, _antialias, _trueColor);
	_game->LoadSpriteCurrentVer(SPRITEID_OBJECT_BG);
	_game->LoadSpriteCurrentVer(SPRITEID_OBJECT_LOGO, _antialias, _trueColor);
	_game->_animLoading.SetAnimSprite(_game->GetSpriteVer(SPRITEID_OBJECT_LOADING), SPRITEID_OBJECT_LOADING);
	_game->_animLoading.SetAnim(0, true);




	//topebox ads
	_game->_topebox_game.InitGamePromo(0, 0, 64, 64, TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER);
	_game->_topebox_game_scrollEff = 0;
	_game->_topebox_game.is_process_download = false;
}
void CGameStateLogo::Render(float x, float y)
{
	if (_game == NULL || _game->_GameSubState == k_INIT)
	{
		InitGameState();
	}
	if (_game->_GameSubState == k_LOOP)
	{
		_gp->SetOpacity(_opacity);
		_gp->SetScale(_game->_game_context.ScaleOutX, _game->_game_context.ScaleOutY, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_game->GetSpriteVer(SPRITEID_OBJECT_BG)->DrawFrame(_gp, 0, 0, 0);
		_gp->ClearScale();


		/*_game->GetSpriteVer(SPRITEID_OBJECT_LOGO)->DrawFrame(_gp, 0, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 + _posShadelessY);
		_gp->SetOpacity(100);
		_game->GetSpriteVer(SPRITEID_OBJECT_LOGO)->DrawFrame(_gp, 1, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 + _posShadelessY);*/
		_gp->SetOpacity(100);
		_game->GetSpriteVer(SPRITEID_OBJECT_LOGO)->DrawFrame(_gp, 0, 0, 0 + _posShadelessY);

	}
}

void CGameStateLogo::Update()
{
	if (_game->_GameSubState == k_LOOP)
	{
		switch (_loadingStep)
		{
		case CGameStateLogo::enumStepLoading::STEP_NONE:
			_loadingStep = CGameStateLogo::enumStepLoading::STEP_START;
			break;

		case CGameStateLogo::enumStepLoading::STEP_START:
			_game->_user.SetGame();
			_game->_user.DataLoad();
			_loadingStep = CGameStateLogo::enumStepLoading::STEP_LOAD_RESOURCE;
			break;

		case  CGameStateLogo::enumStepLoading::STEP_NETWORK_ERROR:
			_loadingStep = CGameStateLogo::enumStepLoading::STEP_LOAD_RESOURCE;
			break;

		case  CGameStateLogo::enumStepLoading::STEP_LOAD_RESOURCE:
			_autoLoading = true;
#ifdef MAC_OS
			_game->_timesShowAdsMod = getConfigAds();
#endif
			break;

		case 4:
			_game->_topebox_game.GetTopeboxAdsInfoRemoteConfig();
			break;

		case 5:
		{
#if defined (ANDROID_OS)
			if (_game->configDownloaded)
			{
				PDEBUG("\n _game->configDownloaded:%d", _game->configDownloaded);
				_autoLoading = true;
			}
			else {
				_autoLoading = false;
				//PDEBUG("\n _game->time_get_remote_config:%d", _game->time_get_remote_config);
			}
#endif
		}
		break;
		case 6:
			
			break;

		case 10:

			break;

		case 11:
			GAME()->InitAds();
			_game->LoadSpriteCurrentVer(SPRITEID_OBJECT_CIRCLE);
			//topebox ads

			break;

		case 12:
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_MAINMENU, _antialias, _trueColor);
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_GOOGLEPLAY, _antialias, _trueColor);
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_TOPEBOX_ADS, _antialias, _trueColor);
			break;

		case 13:
			_game->LoadSpriteCurrentVer(SPRITEID_FONTS_HIGH_SCORE, _antialias, _trueColor);
			_game->GetSpriteVer(SPRITEID_FONTS_HIGH_SCORE)->SetCharSpacing(5);
			break;
		case 14:
			_game->LoadSpriteCurrentVer(SPRITEID_FONTS_BEST_SCORE, _antialias, _trueColor);
			_game->GetSpriteVer(SPRITEID_FONTS_BEST_SCORE)->SetCharSpacing(3);
			_game->LoadSpriteCurrentVer(SPRITEID_FONT_NAME_FACEBOOK, _antialias, _trueColor);
			_game->GetSpriteVer(SPRITEID_FONT_NAME_FACEBOOK)->SetCharSpacing(3);
			break;

		case 15:
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_SETTING, _antialias, _trueColor);
			break;

		case 16:
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_NEWHIGHSCORE, _antialias, _trueColor);
			break;

		case 17:
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_SHARE, _antialias, _trueColor);
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_SHARE_ANDROID, _antialias, _trueColor);
			break;

		case 18:
			_game->LoadSFX(SFX_BUTTON_1);
			break;

		case 19:
			_game->LoadSFX(SFX_BUTTON_2);
			break;

		case 20:
			_game->LoadSFX(SFX_BUTTON_3);
			break;

		case 21:
			_game->LoadSFX(SFX_COLOR_CHANGE);
			break;

		case 22:
			_game->LoadSFX(SFX_INGAME_HIGHSCORE);
			break;

		case 23:
			_game->LoadSFX(SFX_SCORE);
			break;

		case 24:
			_game->LoadSFX(SFX_SHARE);
			break;

		case 25:
			_game->LoadSFX(SFX_SHARE_SCREEN);
			break;

		case 26:
			_game->LoadSFX(SFX_WHEEL);
			break;

		case 27:
			_game->LoadSpriteCurrentVer(SPRITEID_HUD_RATE_US, _antialias, _trueColor);
			break;

		case 28:
			_game->LoadSFX(SFX_SHADELESS);
			break;

		case 29:

#ifdef MAC_OS
			if (!PineSocial::IsDeviceHacked())
			{
				PineSocial::LoadIAP();
			}
			_game->_gcSocial = new PineGameCenterController();
			_game->_gcSocial->Init();
#endif
			break;

		case MAX_LOADING_STEP:
			_game->LogsPlaySession(_game->_user.numPlayGame);
			if (_game->_user._game_promo_delay_show < time(NULL))
			{
				_game->_topebox_game.SetDeviceID(_game->_user._game_promo_deviceID);

			}
			_game->SetGameState(k_GS_GAME_PLAY, false);

			break;
		}

		if (_autoLoading)
		{
			_loadingStep++;
		}
	}
}

bool CGameStateLogo::UpdateTouchButton()
{
	return false;
}

void CGameStateLogo::UpdateTouch()
{

}


bool CGameStateLogo::UpdateTransition()
{
	if (_stateTransit == 0)
	{
		return  false;
	}
	switch (_stateTransit)
	{
	case 1:
		_opacity -= 4;
		_posShadelessY += _speed;
		_speed *= 0.965;
		if (_posShadelessY >= 500)
		{
			_posShadelessY = 500;
		}
		if (_opacity <= 0 && _posShadelessY >= 500)
		{
			_stateTransit = 2;
		}
		break;

	case 2:
		_stateTransit = 0;
		break;
	}
	return 	true;
}
void CGameStateLogo::SetTransition()
{
	_stateTransit = 1;
	_opacity = 100;
	_posShadelessY = 0;
	_speed = 30;
}