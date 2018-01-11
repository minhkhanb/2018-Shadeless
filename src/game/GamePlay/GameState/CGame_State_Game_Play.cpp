#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

void CGameStateGamePlay::InitGameState()
{
	CGameState::InitGameState();

	///////
	_status = CGameStateGamePlay::enumStatusState::STATE_MENU;
	_gamePlay.Init();
	_mainMenu.Init();

	_stateTransition = 1;
	_game->_gameStateLogo.SetTransition();
#if NOTIFICATION_MAX
	Device::AskShowNotification();
#endif
}
void CGameStateGamePlay::Render(float x, float y)
{
	if (_game == NULL || _game->_GameSubState == k_INIT)
	{
		InitGameState();
		
	}
	if (_game->_GameSubState == k_LOOP)
	{

		_gamePlay.Render(x, y);
		_mainMenu.Render(x, y);

		if (_stateTransition == 1)
		{
			_game->_gameStateLogo.Render(x, y);
		}
		if (_game->configDownloaded)
		{
#pragma region TOPEBOX_PROMO_GAME
			//PDEBUG("\n _game->_game_promo_has_show:%d", _game->_user._game_promo_has_show);
			if (_game->_user._game_promo_has_show)
			{
				
				if (_game->disableTopeboxAds)
				{
					float max = -_game->_topebox_game._icon_w*1.5f;
					if (_game->_topebox_game_scrollEff > max)
					{
						_game->_topebox_game_scrollEff += -10;
						if (_game->_topebox_game_scrollEff <= max)
						{
							_game->_topebox_game_scrollEff = max;

						}
					}
				}
				else {
					if (_game->_topebox_game.GamePromoIsReady())
					{
						//PDEBUG("\n _game->_user.GamePromoHasTouchDown: %d", _game->_user._game_promo_has_show);
						if (!_game->_topebox_game.GamePromoHasTouchDown())
						{
							float max = _game->_topebox_game._icon_w*1.3f;
							if (_game->_topebox_game_scrollEff < max)
							{
								_game->_topebox_game_scrollEff += 10.0f;
								if (_game->_topebox_game_scrollEff >= max)
								{
									_game->_topebox_game_scrollEff = max;
								}
							}
						}
					}
				}
				//PDEBUG("\n _game->_user.GamePromoIsReady: %d", _game->_user._game_promo_has_show);
			}
			else
			{
				float max = -_game->_topebox_game._icon_w*1.5f;
				if (_game->_topebox_game_scrollEff == 0)
				{
					_game->_topebox_game_scrollEff = max;
				}
				if (_game->_topebox_game_scrollEff > max)
				{
					_game->_topebox_game_scrollEff += -10;
					if (_game->_topebox_game_scrollEff <= max)
					{
						_game->_topebox_game_scrollEff = max;
						_game->_topebox_game.ResetGamePromoInfo();

					}
				}
				_game->_topebox_game.show_banner_topebox_active = false;
				//PDEBUG("\n _game->_topebox_game.show_banner_topebox_active  => Gameplay render");
			}
			int fw = 165;//GET_SPRITE(SPRITEID_HUD_PAUSE_01)->GetFrameWidth(82);
			char textname[128];
			if (_game->_topebox_game.GamePromoIsReady())
			{
				int lenght = sprintf(textname, "%s", _game->_topebox_game._des);
				float text_w = GET_SPRITE(SPRITEID_FONTS_HIGH_SCORE)->GetAnsiTextWidth(textname, 0, lenght);

				float scalex = text_w / fw + 0.2f;
				//PDEBUG("\n _game->_topebox_game._des:%s -- lenght:%d -- text_w:%f -- scalex:%f", _game->_topebox_game._des, lenght, text_w, scalex);
				_gp->SetColor(0xff000000);
				_gp->SetOpacity(90);
				_gp->SetScale(1.0f, 1.0f, GAME_PROMO_RENDER_X + -_game->_topebox_game_scrollEff + x - 35, GAME_PROMO_RENDER_Y - _game->_topebox_game._icon_h + y - 20);
				//GET_SPRITE(SPRITEID_HUD_PAUSE_01)->DrawFrame(G(), 82, GAME_PROMO_RENDER_X + -_game->_topebox_game_scrollEff + x - 30, GAME_PROMO_RENDER_Y - _game->_topebox_game._icon_h + y - 20);
				if (_game->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_ICON)
				{
					_gp->FillRect(GAME_PROMO_RENDER_X + -_game->_topebox_game_scrollEff + x - 25, GAME_PROMO_RENDER_Y - _game->_topebox_game._icon_h + y + 10, fw, fw, true);
				}
				else if (_game->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER && _game->_topebox_game.render_banner_topebox) {
					//_gp->FillRect(0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT, true);
					_gp->FillFullScreen(true);
				}

				_gp->ClearScale();
				_gp->SetOpacity(100);
			}
			if (_game->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_ICON)
			{
				_game->_topebox_game.RenderGamePromo(GAME_PROMO_RENDER_X + -_game->_topebox_game_scrollEff + x - 20, GAME_PROMO_RENDER_Y - _game->_topebox_game._icon_h + y);
			}
			else if (_game->_topebox_game.typeAds == TopeBox_PromoGame::enumTypeTopeboxAds::TOPEBOX_ADS_BANNER && _game->_topebox_game.render_banner_topebox)
			{
				_game->_topebox_game.RenderGamePromo(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
			


#pragma endregion
		}

	}
}

void CGameStateGamePlay::Update()
{
	if (_game->_GameSubState == k_LOOP)
	{

		if (_game->_canTouch)
		{
			if (!UpdateTouchButton())
			{
				if (_status == CGameStateGamePlay::enumStatusState::STATE_MENU)
				{

					if (_mainMenu._isRenderSetting)
					{
						if (!_mainMenu._setting.UpdateTouchButton())
						{
							if (!_mainMenu._setting._isRenderCredit)
							{
								_mainMenu._setting.UpdateTouch();
							}
						}
					}
					else if (_mainMenu._isRenderGGServices)
					{
						if (!_mainMenu._ggservices.UpdateTouchButton())
						{
							_mainMenu._ggservices.UpdateTouch();
						}
					}
					else
					{
						if (_mainMenu._shareScore._type != CShareScore::enumTypeRender::TYPE_NONE)
						{
							_mainMenu._shareScore.UpdateTouchButton();
						}
						else
						{
							if (!_mainMenu.UpdateTouchButton())
							{
								_mainMenu.UpdateTouch();
							}
						}

					}
				}
				else if (_status == CGameStateGamePlay::enumStatusState::STATE_GAME)
				{
					if (_game->countShowRateUs > 0)
					{
						_game->countShowRateUs--;
						if (_game->countShowRateUs <= 0)
						{
							_game->countShowRateUs = 0;
						}
					}


					//PDEBUG("\n _game->countShowRateUs:%d", _game->countShowRateUs);
					_gamePlay.UpdateTouch();
				}
			}
		}
		_gamePlay.Update();

		if (_stateTransition == 1)
		{
			if (!_game->_gameStateLogo.UpdateTransition())
			{
				_stateTransition = 0;
				_mainMenu._logoY = 0;
				_mainMenu._stateLogo = 4;
				_game->PlaySFX(SFX_WHEEL);
			}
		}
		else
		{

			_mainMenu.Update();
		}
	}
}

bool CGameStateGamePlay::UpdateTouchButton()
{
	return false;
}

void CGameStateGamePlay::UpdateTouch()
{

}

