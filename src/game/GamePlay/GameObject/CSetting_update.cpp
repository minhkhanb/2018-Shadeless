#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"

extern "C" void SendMailSupport(const char *body, const char *title);

extern "C" void IapRestore();



void CSetting::Init()
{
	_game = GAME();
	_gp = G();

	_touchID = -1;
	_touchIDOld = -1;
	//#ifdef _WINDOWS
	//	_game->_user._isUnlockColor = false;
	//#endif
	_game->PlaySFX(SFX_WHEEL);

	int beginAngle = -30;
	_opacityItems = 100;
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		_settingItems[i]._angle = beginAngle;
		_settingItems[i]._angleCheck = beginAngle;
		_settingItems[i]._button.Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER);

		switch (i)
		{
		case CSetting::enumSettingItems::SETTING_SILENCE:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 0);
			_settingItems[i]._frame = 0;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 19;
			break;

		case CSetting::enumSettingItems::SETTING_CREDIT:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 4);
			_settingItems[i]._frame = 5;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 15;
			break;

		case CSetting::enumSettingItems::SETTING_CONTACT:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 12);
			_settingItems[i]._frame = 9;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 23;
			break;

		case CSetting::enumSettingItems::SETTING_BACK:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 8);
			_settingItems[i]._frame = 17;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 16;
			break;

		case CSetting::enumSettingItems::SETTING_COLOR:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 20);
			_settingItems[i]._frame = 21;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 15;
			break;

		case CSetting::enumSettingItems::SETTING_NOTIFI:
			_settingItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 0);
			_settingItems[i]._frame = 31;
			beginAngle += 40;
			_settingItems[i]._angleCheck += 25;
			break;
		}
	}

	//back credit
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER);
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.SetAnim(SPRITEID_HUD_SETTING, 8);
	_stateCredit = 0;
	_isRenderCredit = 0;

	//_scrollBarX.Init(0, 360-45, 1, 0.1, 1.1);
	//_scrollBarX.SetCurrent(160);
	_offsetAngle = 0;
	_currentItems = GetCurrentItems();
	_isTouchDrag = false;
	_stateMoveTarget = 0;
	_offRotaItem = 0;
	_state = 1;
	_stateRotaItem = 1;
	_speedRotaItem = 5;

	_offsetScroll.Init(1, 1.12, 20, 1);
}
void CSetting::Update()
{

	if (_state == 0)
	{
		_offsetScroll.Update();

		for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
		{
			_settingItems[i]._angle += _offsetScroll.GetCurrentSpeed();
			_settingItems[i]._angleCheck += _offsetScroll.GetCurrentSpeed();

			if (_settingItems[i]._angle > 0)
			{
				_settingItems[i]._angle -= 360;
			}
			if (_settingItems[i]._angle <= -0)
			{
				_settingItems[i]._angle += 360;
			}

			if (_settingItems[i]._angleCheck > 0)
			{
				_settingItems[i]._angleCheck -= 360;
			}
			if (_settingItems[i]._angleCheck <= -0)
			{
				_settingItems[i]._angleCheck += 360;
			}
		}

	}
	_currentItems = GetCurrentItems();
	//PDEBUG("\n _currentItems:%d", _currentItems);
	if (!_isRenderCredit)
	{
		for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
		{
			if (i == _currentItems)
			{
				if (_settingItems[i]._button._state == BUTTON_STATE_HIDE)
				{
					if (i == CSetting::enumSettingItems::SETTING_COLOR && !_game->_user._isUnlockColor)
					{
						_settingItems[i]._button.SetState(BUTTON_STATE_LOCK);
					}
					else
					{
						_settingItems[i]._button.SetState(BUTTON_STATE_NORMAL);
					}
				}
			}
			else
			{
				_settingItems[i]._button.SetState(BUTTON_STATE_HIDE);
			}
		}
		UpdateItems();
	}

	if (_state == 1)
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
				_state = 0;
				_game->_canTouch = true;
				_offRotaItem = 0;
			}
			break;
		}
		_settingItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_settingItems[_currentItems]._opacity = _opacityItems;
	}
	else if (_state == 2)
	{
		_opacityItems -= 2;
		if (_opacityItems <= 0)
		{
			if (_stateCredit == 1)
			{
				OpenCredit();
			}
			else
			{
				_game->_gameStateGamePlay._mainMenu.CloseSetting();
			}
			_opacityItems = 0;
			_state = 0;

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
		_settingItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_settingItems[_currentItems]._opacity = _opacityItems;
	}
	//PDEBUG("\n _rotaIcon:%f", _settingItems[_currentItems]._rotaIcon);
	if (_isRenderCredit)
	{
		UpdateCredit();
	}
}

void CSetting::Render(float x, float y)
{
	if (_isRenderCredit)
	{
		RenderCredit(x, y);
	}
	else
	{
		RenderItems(x, y);
	}

}
void CSetting::RenderItems(float x, float y)
{
	int step = 0;
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		_settingItems[i]._x = k_SCREEN_WIDTH / 2 + 300 * CMath::COS(_settingItems[i]._angle + _offsetAngle + _offRotaItem) + x;
		_settingItems[i]._y = k_SCREEN_HEIGHT / 2 + 300 * CMath::SIN(_settingItems[i]._angle + _offsetAngle + _offRotaItem) + y;

		_gp->SetRotate(_settingItems[i]._angle + _offsetAngle + _offRotaItem - 90, _settingItems[i]._x, _settingItems[i]._y);
		_gp->SetOpacity(_opacityItems);

		if (i == _currentItems)
		{
			_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _settingItems[i]._frame + 1, _settingItems[i]._x, _settingItems[i]._y);
		}
		else
		{
			_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _settingItems[i]._frame, _settingItems[i]._x, _settingItems[i]._y);
		}
		_gp->ClearRotate();

		_gp->SetOpacity(_settingItems[i]._opacity);
		if (i == CSetting::enumSettingItems::SETTING_COLOR
			&& !_settingItems[i]._button.CheckState(BUTTON_STATE_HIDE)
			&& !_settingItems[i]._button.CheckState(BUTTON_STATE_LOCK))
		{
			if (_settingItems[i]._button.CheckState(BUTTON_STATE_SELECT))
			{
				_gp->SetScale(1.2, 1.2, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
			_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 26, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			_gp->ClearScale();
			_gp->SetColorMask(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_BG_COLOR));
		}

		_gp->SetRotate(_settingItems[i]._rotaIcon, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_settingItems[i]._button.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_settingItems[i]._button.Render(_gp);
		_settingItems[i]._button.Update();
		_gp->ClearColorMask();

		if (i == CSetting::enumSettingItems::SETTING_SILENCE && !_settingItems[i]._button.CheckState(BUTTON_STATE_HIDE))
		{
			if (_game->IsEnableSound())
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 3, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
			else
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 4, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
		}
		else if (i == CSetting::enumSettingItems::SETTING_NOTIFI && !_settingItems[i]._button.CheckState(BUTTON_STATE_HIDE))
		{
			if (_game->_notificationEnable)
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 3, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
			else
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 4, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
			}
		}
		_gp->ClearRotate();
		_gp->SetOpacity(100);
	}

}
void CSetting::UpdateItems()
{
	if (_game->_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_MENU)
	{

		bool updateIcon = false;
		if (_timeMove > 0)
		{
			_timeMove--;
			for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
			{
				_settingItems[i]._angle += _speedMove;
				_settingItems[i]._angleCheck += _speedMove;
				//PDEBUG("\n _speedMove:%f", _speedMove);

				if (_settingItems[i]._angle > 0)
				{
					_settingItems[i]._angle -= 360;
				}
				if (_settingItems[i]._angle <= -0)
				{
					_settingItems[i]._angle += 360;
				}

				if (_settingItems[i]._angleCheck > 0)
				{
					_settingItems[i]._angleCheck -= 360;
				}
				if (_settingItems[i]._angleCheck <= -0)
				{
					_settingItems[i]._angleCheck += 360;
				}
			}

		}

		//return;
		if (_state == 0)
		{
			for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
			{
				if (i == _currentItems)
				{
					int angle = (int)(_settingItems[i]._angle + _offsetAngle) % 360;
					int angleCheck = (int)(_settingItems[i]._angleCheck + _offsetAngle) % 360;

					float offOpa = CMath::ABSOLUTE_VALUE(angle - 90) % 360;
					float range = 0;
					if (angle > 90)
					{
						range = CMath::ABSOLUTE_VALUE(_settingItems[i - 1]._angleCheck - _settingItems[i]._angle);
						if (i == CSetting::enumSettingItems::SETTING_SILENCE)
						{
							range = CMath::ABSOLUTE_VALUE(_settingItems[CSetting::enumSettingItems::SETTING_NOTIFI]._angleCheck - _settingItems[i]._angle - 360);
						}
					}
					else
					{
						range = CMath::ABSOLUTE_VALUE(_settingItems[i]._angleCheck - _settingItems[i]._angle);
					}
					_settingItems[i]._opacity = 100 * (1 - offOpa / range);



					float aaa = (_settingItems[i]._angle + _offsetAngle) - 90;
					if (aaa > 360)
					{
						aaa -= 360;
					}
					if (aaa < -360)
					{
						aaa += 360;
					}
					if (CMath::ABSOLUTE_VALUE(aaa) > 200)
					{
						if (angle > 90)
						{
							aaa += 360;
						}
						else
						{
							aaa -= 360;
						}
					}
					float bbb = (_settingItems[i]._angle - _settingItems[i]._angleCheck);
					_settingItems[i]._rotaIcon = 360 * (bbb - aaa) / bbb;
					//PDEBUG("\n _settingItems[%d]._rotaIcon:%f", i, _settingItems[i]._rotaIcon);
				}
			}
		}

		if (!_isTouchDrag)
		{
			if (_stateMoveTarget == 1 && !_offsetScroll.IsScroll())
			{
				_stateMoveTarget = 2;
			}
			else if (_stateMoveTarget == 2)
			{
				_targetItems = _currentItems;
				_timeMove = 20;
				_speedMove = ((_settingItems[_targetItems]._angle + _offsetAngle) - 90) / (float)_timeMove;
				_stateMoveTarget = 0;
			}

			if (_stateMoveTarget == 10 && !_offsetScroll.IsScroll())
			{
				_stateMoveTarget = 11;
			}
			else if (_stateMoveTarget == 11)
			{
				bool hasForce = false;
				if (_moveDirection == 1)
				{
					if (_currentItems > 0)
					{
						_targetItems = _currentItems - 1;
						hasForce = true;
					}
					else
					{
						_targetItems = SETTING_ITEMS_NUMS - 2;
						hasForce = true;
					}
				}
				else  if (_moveDirection == 2)
				{

					if (_currentItems < SETTING_ITEMS_NUMS - 2)
					{
						_targetItems = _currentItems + 1;
						hasForce = true;
					}
					else
					{
						_targetItems = 0;
						hasForce = true;
					}
				}
				if (hasForce)
				{
					_timeMove = 20;
					float curAngle = (_settingItems[_targetItems]._angle + _offsetAngle);
					if (curAngle > 360)
					{
						curAngle -= 360;
					}
					if (curAngle <= -360)
					{
						curAngle += 360;
					}
					curAngle -= 90;
					if (CMath::ABSOLUTE_VALUE(curAngle) > 200)
					{
						if (_moveDirection == 1)
						{
							curAngle -= 360;
						}
						else
						{
							curAngle += 360;
						}
					}
					_speedMove = -(curAngle / (float)_timeMove);
				}
				_stateMoveTarget = 0;
			}
		}
	}
}
int CSetting::GetCurrentItems()
{
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		float left, right;

		if (i == CSetting::enumSettingItems::SETTING_SILENCE)
		{
			left = _settingItems[i]._angleCheck + _offsetAngle;
			right = _settingItems[CSetting::enumSettingItems::SETTING_NOTIFI]._angleCheck + _offsetAngle;
			if (left >= 90 && left <= 128)
				return i;
		}
		else
		{
			left = _settingItems[i]._angleCheck + _offsetAngle;
			right = _settingItems[i - 1]._angleCheck + _offsetAngle;
		}
		//PDEBUG("\n index:%d --------- left:%f -- right:%f --- _offsetAngle:%f -- _angleCheck:%f", i, left, right, _offsetAngle, _settingItems[i]._angleCheck);
		if (left > 360)			left -= 360;
		if (left < -360)			left += 360;
		if (right > 360)			right -= 360;
		if (right < -360)			right += 360;

		left = (int)left % 360;
		right = (int)right % 360;
		//PDEBUG("\n left:%f -- right:%f", left, right);
		if ((right <= 90 && left >= 90)
			|| (right <= 90 - 360 && left >= 90 - 360)
			|| (right <= 90 + 360 && left >= 90 + 360))
		{
			//PDEBUG("\n currentItem:%d", i);
			return i;
		}
		//PDEBUG("\n ___currentItem:%d", i);
	}
	return -1;
}


bool CSetting::UpdateTouchButton()
{
	bool ret = false;
	int index = -1;
	for (int i = 0; i < SETTING_ITEMS_NUMS; i++)
	{
		if (_settingItems[i]._button._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			PDEBUG("\n index:%d", index);
			break;
		}
	}

#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress)
	{
		if (!_isRenderCredit)
		{
			_state = 2;
			_stateRotaItem = 1;
			_speedRotaItem = 5;
			_game->_canTouch = false;
			_game->PlaySFX(SFX_WHEEL);
			_game->PlaySoundButton();
		}
		else {
			CloseCredit();
			_game->_canTouch = false;
			_game->PlaySFX(SFX_WHEEL);
			_game->PlaySoundButton();
		}
		GAME()->_isBackkeyPress = false;
	}
#endif

	if (ret)
	{
		if (_settingItems[index]._button.NeedAction())
		{
			switch (index)
			{
			case CSetting::enumSettingItems::SETTING_BACK:
				_state = 2;
				_stateRotaItem = 1;
				_speedRotaItem = 5;
				_game->_canTouch = false;
				_game->PlaySFX(SFX_WHEEL);
				_game->PlaySoundButton();
				break;

			case CSetting::enumSettingItems::SETTING_CREDIT:
				_state = 2;
				_stateCredit = 1;
				_stateRotaItem = 1;
				_speedRotaItem = 5;
				_game->_canTouch = false;
				_game->PlaySFX(SFX_WHEEL);
				_game->PlaySoundButton();
				break;

			case CSetting::enumSettingItems::BACK_CREDIT:
				CloseCredit();
				_game->_canTouch = false;
				_game->PlaySFX(SFX_WHEEL);
				_game->PlaySoundButton();
				break;

			case CSetting::enumSettingItems::SETTING_SILENCE:
				_game->SetSoundEnable(!_game->IsEnableSound());
				_game->SetMusicEnable(_game->IsEnableSound());
				if (_game->IsEnableSound())
				{
					_game->StopCurrentMusic();
					_game->LoadMusic(BGM_MAINMENU);
					_game->PlayCurrentMusic(-1);
				}
				else
				{
					_game->StopCurrentMusic();
				}
				_game->_user.DataSave();
				_game->PlaySoundButton();
				break;

			case CSetting::enumSettingItems::SETTING_NOTIFI:
				_game->_notificationEnable = !_game->_notificationEnable;
				_game->_user.DataSave();
				_game->PlaySoundButton();
				break;

			case CSetting::enumSettingItems::SETTING_COLOR:
				PDEBUG("\n CSetting::enumSettingItems::SETTING_COLOR");
				if (_game->_user._isUnlockColor)
				{
					_game->PlaySFX(SFX_COLOR_CHANGE);
					_game->_user._curColorUse++;
					if (_game->_user._curColorUse >= TOTAL_COLOR_DES_ROW)
					{
						_game->_user._curColorUse = 0;
					}
					_game->_gameStateGamePlay._gamePlay.SetNewColor();
					_game->_user.DataSave();
				}
				break;

			case CSetting::enumSettingItems::SETTING_CONTACT:
				_game->_hasAction = true;
				_game->PlaySoundButton();
				if (Device::IsNetworkAvailable())
				{
#if defined (MAC_OS)
					char title[1024], body[1024];
					sprintf(title, "Shadeless's Support Request");
					sprintf(body, "%s", "");
					SendMailSupport(body, title);
#elif defined (ANDROID_OS)
					sprintf(GAME()->_async_task_manager._send_email_title, "%s", "[Shadeless! - Android]");
					sprintf(GAME()->_async_task_manager._send_email_body, "%s", "");
					GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SEND_EMAIL_INTENT);
#endif
				}
				else
				{
					_game->_user.NetWorkError();
				}
				_game->show_ads_interrupt = false;
				break;

			}
			//_settingItems[index]._button.SetState(BUTTON_STATE_NORMAL, -1);
			_settingItems[6]._button.SetState(BUTTON_STATE_NORMAL, -1);

			for (int i = 0; i < SETTING_ITEMS_NUMS; i++)
			{
				if (_settingItems[i]._button._state != BUTTON_STATE_LOCK && _settingItems[i]._button._state != BUTTON_STATE_HIDE)
				{
					_settingItems[i]._button.SetState(BUTTON_STATE_NORMAL, -1);
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
			for (int touch = 0; touch < SETTING_ITEMS_NUMS; touch++)
			{
				if (_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					continue;
				}
				if (_settingItems[touch]._button._state == BUTTON_STATE_LOCK || _settingItems[touch]._button._state == BUTTON_STATE_HIDE)
				{
					continue;
				}

				int x = _settingItems[touch]._button.TZ_GetX() - 20;
				int y = _settingItems[touch]._button.TZ_GetY() - 20;
				int w = _settingItems[touch]._button.TZ_GetWidth() + 40;
				int h = _settingItems[touch]._button.TZ_GetHeight() + 40;
				Button *button = &_settingItems[touch]._button;
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						//PDEBUG("\n UP touch id:%d -- x:%d -- y:%d -- w:%d -- h:%d", touch, x, y, w, h);
						if (!button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id) && Button::GetButtonActive() == NULL)
						{

							button->SetState(BUTTON_STATE_SELECT, TOUCHS(i)->_id, TOUCH_DOWN);
							_game->SetTouchProcessing(TOUCHS(i)->_id);
							/*if (!_game->_user._isUnlockColor)
							{
								_settingItems[4]._button.SetState(BUTTON_STATE_LOCK, TOUCHS(i)->_id, TOUCH_DOWN);
							}*/

							pressed = true;
						}
						else if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							pressed = true;
						}
						//PDEBUG("\n DOWN touch id:%d -- x:%d -- y:%d -- w:%d -- h:%d -- targetitem:%d", touch, x, y, w, h, _targetItems);
					}
					else if (TOUCHS(i)->_state == TOUCH_DRAG)
					{
						//PDEBUG("\n touch id:%d -- x:%d -- y:%d -- w:%d -- h:%d", touch, x, y, w, h);
						if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							pressed = true;

						}
						//PDEBUG("\n DRAG touch id:%d -- x:%d -- y:%d -- w:%d -- h:%d", touch, x, y, w, h);
					}
					else if (TOUCHS(i)->_state == TOUCH_UP)
					{
						if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
						{
							button->SetState(BUTTON_STATE_ACTIVE);
						}
						//PDEBUG("\n UP touch id:%d -- x:%d -- y:%d -- w:%d -- h:%d", touch, x, y, w, h);

					}

				}
				else
				{
					if (button->CheckState(BUTTON_STATE_SELECT, TOUCHS(i)->_id))
					{
						for (int j = 0; j < SETTING_ITEMS_NUMS; j++)
						{
							if (_settingItems[j]._button._state != BUTTON_STATE_LOCK && _settingItems[j]._button._state != BUTTON_STATE_HIDE)
							{
								if (_game->_user._isUnlockColor)
								{
									_settingItems[j]._button.SetState(BUTTON_STATE_NORMAL, -1);
								}

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

void CSetting::UpdateTouch()
{
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{

				if (!_offsetScroll.CheckTouch(TOUCHS(i)->_id) && !_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					_beginAngleScroll = CMath::ANGLE(TOUCHS(i)->x, TOUCHS(i)->y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
					PDEBUG("\n _beginAngleScroll =%f", _beginAngleScroll);
					_offAngleScroll = 0;
					_isTouchDrag = false;

					_touchDragX = TOUCHS(i)->x;
					_touchDragY = TOUCHS(i)->y;

					_offsetScroll.SetBeginScorll(TOUCHS(i)->_id);
					_game->SetTouchProcessing(TOUCHS(i)->_id);
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_DRAG)
			{
				if (_offsetScroll.CheckTouch(TOUCHS(i)->_id))
				{
					_offAngleScroll = CMath::ANGLE(TOUCHS(i)->x, TOUCHS(i)->y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2) - _beginAngleScroll;
					_beginAngleScroll = CMath::ANGLE(TOUCHS(i)->x, TOUCHS(i)->y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
					_offsetScroll.AddOffset(_offAngleScroll);
					_isTouchDrag = true;

					_moveDirection = 0;
					if (_offAngleScroll > 0)
					{
						_moveDirection = 1;
					}
					else if (_offAngleScroll < 0)
					{
						_moveDirection = 2;
					}

					_touchDragX = TOUCHS(i)->x;
					_touchDragY = TOUCHS(i)->y;
					PDEBUG("\n _offAngleScroll =%f", _offAngleScroll);
				}
			}
			else if (TOUCHS(i)->_state == TOUCH_UP)
			{
				if (_offsetScroll.CheckTouch(TOUCHS(i)->_id))
				{
					_offsetScroll.SetState(2);
					_game->PlaySoundButton();
					if (CMath::ABSOLUTE_VALUE(_offAngleScroll) < 1)
					{
						_targetItems = GetIndexTouch(TOUCHS(i)->x, TOUCHS(i)->y);
						PDEBUG("\n _targetItems:%d", _targetItems);

						if (_targetItems != -1 && _targetItems != _currentItems)
						{
							_timeMove = 30;
							float curAngle = (_settingItems[_targetItems]._angle + _offsetAngle);
							if (curAngle > 360)
							{
								curAngle -= 360;
							}
							if (curAngle <= -360)
							{
								curAngle += 360;
							}
							curAngle -= 90;
							if (CMath::ABSOLUTE_VALUE(curAngle) > 200)
							{
								if (TOUCHS(i)->x > k_SCREEN_WIDTH / 2)
								{
									curAngle -= 360;
								}
								else if (TOUCHS(i)->x < k_SCREEN_WIDTH / 2)
								{
									curAngle += 360;
								}
							}
							_speedMove = -(curAngle / (float)_timeMove);
						}
						else
						{
							_stateMoveTarget = 10;
						}
					}
					else
					{
						_stateMoveTarget = 10;
					}
					_offsetScroll.ClearTouchScroll();
					_game->ClearTouchProcessing();
					_isTouchDrag = false;
				}
			}
		}
	}
}
int  CSetting::GetIndexTouch(float x, float y)
{
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		if (CHECK_POINT_IN_CIRCLE(x, y, _settingItems[i]._x, _settingItems[i]._y, 100))
		{
			return i;
		}
	}
	return -1;
}

void CSetting::UpdateCredit()
{
	if (_stateCredit == 2)
	{
		_opacityItems += 3;
		if (_opacityItems >= 100)
		{
			_opacityItems = 100;
		}
		switch (_stateRotaItem)
		{
		case 1:
			_creditAngle += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_creditAngle += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 0.3)
			{
				_creditAngle = 0;
				_speedRotaItem = 0.2;
				_stateRotaItem = 0;
				_stateCredit = 3;
				_game->_canTouch = true;
			}
			break;
		}
	}
	else if (_stateCredit == 3)
	{
		_creditAngle += _speedRotaItem;
		if (_creditAngle > 360)
		{
			_creditAngle -= 360;
		}
	}
	else if (_stateCredit == 4)
	{
		_opacityItems -= 2;
		if (_opacityItems <= 0)
		{
			_game->PlaySFX(SFX_WHEEL);
			_isRenderCredit = false;
			_stateCredit = 0;
			_opacityItems = 0;
			_state = 1;
			_stateRotaItem = 1;
			_speedRotaItem = 5;
		}

		switch (_stateRotaItem)
		{
		case 1:
			_creditAngle += _speedRotaItem;
			_speedRotaItem *= 1.1;
			if (_speedRotaItem >= 20)
			{
				_stateRotaItem = 2;
				_timeRoteItems = 30;
			}
			break;

		case 2:
			_creditAngle += _speedRotaItem;
			_speedRotaItem *= 0.9;
			if (_speedRotaItem <= 1)
			{
				_stateRotaItem = 0;
				_offRotaItem = 0;
			}
			break;
		}
	}

}
void CSetting::RenderCredit(float x, float y)
{
	_gp->SetOpacity(_opacityItems);
	if (_stateCredit == 2 || _stateCredit == 4)
	{
		_gp->SetRotate(-_creditAngle * 2, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	}
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.Render(_gp);
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.Update();
	_gp->ClearRotate();
	_gp->SetOpacity(100);

	float beginAg = _creditAngle;
	for (int i = 0; i < 5; i++)
	{
		float px = k_SCREEN_WIDTH / 2 + 320 * CMath::COS(beginAg) + x;
		float py = k_SCREEN_HEIGHT / 2 + 320 * CMath::SIN(beginAg) + y;


		_gp->SetRotate(beginAg - 88, px, py);
		_gp->SetOpacity(_opacityItems);
		if (i == 4)
		{
			_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 42, px, py);
		}
		else
		{
			_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, 33 + i, px, py);
		}

		_gp->ClearRotate();
		_gp->SetOpacity(100);
		switch (i)
		{
		case 0:
			beginAg += 53;
			break;

		case 3:
			beginAg += 85;
			break;

		default:
			beginAg += 360 / 5.0f;
			break;
		}

	}
}
void CSetting::OpenCredit()
{
	_settingItems[CSetting::enumSettingItems::BACK_CREDIT]._button.SetState(BUTTON_STATE_NORMAL);
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		_settingItems[i]._button.SetState(BUTTON_STATE_HIDE);
	}
	_stateCredit = 2;
	_isRenderCredit = true;
	_stateRotaItem = 1;
	_speedRotaItem = 5;
	_creditAngle = 0;
	_game->PlaySFX(SFX_WHEEL);
}
void CSetting::CloseCredit()
{
	Init();
	/*_stateCredit = 4;
	_stateRotaItem = 1;
	_speedRotaItem = 5;
	int beginAngle = -30;
	_opacityItems = 0;
	for (int i = 0; i < SETTING_ITEMS_NUMS - 1; i++)
	{
		_settingItems[i]._angle = beginAngle;
		_settingItems[i]._angleCheck = beginAngle;
		_settingItems[i]._button.Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER);
		switch (i)
		{
		case CSetting::enumSettingItems::SETTING_SILENCE:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 19;
			break;

		case CSetting::enumSettingItems::SETTING_CREDIT:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 15;
			break;

		case CSetting::enumSettingItems::SETTING_CONTACT:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 23;
			break;

		case CSetting::enumSettingItems::SETTING_BACK:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 16;
			break;


		case CSetting::enumSettingItems::SETTING_COLOR:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 15;
			break;

		case CSetting::enumSettingItems::SETTING_NOTIFI:
			beginAngle += 40;
			_settingItems[i]._angleCheck += 25;
			break;
		}
	}*/
}