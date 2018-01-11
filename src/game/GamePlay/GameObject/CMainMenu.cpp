#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

void CMainMenu::Init()
{
	_game = GAME();
	_gp = G();
	_isNewScore = false;
	_shareScore._type = CShareScore::enumTypeRender::TYPE_NONE;
	_touchID = -1;
	_touchIDOld = -1;

	_stateLogo = 0;
	_stateHighScore = 0;
	_stateBestScore = 0;
	InitResult_Logo(0, true);

	_scrollBarX.Init(-80, 104, 1, 0.1, 1.1);
	_scrollBarX.SetCurrent(0);
	_offsetAngle = 0;
	_currentItems = GetCurrentItems();
	_isTouchDrag = false;
	_stateMoveTarget = 0;

	_offsetDragX = 0;
	_timeMove = 0;
	_opacityItems = 0;

	_isRenderSetting = false;
	_stateSeting = 0;

	_isRenderGGServices = false;
	_stateGGServices = 0;
}
void CMainMenu::GoToPlay(int state)
{

	_stateGoPlay = state;
	if (_stateGoPlay == 1)
	{
		_stateRotaItem = 1;
		_offRotaItem = 0;
		_speedRotaItem = 5;
		_opacityItems = 0;
		if (_game->_gameStateGamePlay._stateTransition == 0)
		{
			_game->PlaySFX(SFX_WHEEL);
		}
	}
	else if (_stateGoPlay == 10)
	{
		_game->StopCurrentMusic();
		_game->PlaySFX(SFX_WHEEL);
		_stateRotaItem = 1;
		_stateLogo = 10;
		_stateHighScore = 10;
		_stateBestScore = 10;
		_game->_user.numPlayGame++;
		_game->_numShowAds++;
		if (_game->_advert.IsAvailable())
		{
			_game->_showPineAds++;
		}
		if (!_game->_user._hadRate)
		{
			_game->_user._numPlayRate++;
		}
		_isNewScore = false;
		_game->_gameStateGamePlay._gamePlay.InitScoreTime();
		_game->_user.DataSave();
	}
	_game->_canTouch = false;
}
void CMainMenu::UpdateGoPlay()
{
	if (_stateGoPlay != 0)
	{
		if (_stateGoPlay == 1)
		{
			_opacityItems += 3;
			if (_opacityItems >= 100)
			{
				_opacityItems = 100;
			}
			switch (_stateRotaItem)
			{
			case 1:
				_offRotaItem += _speedRotaItem;
				_speedRotaItem *= 1.1;
				if (_speedRotaItem >= 20)
				{
					_stateRotaItem = 2;
					_timeRoteItems = 30;
				}
				break;

			case 2:
				_offRotaItem += _speedRotaItem;
				_speedRotaItem *= 0.9;
				if (_speedRotaItem <= 1)
				{
					_stateRotaItem = 0;
					_stateGoPlay = 0;
					_game->_canTouch = true;
					_offRotaItem = 0;
				}
				break;
			}
			_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
			_menuItems[_currentItems]._opacity = _opacityItems;
			//PDEBUG("\n Update goplay state 0 _opacityItems: %d", _opacityItems);
		}

		if (_stateGoPlay == 10)
		{
			bool res = true;
			_opacityItems -= 2;
			if (_opacityItems <= 0)
			{
				_opacityItems = 0;
			}
			else
			{
				res = false;
			}

			if (res && _stateLogo == 0 && _stateBestScore == 0 & _stateHighScore == 0)
			{
				_stateGoPlay = 0;
				_game->_gameStateGamePlay._status = CGameStateGamePlay::enumStatusState::STATE_GAME;
				_game->_gameStateGamePlay._gamePlay.SetState(CGamePlay::enumStateGame::GAME_DOWN_RADIUS);
				if (_game->_user._stepTutorial < 100)
				{
					_game->_gameStateGamePlay._gamePlay._beginGame = false;
					_game->_gameStateGamePlay._gamePlay.SetTutorial(0);
				}
				else
				{
					_game->_gameStateGamePlay._gamePlay._beginGame = true;
				}
				_game->_canTouch = true;
			}
			switch (_stateRotaItem)
			{
			case 1:
				_offRotaItem += _speedRotaItem;
				_speedRotaItem *= 1.1;
				if (_speedRotaItem >= 20)
				{
					_stateRotaItem = 2;
					_timeRoteItems = 30;
				}
				break;

			case 2:
				_offRotaItem += _speedRotaItem;
				_speedRotaItem *= 0.9;
				if (_speedRotaItem <= 1)
				{
					_stateRotaItem = 0;
					_offRotaItem = 0;
				}
				break;
			}
			_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
			_menuItems[_currentItems]._opacity = _opacityItems;
			//PDEBUG("\n Update goplay state 10 _opacityItems: %d", _opacityItems);
		}
	}
}
void CMainMenu::Update()
{
	_scrollBarX.Update();
	_offsetAngle = -_scrollBarX.GetCurrent();
	//PDEBUG("\n _offsetAngle:%f", _offsetAngle);
	_currentItems = GetCurrentItems();
	for (int i = 0; i < MENU_ITEMS_NUMS; i++)
	{
		if (i == _currentItems)
		{
			if (_menuItems[i]._button._state == BUTTON_STATE_HIDE)
			{
				_menuItems[i]._button.SetState(BUTTON_STATE_NORMAL);
			}
		}
		else
		{
			_menuItems[i]._button.SetState(BUTTON_STATE_HIDE);
		}
	}
	UpdateResult_Logo();
	UpdateGoPlay();
	UpdateItems();
	UpdateStateSetting();
#if defined (ANDROID_OS) || _WINDOWS
	UpdateStateGGServices();
#endif
	if (_isRenderSetting)
	{
		_setting.Update();
	}
	else
	{
		if (_shareScore._type != CShareScore::enumTypeRender::TYPE_NONE)
		{
			_shareScore.UpdateHighScore();
		}
	}
#if defined (ANDROID_OS) || _WINDOWS
	if (_isRenderGGServices)
	{
		_ggservices.Update();
	}
#endif
	//PDEBUG("\n _game->_user._hadRate:%d", _game->_user._hadRate);
	if (!_game->_user._hadRate)
	{
		//PDEBUG("\n _shareScore.stateRateGame:%d -- _shareScore.isCompleteRate:%d", _shareScore.stateRateGame, _shareScore.isCompleteRate);

		if (_shareScore.stateRateGame >= 5 && _shareScore.isCompleteRate)
		{
			_shareScore.stateRateGame = CShareScore::enumStarRateStatus::RATE_STATE_NONE;
			PDEBUG("\n OS_Rating");
#if defined ANDROID_OS
			OS_Rating();
#endif
			_game->_user._hadRate = true;
			_game->_user.DataSave();
		}
		else if (_shareScore.stateRateGame > 0 && _shareScore.isCompleteRate)
		{
			_shareScore.Init(0, CShareScore::enumTypeRender::CONFIRM_RATE_US);
			PDEBUG("\n render confirm rating");
			_shareScore.stateRateGame = CShareScore::enumStarRateStatus::RATE_STATE_NONE;
			_game->_user._hadRate = true;
			_game->_user.DataSave();
		}
	}
}

void CMainMenu::Render(float x, float y)
{

	RenderResult_Logo(x, y);
	if (_isRenderSetting)
	{
		_setting.Render(x, y);
	}
	else
	{
		if (_shareScore._type != CShareScore::enumTypeRender::TYPE_NONE)
		{
			_shareScore.RenderHighScore(x, y);
		}
		else
		{
			RenderItems(x, y);
		}
	}
	_shareScore.RenderShare(x, y);
#if defined (ANDROID_OS) || _WINDOWS
	if (_isRenderGGServices)
	{
		_ggservices.Render(x, y);
	}
#endif

}
void CMainMenu::RenderItems(float x, float y)
{
	for (int i = 0; i < MENU_ITEMS_NUMS; i++)
	{
#if defined (ANDROID_OS) || _WINDOWS
		if (i == CMainMenu::enumMenuItems::ITEM_ACHIEVEMENT || i == CMainMenu::enumMenuItems::ITEM_LEADERBOARD)
		{
			continue;
		}
#endif
		_menuItems[i]._x = k_SCREEN_WIDTH / 2 + 300 * CMath::COS(_menuItems[i]._angle + _offsetAngle + _offRotaItem) + x;
		_menuItems[i]._y = k_SCREEN_HEIGHT / 2 + 300 * CMath::SIN(_menuItems[i]._angle + _offsetAngle + _offRotaItem) + y;

		_gp->SetRotate(_menuItems[i]._angle + _offsetAngle + _offRotaItem - 90, _menuItems[i]._x, _menuItems[i]._y);
		_gp->SetOpacity(_opacityItems);
		if (i == _currentItems)
		{
#if defined (ANDROID_OS) || _WINDOWS
			if (i == CMainMenu::enumMenuItems::ITEM_REMOVEADS)
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _menuItems[i]._frame + 1, _menuItems[i]._x, _menuItems[i]._y);
			}
			else
#endif
			{
				_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, _menuItems[i]._frame + 1, _menuItems[i]._x, _menuItems[i]._y);
			}
		}
		else
		{
#if defined (ANDROID_OS) || _WINDOWS
			if (i == CMainMenu::enumMenuItems::ITEM_REMOVEADS)
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _menuItems[i]._frame, _menuItems[i]._x, _menuItems[i]._y);
			}
			else
#endif
			{
				_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, _menuItems[i]._frame, _menuItems[i]._x, _menuItems[i]._y);
			}
		}

		_gp->ClearRotate();
		_gp->SetOpacity(_menuItems[i]._opacity);

		if (i == CMainMenu::enumMenuItems::ITEM_REMOVEADS
			&& !_menuItems[i]._button.CheckState(BUTTON_STATE_HIDE)
			&& !_menuItems[i]._button.CheckState(BUTTON_STATE_LOCK))
		{
			if (!_game->_user._isUnlockColor)
			{
				_gp->SetRotate(_menuItems[i]._rotaIcon, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 41, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2 + POS_Y(60));
			}
			else {
				_menuItems[CMainMenu::enumMenuItems::ITEM_REMOVEADS]._button.SetState(BUTTON_STATE_LOCK);
			}
		}

		_gp->SetRotate(_menuItems[i]._rotaIcon, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_menuItems[i]._button.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_menuItems[i]._button.Render(_gp);
		_menuItems[i]._button.Update();
		_gp->ClearRotate();
		_gp->SetOpacity(100);

		/*	int px = k_SCREEN_WIDTH / 2 + 300 * CMath::COS(_menuItems[i]._angleCheck + _offsetAngle + _offRotaItem) + x;
			int py = k_SCREEN_HEIGHT / 2 + 300 * CMath::SIN(_menuItems[i]._angleCheck + _offsetAngle + _offRotaItem) + y;
			_gp->SetColor(0xffff0000);
			_gp->FillCircle(px, py, 10, 10,true);*/


	}

}
void CMainMenu::UpdateItems()
{
	if (_game->_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_MENU)
	{

		bool updateIcon = false;
		if (_timeMove > 0)
		{
			_timeMove--;
			_scrollBarX.SetCurrent(_scrollBarX.GetCurrent() + _speedMove);
		}

		//PDEBUG("\n Update items _stateGoPlay: %d --_isRenderSetting:%d --_stateSeting:%d -- _isRenderGGServices:%d -- _stateGGServices:%d", _stateGoPlay, _isRenderSetting, _stateSeting, _isRenderGGServices, _stateGGServices);
		if (_stateGoPlay == 0 && !_isRenderSetting && _stateSeting == 0 && !_isRenderGGServices && _stateGGServices == 0)
		{
			for (int i = 0; i < MENU_ITEMS_NUMS; i++)
			{
				if (i == _currentItems)
				{

					float offAg = CMath::ABSOLUTE_VALUE((_menuItems[i]._angle + _offsetAngle) - 90);
					if (i == CMainMenu::enumMenuItems::ITEM_SETTING/* || i == CMainMenu::enumMenuItems::ITEM_GGSERVICES*/)
					{
						float range = CMath::ABSOLUTE_VALUE(_menuItems[i]._angle - _menuItems[i]._angleCheck);
						_menuItems[i]._opacity = 100 * (1 - offAg / range);
						//PDEBUG("\n Update items 11111111111111 ===>  _offsetAngle: %f -- _menuItems[i]._angle:%f -- _menuItems[i]._angleCheck:%f",  _offsetAngle, _menuItems[i]._angle, _menuItems[i]._angleCheck);
					}
#if defined (ANDROID_OS) || _WINDOWS
					else if (i == CMainMenu::enumMenuItems::ITEM_GGSERVICES)
					{
						float range = CMath::ABSOLUTE_VALUE(_menuItems[i]._angle - _menuItems[i]._angleCheck);
						_menuItems[i]._opacity = 100 * (1 - offAg / range);
						//PDEBUG("\n Update items 2222222222222222 ===>  _offsetAngle: %f -- _menuItems[i]._angle:%f -- _menuItems[i]._angleCheck:%f", _offsetAngle, _menuItems[i]._angle, _menuItems[i]._angleCheck);
					}
#endif
					else if (i == CMainMenu::enumMenuItems::ITEM_ACHIEVEMENT)
					{
						float range = CMath::ABSOLUTE_VALUE(_menuItems[i]._angle - _menuItems[i + 1]._angleCheck);
						_menuItems[i]._opacity = 100 * (1 - offAg / range);
						//PDEBUG("\n Update items 222222222222222 ===>  _menuItems[2]._opacity: %d", _menuItems[i]._opacity);
					}
					else
					{
						float range = CMath::ABSOLUTE_VALUE(_menuItems[i]._angle - _menuItems[i + 1]._angleCheck);
						if (_menuItems[i]._angle + _offsetAngle < 90)
						{
							range = CMath::ABSOLUTE_VALUE(_menuItems[i]._angle - _menuItems[i]._angleCheck);
						}
						_menuItems[i]._opacity = 100 * (1 - offAg / range);
						//PDEBUG("\n Update items 33333333333333 ===>  _menuItems[3]._opacity: %d", _menuItems[i]._opacity);
					}

					float aaa = (_menuItems[i]._angle + _offsetAngle) - 90;
					float bbb = (_menuItems[i]._angle - _menuItems[i]._angleCheck);

					_menuItems[i]._rotaIcon = 360 * (bbb - aaa) / bbb;
					//PDEBUG("\n _menuItems[%d]._rotaIcon: %f",i,  _menuItems[i]._rotaIcon);
					//_menuItems[i]._rotaIcon = 360 * ((_menuItems[i]._angle + _offsetAngle) - 90) / (float)(_menuItems[i]._angle - _menuItems[i]._angleCheck);
				}

			}
		}



		if (!_isTouchDrag)
		{
			if (_stateMoveTarget == 1 && !_scrollBarX.IsScrolling())
			{
				_stateMoveTarget = 2;
			}
			else if (_stateMoveTarget == 2)
			{
				_targetItems = _currentItems;
				_timeMove = 20;
				_speedMove = ((_menuItems[_targetItems]._angle + _offsetAngle) - 90) / (float)_timeMove;
				_stateMoveTarget = 0;
			}

			if (_stateMoveTarget == 10 && !_scrollBarX.IsScrolling())
			{
				_stateMoveTarget = 11;
			}
			else if (_stateMoveTarget == 11)
			{
				bool hasForce = false;
				if (_moveDirection == _RIGHT)
				{
					if (_currentItems > 0)
					{
						_targetItems = _currentItems - 1;
						hasForce = true;
					}
				}
				else if (_moveDirection == _LEFT)
				{
					if (_currentItems < MENU_ITEMS_NUMS - 1)
					{
						_targetItems = _currentItems + 1;
						hasForce = true;
					}
				}
				if (hasForce)
				{
					_timeMove = 20;
					_speedMove = ((_menuItems[_targetItems]._angle + _offsetAngle) - 90) / (float)_timeMove;
				}
				_stateMoveTarget = 0;
			}
		}
	}
}
int CMainMenu::GetCurrentItems()
{
	for (int i = 0; i < MENU_ITEMS_NUMS; i++)
	{
		if (i == CMainMenu::enumMenuItems::ITEM_ACHIEVEMENT)
		{
			if (_menuItems[i + 1]._angleCheck + _offsetAngle <= 90)
			{
				return i;
			}
		}
		else
		{
			if (i == CMainMenu::enumMenuItems::ITEM_SETTING)
			{
				if (_menuItems[i]._angleCheck + _offsetAngle >= 90)
				{
					return i;
				}
			}
			else
			{
				if (_menuItems[i + 1]._angleCheck + _offsetAngle <= 90)
				{
					return i;
				}
			}
		}
	}
	return -1;
}
void CMainMenu::InitMenu()
{
	_isNewScore = false;
	_game->_canTouch = false;
	int beginAngle = 10;
	_opacityItems = 100;
	for (int i = MENU_ITEMS_NUMS - 1; i >= 0; i--)
	{
		_menuItems[i]._angle = beginAngle;
		_menuItems[i]._angleCheck = beginAngle;
		_menuItems[i]._button.Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER);

		switch (i)
		{
		case CMainMenu::enumMenuItems::ITEM_ACHIEVEMENT:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 0);
			beginAngle += 30;
			_menuItems[i]._frame = 15;
			_menuItems[i]._angleCheck += 30 / 2;
			break;

		case CMainMenu::enumMenuItems::ITEM_LEADERBOARD:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 4);
			beginAngle += 56;
			_menuItems[i]._frame = 19;
			_menuItems[i]._angleCheck += 56 / 2;
			break;

		case CMainMenu::enumMenuItems::ITEM_PLAY:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 8);
			beginAngle += 48;
			_menuItems[i]._frame = 3;
			_menuItems[i]._angleCheck += 20;
			break;

		case CMainMenu::enumMenuItems::ITEM_SHARE:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 12);
			beginAngle += 37;
			_menuItems[i]._frame = 7;
			_menuItems[i]._angleCheck += 37 / 2;
			break;

		case CMainMenu::enumMenuItems::ITEM_SETTING:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 16);
			beginAngle += 43;
			_menuItems[i]._frame = 11;
			_menuItems[i]._angleCheck += 43 / 2;
			break;
#if defined (ANDROID_OS) || _WINDOWS
		case CMainMenu::enumMenuItems::ITEM_GGSERVICES:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_MAINMENU, 20);
			beginAngle += 43;
			_menuItems[i]._frame = 30;
			_menuItems[i]._angleCheck += 43 / 2;
			break;


		case CMainMenu::enumMenuItems::ITEM_REMOVEADS:
			_menuItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 24);
			_menuItems[i]._frame = 27;
			beginAngle += 56;
			_menuItems[i]._angleCheck += 56 / 2;
			break;
#endif
		}
	}
	GoToPlay(1);

	_scrollBarX.Init(-80, 104, 1, 0.1, 1.1);
	_scrollBarX.SetCurrent(0);
	_offsetAngle = 0;
	_currentItems = GetCurrentItems();

	_game->disableTopeboxAds = true;


}
void CMainMenu::InitResult_Logo(int Score, bool isStart)
{
	_game->StopCurrentMusic();
	_game->LoadMusic(BGM_MAINMENU);
	_game->PlayCurrentMusic(-1);



	if (!isStart)
	{
		if (Score > 1)
		{
			if (Score == _game->_oldScore)
			{
				_game->_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_FamiliarFace);
			}
			_game->_oldScore = Score;
			_game->_numGameArow++;
		}
	}
	if (_game->_user._highScore < Score)
	{
		_game->_user._highScore = Score;
		_isNewScore = true;

		_game->_user._scoreColor[_game->_user._curColorUse] = _game->_user._highScore;

	}

	_stateLogo = 1;
	_logoY = 200;

	_stateHighScore = 1;
	_highScore = Score;
	_highScoreY = -170;

	_stateBestScore = 1;
	_bestScore = _game->_user._highScore;
	_bestScoreY = -250;
	_game->LogsScoreSession(Score);
	if (_isNewScore)
	{
#ifdef MAC_OS	
		PineGameCenterController::SubmitMyScore("Shadeless_Leaderboard", _game->_user._highScore);
#elif defined (ANDROID_OS)
		_game->SubmitHighScore();
		PDEBUG("\n Submit highScore");
#endif
		_shareScore.Init(_game->_user._highScore, CShareScore::enumTypeRender::HIGHT_SCORE);
	}
	else
	{
		//_shareScore.Init(0, CShareScore::enumTypeRender::RATE_US);
		//PDEBUG("\n _game->_numGameArow:%d", _game->_numGameArow);
		if (!_game->_user._hadRate && _game->_numGameArow >= 15 /*&& !_game->_user._removeAds && GAME()->countShowRateUs <= 0*/)
		{
			_game->_user._numPlayRate = -1;
			_shareScore.Init(0, CShareScore::enumTypeRender::RATE_US);
			GAME()->countShowRateUs = TIME_SHOW_RATE_US;
		}
		else
		{
			InitMenu();
		}
	}

	printf("\n _game->_numShowAds = %d, _game->_timesShowAdsMod = %d", _game->_numShowAds, _game->_timesShowAdsMod);
	if (_game->_showPineAds == 1)
	{
		_game->_stateShowPineAds = 1;
	}
	else if (_game->_numShowAds >= _game->_timesShowAdsMod && !_game->_user._removeAds)
	{
#ifdef MAC_OS
		if (_game->_timesShowAdsVideo >= 4)
		{
			_game->_hasAction = true;
			int adboxtype = PineAdBox::HasVideoAds();
			if (adboxtype >= 0)
			{
				PineAdBox::ShowVideoAds(adboxtype);
				_game->_timesShowAdsVideo = 0;
			}
			else
			{
				PineAdBox::ShowAdvertFullScreen();
			}
		}
		else
		{
			_game->_hasAction = true;
			PineAdBox::ShowAdvertFullScreen();
		}
		_game->_numShowAds = 0;
		_game->_timesShowAdsVideo++;
#endif
	}

	_game->_achievement.LoadInfoAchievement();
	_game->_achievement.CheckAchievementComplete();
	_game->_user.DataSave();
}
void CMainMenu::RenderResult_Logo(float x, float y)
{
	if (_stateLogo != 0)
	{
		_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 0, x, _logoY + y);

		_gp->SetColorMask(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_TEXT_COLOR));
		_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 23, x, _logoY + y);
		_gp->ClearColorMask();

		_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 26, x, _logoY + y);
	}

	if (_stateHighScore != 0)
	{
		_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 1, x, _highScoreY + y);
		_gp->SetColorMask(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_TEXT_COLOR));
		_game->GetSpriteVer(SPRITEID_FONTS_HIGH_SCORE)->DrawAnsiTextFormat(_gp, k_SCREEN_WIDTH / 2 + x, _highScoreY + POS_Y(115) + y, _HCENTER | _VCENTER, "%d", _highScore);
		_gp->ClearColorMask();
	}

	if (_stateBestScore != 0)
	{
		_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 2, x, _bestScoreY + y);

		_game->GetSpriteVer(SPRITEID_FONTS_BEST_SCORE)->DrawAnsiTextFormat(_gp, k_SCREEN_WIDTH / 2 + x, _bestScoreY + POS_Y(240) + y, _HCENTER | _VCENTER, "%d", _bestScore);
	}
}
void CMainMenu::UpdateResult_Logo()
{
	//logo
	switch (_stateLogo)
	{
	case 1:
		_logoY -= 20;
		if (_logoY <= -10)
		{
			_logoY = -10;
			_stateLogo = 2;
		}
		break;

	case 2:
		_logoY += 3;
		if (_logoY >= 5)
		{
			_logoY = 5;
			_stateLogo = 3;
		}
		break;

	case 3:
		_logoY -= 2;
		if (_logoY <= 0)
		{
			_logoY = 0;
			_stateLogo = 4;
		}
		break;

	case 10:
		_logoY += 20;
		if (_logoY >= 200)
		{
			_logoY = 200;
			_stateLogo = 0;
		}
		break;
	}

	//best score
	switch (_stateBestScore)
	{
	case 1:
		_bestScoreY += 20;
		if (_bestScoreY >= 10)
		{
			_bestScoreY = 10;
			_stateBestScore = 2;
		}
		break;

	case 2:
		_bestScoreY -= 3;
		if (_bestScoreY <= -5)
		{
			_bestScoreY = -5;
			_stateBestScore = 3;
		}
		break;

	case 3:
		_bestScoreY += 2;
		if (_bestScoreY >= 0)
		{
			_bestScoreY = 0;
			_stateBestScore = 4;
		}
		break;

	case 10:
		_bestScoreY -= 20;
		if (_bestScoreY <= -250)
		{
			_bestScoreY = 250;
			_stateBestScore = 0;
		}
		break;
	}

	//best score
	switch (_stateHighScore)
	{
	case 1:
		_highScoreY += 10;
		if (_highScoreY >= 10)
		{
			_highScoreY = 10;
			_stateHighScore = 2;
		}
		break;

	case 2:
		_highScoreY -= 3;
		if (_highScoreY <= -5)
		{
			_highScoreY = -5;
			_stateHighScore = 3;
		}
		break;

	case 3:
		_highScoreY += 2;
		if (_highScoreY >= 0)
		{
			_highScoreY = 0;
			_stateHighScore = 4;
		}
		break;

	case 10:
		_highScoreY -= 20;
		if (_highScoreY <= -170)
		{
			_highScoreY = 170;
			_stateHighScore = 0;
		}
		break;
	}
}
bool CMainMenu::UpdateTouchButton()
{
	bool ret = false;
	int index = -1;
	for (int i = 0; i < MENU_ITEMS_NUMS; i++)
	{
		if (_menuItems[i]._button._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_menuItems[index]._button.NeedAction())
		{
			switch (index)
			{
			case CMainMenu::enumMenuItems::ITEM_ACHIEVEMENT:
				_game->PlaySoundButton();
#ifdef MAC_OS
				PineGameCenterController::ShowLeaderAchivement();
#endif
				break;

			case CMainMenu::enumMenuItems::ITEM_LEADERBOARD:
				_game->PlaySoundButton();
#ifdef MAC_OS
				PineGameCenterController::SubmitMyScore("Shadeless_Leaderboard", _game->_user._highScore);
				PineGameCenterController::ShowLeaderBoard("Shadeless_Leaderboard");
				//PineGameCenterController::ResetAllAchivement();
#endif
				break;
#if defined (ANDROID_OS)
			case CMainMenu::enumMenuItems::ITEM_GGSERVICES:
				PDEBUG("\n#######GAME()->_async_task_manager._is_login_google: %d", GAME()->_async_task_manager._is_login_google);
				if (!GAME()->_async_task_manager._is_login_google)
				{
					GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
					GAME()->_async_task_manager._is_force_login_google_play = true;
					GAME()->_async_task_manager._on_process_sync_cloud = true;
					GAME()->_async_task_manager._is_show_game_services_menu = true;
					//_game->_user.CheckLocalAndCloudSaveFile(false);
				}
				else {
					OpenGGServices();
				}
				_game->PlaySoundButton();
				break;
#elif _WINDOWS
			case CMainMenu::enumMenuItems::ITEM_GGSERVICES:
				OpenGGServices();
				break;
#endif

			case CMainMenu::enumMenuItems::ITEM_PLAY:
				_game->PlaySoundButton();
				_game->disableTopeboxAds = true;

				GoToPlay(10);
#if defined (ANDROID_OS)
				if (!GAME()->_async_task_manager._is_login_google)
				{
					GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_IS_LOG_IN_GOOGLE_PLAY);
					GAME()->_async_task_manager._is_force_login_google_play = true;
				}
#endif
				break;

			case CMainMenu::enumMenuItems::ITEM_SHARE:
				_game->show_ads_interrupt = false;
				_shareScore.AddShare();
				_game->PlaySoundButton();
				
				break;

			case CMainMenu::enumMenuItems::ITEM_SETTING:
				_game->PlaySoundButton();
				OpenSetting();
				break;

			case CMainMenu::enumMenuItems::ITEM_REMOVEADS:
#if _WINDOWS
				_game->_gameStateGamePlay._mainMenu._menuItems[CMainMenu::enumMenuItems::ITEM_REMOVEADS]._button.SetState(BUTTON_STATE_LOCK);
				_game->_user._removeAds = true;
				_game->_user._isUnlockColor = true;
				_game->_user.DataSave();
#endif
				_game->PlaySoundButton();
				_game->_hasAction = true;
				if (Device::IsNetworkAvailable())
				{
#if defined (MAC_OS)
					_game->_isAdsProcessing = true;
					PurchaseProduct("Shadeless_NoAds");
#elif defined (ANDROID_OS)
					_game->_isAdsProcessing = true;
					PurchaseProduct("shadeless_noads_1_99");
					PDEBUG("\n _user._removeAds:%d", _game->_user._removeAds);
#endif
				}
				else
				{
					_game->_user.NetWorkError();
				}
				break;

			}
			_menuItems[index]._button.SetState(BUTTON_STATE_NORMAL, -1);

			for (int i = 0; i < MENU_ITEMS_NUMS; i++)
			{
				if (_menuItems[i]._button._state != BUTTON_STATE_LOCK && _menuItems[i]._button._state != BUTTON_STATE_HIDE)
				{
					_menuItems[i]._button.SetState(BUTTON_STATE_NORMAL, -1);
				}
			}
			_game->ClearTouchProcessing();
		}
		return true;
	}
	bool pressed = false;
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			for (int touch = 0; touch < MENU_ITEMS_NUMS; touch++)
			{
				if (_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					continue;
				}
				if (_menuItems[touch]._button._state == BUTTON_STATE_LOCK || _menuItems[touch]._button._state == BUTTON_STATE_HIDE)
				{
					continue;
				}

				int x = _menuItems[touch]._button.TZ_GetX() - 20;
				int y = _menuItems[touch]._button.TZ_GetY() - 20;
				int w = _menuItems[touch]._button.TZ_GetWidth() + 40;
				int h = _menuItems[touch]._button.TZ_GetHeight() + 40;
				Button *button = &_menuItems[touch]._button;
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (!button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id) && Button::GetButtonActive() == NULL)
						{
							button->SetState(BUTTON_STATE_SELECT, TOUCHS(i)->_id, TOUCH_DOWN);
							_game->SetTouchProcessing(TOUCHS(i)->_id);
							pressed = true;
						}
						else if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							pressed = true;
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							pressed = true;
						}
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							button->SetState(BUTTON_STATE_ACTIVE);
						}
					}
				}
				else
				{
					if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
					{
						for (int j = 0; j < MENU_ITEMS_NUMS; j++)
						{
							if (_menuItems[j]._button._state != BUTTON_STATE_LOCK && _menuItems[j]._button._state != BUTTON_STATE_HIDE)
							{
								_menuItems[j]._button.SetState(BUTTON_STATE_NORMAL, -1);
							}
						}
						_game->ClearTouchProcessing();
					}
				}
			}
		}
	}
	return pressed;
}

void CMainMenu::UpdateTouch()
{
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (!_scrollBarX.CheckTouch(TOUCHS(i)->_id) && !_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					_offsetDragX = 0;
					_touchDragX = TOUCHS(i)->x;
					_scrollBarX.SetBeginScorll(TOUCHS(i)->_id);
					_scrollBarX.SetState(0);
					_game->SetTouchProcessing(TOUCHS(i)->_id);
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				if (_scrollBarX.CheckTouch(TOUCHS(i)->_id))
				{
					_offsetDragX = TOUCHS(i)->x - _touchDragX;
					_touchDragX = TOUCHS(i)->x;
					if (_offsetDragX > 20)
					{
						_offsetDragX = 20;
					}
					if (_offsetDragX < -20)
					{
						_offsetDragX = -20;
					}
					_scrollBarX.AddOffset(_offsetDragX*0.25);
					_scrollBarX.ValidateRangeExt();
					_scrollBarX.ForceToTarget();
					_scrollBarX.SetState(1);

					if (_offsetDragX > 1)
					{
						_isTouchDrag = true;
						_moveDirection = _RIGHT;
					}
					if (_offsetDragX < -1)
					{
						_isTouchDrag = true;
						_moveDirection = _LEFT;
					}
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_scrollBarX.CheckTouch(TOUCHS(i)->_id))
				{
					if (!_scrollBarX._collided)
					{
						_scrollBarX.End();
					}
					_scrollBarX.SetState(2);
					if (CMath::ABSOLUTE_VALUE(_offsetDragX) <= 1)
					{
						_targetItems = GetIndexTouch(TOUCHS(i)->x, TOUCHS(i)->y);
						if (_targetItems != -1 && _targetItems != _currentItems)
						{

							_timeMove = 30;
							_speedMove = ((_menuItems[_targetItems]._angle + _offsetAngle) - 90) / (float)_timeMove;
						}
						else
						{
							_stateMoveTarget = 1;
						}
					}
					else
					{
						if (CMath::ABSOLUTE_VALUE(_offsetDragX) > 3)
						{
							_stateMoveTarget = 10;
							_scrollBarX.SetCurrent(_scrollBarX.GetCurrent());
						}
						else
						{
							_stateMoveTarget = 1;
						}
					}
					_scrollBarX.ClearTouchScroll();
					_game->ClearTouchProcessing();
					_game->PlaySoundButton();
					_isTouchDrag = false;
				}
			}
		}
	}
}
int  CMainMenu::GetIndexTouch(float x, float y)
{
	for (int i = 0; i < MENU_ITEMS_NUMS; i++)
	{
		if (CHECK_POINT_IN_CIRCLE(x, y, _menuItems[i]._x, _menuItems[i]._y, 80))
		{
			return i;
		}
	}
	return -1;
}
void CMainMenu::UpdateStateGGServices() {
	if (_stateGGServices == 1)
	{
		//PDEBUG("\n Update state GGServices 1 _speedRotaItem: %f", _speedRotaItem);
		_opacityItems -= 2;
		if (_opacityItems <= 0)
		{
			_stateGGServices = 0;
			_opacityItems = 0;
			_isRenderGGServices = true;
			_ggservices.Init();
		}

		switch (_stateRotaItem)
		{
		case 1:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 40;
			}
			break;

		case 2:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 1)
			{
				_stateRotaItem = 0;
				_offRotaItem = 0;
			}
			break;
		}
		_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_menuItems[_currentItems]._opacity = _opacityItems;
		//PDEBUG("\n Update state GGServices 1 _opacityItems: %d", _opacityItems);
	}
	else if (_stateGGServices == 2)
	{
		//PDEBUG("\n Update state GGServices 2 _speedRotaItem: %f", _speedRotaItem);
		_opacityItems += 2;
		if (_opacityItems >= 100)
		{
			_opacityItems = 100;
		}
		switch (_stateRotaItem)
		{
		case 1:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 40;
			}
			break;

		case 2:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 1)
			{
				_stateRotaItem = 0;
				_stateGGServices = 0;
				_game->_canTouch = true;
				_offRotaItem = 0;
			}
			break;
		}
		_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_menuItems[_currentItems]._opacity = _opacityItems;
		//PDEBUG("\n Update state GGServices 2 _offRotaItem: %d", _offRotaItem);
	}
}
void CMainMenu::UpdateStateSetting()
{

	if (_stateSeting == 1)
	{
		_opacityItems -= 2;
		if (_opacityItems <= 0)
		{
			_stateSeting = 0;
			_opacityItems = 0;
			_isRenderSetting = true;
			_setting.Init();
		}

		switch (_stateRotaItem)
		{
		case 1:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 1)
			{
				_stateRotaItem = 0;
				_offRotaItem = 0;
			}
			break;
		}
		_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_menuItems[_currentItems]._opacity = _opacityItems;
		//PDEBUG("\n if Update state Setting 1 _offRotaItem: %f", _offRotaItem);
	}
	else if (_stateSeting == 2)
	{
		_opacityItems += 3;
		if (_opacityItems >= 100)
		{
			_opacityItems = 100;
		}
		switch (_stateRotaItem)
		{
		case 1:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_offRotaItem += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 1)
			{
				_stateRotaItem = 0;
				_stateSeting = 0;
				_game->_canTouch = true;
				_offRotaItem = 0;
			}
			break;
		}
		_menuItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_menuItems[_currentItems]._opacity = _opacityItems;
		//PDEBUG("\n Update state setting 2 _offRotaItem: %f", _offRotaItem);
	}
}
void CMainMenu::OpenSetting()
{
	_stateSeting = 1;
	_stateRotaItem = 1;
	//_stateGGServices = 0;
	_game->_canTouch = false;
	_game->PlaySFX(SFX_WHEEL);
}
void CMainMenu::OpenGGServices() {
	_stateGGServices = 1;
	_stateRotaItem = 1;
	//_stateSeting = 0;
	_game->_canTouch = false;
	_game->PlaySFX(SFX_WHEEL);

}
void CMainMenu::CloseSetting()
{
	_stateSeting = 2;
	_isRenderSetting = false;
	_stateRotaItem = 1;
	_offRotaItem = 0;
	_speedRotaItem = 5;
	_opacityItems = 0;
	_game->_canTouch = false;
	//reset params
	_isRenderSetting = false;
	_stateSeting = 0;

	InitMenu();
}
void CMainMenu::CloseGGServices() {
	_stateGGServices = 2;
	_isRenderGGServices = false;
	_stateRotaItem = 1;
	_offRotaItem = 0;
	_speedRotaItem = 5;
	_opacityItems = 0;
	_game->_canTouch = false;
	//reset params
	_isRenderGGServices = false;
	_stateGGServices = 0;

	InitMenu();
}