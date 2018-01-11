#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"
extern "C" void	GoToReviewInItune();



void CShareScore::Init(int hightScore, int type)
{
	_game = GAME();
	_gp = G();
	_type = type;

	if (_type == CShareScore::enumTypeRender::RATE_US)
	{
		_game->PlaySFX(SFX_INGAME_HIGHSCORE);
		for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
		{
			_shareButton[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
			_shareButton[i].SetAnim(SPRITEID_HUD_RATE_US, 0 + 4 * i);
		}
	}
	else
	{
		_hightScore = hightScore;
		_game->PlaySFX(SFX_INGAME_HIGHSCORE);
		for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
		{
			_shareButton[i].Init(BUTTON_STATE_NORMAL, 0, 0, A_HVCENTER);
			_shareButton[i].SetAnim(SPRITEID_HUD_NEWHIGHSCORE, 0 + 4 * i);
		}
	}

	_stateHightScore = 1;
	_opacityTitle = 0;
	_stateRota = 1;
	_rotaTile = 0;
	_speedRota = 5;
	_game->_canTouch = false;
	isRated = false;
	isCompleteRate = false;
	stateRateGame = 0;
}
void CShareScore::UpdateHighScore()
{
	if (_stateHightScore == 1)
	{
		_opacityTitle += 3;
		if (_opacityTitle >= 100)
		{
			_opacityTitle = 100;
		}
		switch (_stateRota)
		{
		case 1:
			_rotaTile += _speedRota;
			_speedRota *= 1.1;
			if (_speedRota >= 20)
			{
				_speedRota = 21;
				_stateRota = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_rotaTile += _speedRota;
			_speedRota *= 0.9;
			if (_speedRota <= 1)
			{
				_rotaTile = 0;
				_stateRota = 0;
				_stateHightScore = 0;
				_game->_canTouch = true;
			}
			break;
		}
	}
	else if (_stateHightScore == 2)
	{
		_opacityTitle -= 3;
		if (_opacityTitle <= 0)
		{
			_opacityTitle = 0;
			_stateHightScore = 0;
			_type = CShareScore::enumTypeRender::TYPE_NONE;
			_game->_gameStateGamePlay._mainMenu.InitMenu();
		}
		switch (_stateRota)
		{
		case 1:
			_rotaTile += _speedRota;
			_speedRota *= 1.1;
			if (_speedRota >= 20)
			{
				_speedRota = 21;
				_stateRota = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_rotaTile += _speedRota;
			_speedRota *= 0.9;
			if (_speedRota <= 1)
			{
				_rotaTile = 0;
				_stateRota = 0;
				_stateHightScore = 0;
				_game->_canTouch = true;
			}
			break;
		}
	}
	else if (_stateHightScore == 10)
	{
		INT_TIME timeOut = time(NULL) - _game->_timeOut;
		if (timeOut >= 10)
		{
			PDEBUG("\n =================>Rated");
			//_game->_user._hadRate = true;
			_game->_user._isUnlockColor = true;
			_game->_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_Colorful);
			_stateHightScore = 2;
			_speedRota = 5;
			_stateRota = 1;
			_game->_user._numPlayRate = 0;
			_game->PlaySFX(SFX_WHEEL);
			_game->_user.DataSave();
		}
		else if (timeOut > 0)
		{
			_stateHightScore = 2;
			_speedRota = 5;
			_stateRota = 1;
			_game->_user._numPlayRate = 0;
			_game->PlaySFX(SFX_WHEEL);
		}
	}
}
void CShareScore::RenderHighScore(float x, float y)
{
	if (_type == CShareScore::enumTypeRender::RATE_US)
	{
		//text
		_gp->SetOpacity(_opacityTitle);
		_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 1, x, y);

		_gp->SetRotate(_rotaTile, k_SCREEN_WIDTH / 2 + x, k_SCREEN_HEIGHT / 2 + y);
		_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 0, x, y);
		for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
		{
			_shareButton[i].SetPos(x, y);
			_shareButton[i].Render(_gp);
			_shareButton[i].Update();
		}
		_gp->ClearRotate();
		int starW = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleWidth(1, 1) + 40 * 0.8f;

		for (int i = 0; i < enumStarRateStatus::RATE_STATE_LOVED; i++)
		{
			_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 8, x + i * starW, y);
		}

		for (int i = 0; i < stateRateGame; i++)
		{
			_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 7, x + i * starW, y);
		}
		_gp->SetOpacity(100);


	}
	else if (_type == CShareScore::enumTypeRender::CONFIRM_RATE_US)
	{
		_gp->SetOpacity(_opacityTitle);
		_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 6, x, y);

		_gp->SetRotate(_rotaTile, k_SCREEN_WIDTH / 2 + x, k_SCREEN_HEIGHT / 2 + y);
		_game->GetSpriteVer(SPRITEID_HUD_RATE_US)->DrawFrame(_gp, 0, x, y);
		_gp->ClearRotate();
		_gp->SetOpacity(100);
	}
	else
	{
		//so high
		_gp->SetOpacity(_opacityTitle);
		_gp->SetRotate(-_rotaTile * 2, k_SCREEN_WIDTH / 2 + x, k_SCREEN_HEIGHT / 2 + y);
		_game->GetSpriteVer(SPRITEID_FONTS_HIGH_SCORE)->DrawAnsiTextFormat(_gp, k_SCREEN_WIDTH / 2 + x, k_SCREEN_HEIGHT / 2 + y, _HCENTER | _VCENTER, "%d", _hightScore);
		_gp->ClearRotate();
		_game->GetSpriteVer(SPRITEID_HUD_NEWHIGHSCORE)->DrawFrame(_gp, 0, x, y);

		_gp->SetRotate(_rotaTile, k_SCREEN_WIDTH / 2 + x, k_SCREEN_HEIGHT / 2 + y);
		_game->GetSpriteVer(SPRITEID_HUD_NEWHIGHSCORE)->DrawFrame(_gp, 1, x, y);
		for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
		{
			_shareButton[i].SetPos(x, y);
			_shareButton[i].Render(_gp);
			_shareButton[i].Update();
		}
		_gp->ClearRotate();
		_gp->SetOpacity(100);
	}
}


#define SHARE_W		(750.0f)	
#define SHARE_H		(195.0f)	

void CShareScore::AddShare()
{
	_game = GAME();
	_gp = G();

	_stateShare = 1;
	_opacityShare = 0;
	_game->PlaySFX(SFX_SHARE);
	_game->show_ads_interrupt = false;
	PDEBUG("\n Share => _show_ads_interrupt:%d", _game->_show_ads_interrupt);
}
void CShareScore::RenderShare(float x, float y)
{
	if (_stateShare == 0)
	{
		return;
	}
	_gp->SetColor(0xff000000);
	int opa = _opacityShare * 0.8;
	if (opa >= 70)
	{
		opa = 70;
	}
	_gp->SetOpacity(opa);
	_gp->FillFullScreen(true);

	_gp->SetOpacity(_opacityShare);

	y += k_SCREEN_HEIGHT / 2 - (SHARE_H / 2);
	_gp->SetClip(x, y, SHARE_W, SHARE_H);

	_gp->SetColor(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_BG_COLOR));

#if defined ANDROID_OS
	_gp->FillRect(x, y - 14, SHARE_W, SHARE_H + 14, true);
#elif defined MAC_OS
	_gp->FillRect(x, y - 4, SHARE_W, SHARE_H + 4, true);
#else
	_gp->FillRect(x, y - 14, SHARE_W, SHARE_H + 14, true);
#endif



	_gp->SetColor(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_MAX_COLOR));
	_gp->FillCircle(0 + 220, k_SCREEN_HEIGHT / 2, 350, 20, true);

#if defined ANDROID_OS
	GET_SPRITE(SPRITEID_HUD_SHARE_ANDROID)->DrawFrame(_gp, 0, x, y);
#elif defined MAC_OS
	GET_SPRITE(SPRITEID_HUD_SHARE)->DrawFrame(_gp, 0, x, y);
#else
	GET_SPRITE(SPRITEID_HUD_SHARE_ANDROID)->DrawFrame(_gp, 0, x, y);
#endif


	_gp->SetColorMask(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_TEXT_COLOR));

#if defined ANDROID_OS
	GET_SPRITE(SPRITEID_HUD_SHARE_ANDROID)->DrawFrame(_gp, 1, x, y);
#elif defined MAC_OS
	GET_SPRITE(SPRITEID_HUD_SHARE)->DrawFrame(_gp, 1, x, y);
#else
	GET_SPRITE(SPRITEID_HUD_SHARE_ANDROID)->DrawFrame(_gp, 1, x, y);
#endif


	_gp->ClearColorMask();

	int stringX = x + 360;
	int stringY = y + 74;

	G()->SetScale(1.3, 1.3, stringX, stringY);
	GET_SPRITE(SPRITEID_FONTS_BEST_SCORE)->DrawAnsiTextFormat(G(), stringX, stringY, _LEFT | _VCENTER, "%d", _game->_user._highScore);
	G()->ClearScale();

	_gp->SetOpacity(100);
	_gp->ClearClip();
}
bool CShareScore::UpdateShare()
{
	switch (_stateShare)
	{
	case 10:
		if (_type == CShareScore::enumTypeRender::HIGHT_SCORE)
		{
			_stateHightScore = 2;
			_speedRota = 5;
			_stateRota = 1;
			_game->_user._numPlayRate = 0;
			_game->PlaySFX(SFX_WHEEL);
			_game->_user.DataSave();
		}
		_stateShare = 0;
		break;

	case 0:
		return false;
		break;
	case 1:
		_opacityShare += 10;
		if (_opacityShare >= 100)
		{
			_opacityShare = 100;
			_stateShare = 2;
		}
		break;

	case 2:
		_opacityShare += 1;
		if (_opacityShare >= 100)
		{
			int mx = k_SCREEN_WIDTH / 2 - (SHARE_W / 2);
			int my = k_SCREEN_HEIGHT / 2 - (SHARE_H / 2);
			float offsetx = 0;
			if (GAME()->_device._version == SupportVersion::RESVER_960_640)
			{
				offsetx = 64;
				my += 2;
			}
			else if (GAME()->_device._version == SupportVersion::RESVER_1024_768)
			{
				offsetx = 96;
				my += 2;
			}
			_game->_hasAction = true;
#ifdef MAC_OS
			PineSocial::SaveScreenToDocument("sharePicture", mx + offsetx, my, SHARE_W - offsetx * 2, SHARE_H);
#endif
			_stateShare = 3;
		}
		break;

	case 3:
		_opacityShare += 1;
		if (_opacityShare >= 110)
		{
			_opacityShare = 110;
			_stateShare = 4;

			sprintf(_stringFeed, "I scored %d in Shadeless!", _game->_user._highScore);
#ifdef _WINDOWS 
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL;
#elif defined (ANDROID_OS)
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE;
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHARE_SCREEN_SHOT_INTENT);
#else
			PineSocial::ShareImageWithURLDocument(_stringFeed, "sharePicture");
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_PROCESSING;
#endif


		}
		break;

	case 4:
		_opacityShare -= 7;
		if (_opacityShare <= 0)
		{
			_opacityShare = 0;
		}
		if (_game->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE
			|| _game->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL)
		{
			_opacityShare -= 7;
			if (_opacityShare <= 0)
			{
				_stateShare = 10;
				if (_game->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE)
				{
					_game->_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_ShowOff);
				}
				_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_NONE;
			}
		}
		break;
	}
	return true;
}
bool CShareScore::UpdateTouchButton()
{
	bool ret = false;
	int index = -1;
	for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
	{
		if (_shareButton[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_shareButton[index].NeedAction())
		{
			if (_type == CShareScore::enumTypeRender::RATE_US)
			{
				switch (index)
				{
				case 0://rate
					if (Device::IsNetworkAvailable())
					{
#ifdef MAC_OS
						_game->_isAdsProcessing = true;
						GoToReviewInItune();
						_stateHightScore = 10;
						_game->_timeOut = time(NULL);
						_game->_hasAction = true;
#endif
					}
					else
					{
						_game->_user.NetWorkError();
					}
#if defined (ANDROID_OS) || _WINDOWS
					PDEBUG("\n isRated:%d", isRated);
					if (isRated)
					{
						PDEBUG("\n 333333333333333333333333isRated:%d", isRated);
						isRated = false;
						_stateHightScore = 10;
						_speedRota = 5;
						_stateRota = 1;
						isCompleteRate = true;
						_game->_user.DataSave();
					}
#endif
					break;

				case 1://late
					_stateHightScore = 2;
					_speedRota = 5;
					_stateRota = 1;
					_game->_user._numPlayRate = 0;
					_game->PlaySFX(SFX_WHEEL);
					stateRateGame = CShareScore::enumStarRateStatus::RATE_STATE_NONE;
					_game->_user.DataSave();
					break;
				}
			}
			else
			{
				switch (index)
				{
				case CShareScore::enumShareButton::SHARE_NEXT:
					_stateHightScore = 2;
					_speedRota = 5;
					_stateRota = 1;
					_game->PlaySFX(SFX_WHEEL);

					break;

				case CShareScore::enumShareButton::SHARE_YEAH:
					AddShare();
					
					break;
				}
			}
			_shareButton[index].SetState(BUTTON_STATE_NORMAL, -1);

			for (int i = 0; i < SHARE_SCORE_BUTTON_NUMS; i++)
			{
				if (_shareButton[i]._state != BUTTON_STATE_LOCK && _shareButton[i]._state != BUTTON_STATE_HIDE)
				{
					_shareButton[i].SetState(BUTTON_STATE_NORMAL, -1);
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
			for (int touch = 0; touch < SHARE_SCORE_BUTTON_NUMS; touch++)
			{
				if (_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					continue;
				}
				if (_shareButton[touch].CheckState(BUTTON_STATE_LOCK) || _shareButton[touch].CheckState(BUTTON_STATE_HIDE))
				{
					continue;
				}

				int x = _shareButton[touch].TZ_GetX() - 20;
				int y = _shareButton[touch].TZ_GetY() - 20;
				int w = _shareButton[touch].TZ_GetWidth() + 40;
				int h = _shareButton[touch].TZ_GetHeight() + 40;
				Button *button = &_shareButton[touch];
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
						for (int j = 0; j < SHARE_SCORE_BUTTON_NUMS; j++)
						{
							if (_shareButton[touch].CheckState(BUTTON_STATE_LOCK) || _shareButton[touch].CheckState(BUTTON_STATE_HIDE))
							{
								_shareButton[j].SetState(BUTTON_STATE_NORMAL, -1);
							}
						}
						_game->ClearTouchProcessing();
					}
				}

				int starX = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleX(1, 1);
				int starY = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleY(1, 1) - 10;
				int starW = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleWidth(1, 1) + 40 * 0.8f;
				int starH = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleHeight(1, 1) + 30;

				//G()->FillRect(otherStar_x, otherStar_y, otherStar_w * enumStarRateStatus::RATE_STATE_LIKED, otherStar_h);

				int starW_5 = GET_SPRITE(SPRITEID_HUD_RATE_US)->GetFModuleWidth(1, 1);

				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, starX, starY, starW * enumStarRateStatus::RATE_STATE_LIKED, starH))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						PDEBUG("\n otherStar_y:%d", starY);
						//PDEBUG("\n Inside TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, otherStar_w * enumStarRateStatus::RATE_STATE_HATED);
						if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_HATED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_HATED;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_DISLIKED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_DISLIKED;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_OK))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_OK;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_LIKED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_LIKED;
						}
						isRated = true;
						PDEBUG("\n stateRateGame = %d", stateRateGame);
					}
					if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						//PDEBUG("\n TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, otherStar_w * enumStarRateStatus::RATE_STATE_HATED);
						if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_HATED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_HATED;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_DISLIKED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_DISLIKED;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_OK))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_OK;
						}
						else if (TOUCHS(i)->x < starX + (starW * enumStarRateStatus::RATE_STATE_LIKED))
						{
							stateRateGame = enumStarRateStatus::RATE_STATE_LIKED;
						}
						isRated = true;
						PDEBUG("\n stateRateGame = %d", stateRateGame);
					}
				}
				else if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, starX + starW * enumStarRateStatus::RATE_STATE_LIKED - 35 + 20, starY, starW + 40, starH))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						PDEBUG("\n outside TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, starW * enumStarRateStatus::RATE_STATE_HATED);
						isRated = true;
						stateRateGame = enumStarRateStatus::RATE_STATE_LOVED;
						PDEBUG("\n stateRateGame = %d", stateRateGame);
					}
					if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						isRated = true;
						stateRateGame = enumStarRateStatus::RATE_STATE_LOVED;
					}
				}

				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, 0, 0, k_SCREEN_WIDTH, k_SCREEN_HEIGHT))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						if (_type == CShareScore::enumTypeRender::CONFIRM_RATE_US)
						{
							_stateHightScore = 2;
							_speedRota = 5;
							_stateRota = 1;
							isCompleteRate = true;
							_game->_user.DataSave();
						}

					}
				}
			}
		}
	}
	return pressed;
}

