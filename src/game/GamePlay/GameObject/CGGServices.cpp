#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"



void CGGServices::Init() {
	_game = GAME();
	_gp = G();

	_touchID = -1;
	_touchIDOld = -1;
#ifdef _WINDOWS
	_game->_user._isUnlockColor = true;
#endif
	_game->PlaySFX(SFX_WHEEL);

	int beginAngle = -20;
	_opacityItems = 0;
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		_ggplayItems[i]._angle = beginAngle;
		_ggplayItems[i]._angleCheck = beginAngle;
		_ggplayItems[i]._button.Init(BUTTON_STATE_HIDE, 0, 0, A_HVCENTER);
		switch (i)
		{

		case CGGServices::enumGGItems::GGSERVICES_LEADERBOARD:
			_ggplayItems[i]._button.SetAnim(SPRITEID_HUD_GOOGLEPLAY, 0);
			_ggplayItems[i]._frame = 4;
			beginAngle += 40;
			_ggplayItems[i]._angleCheck += 25;
			break;

		case CGGServices::enumGGItems::GGSERVICES_ACHIEVEMENT:
			_ggplayItems[i]._button.SetAnim(SPRITEID_HUD_GOOGLEPLAY, 4);
			_ggplayItems[i]._frame = 8;
			beginAngle += 40;
			_ggplayItems[i]._angleCheck += 25;
			break;

		case CGGServices::enumGGItems::GGSERVICES_SYNC:
			_ggplayItems[i]._button.SetAnim(SPRITEID_HUD_GOOGLEPLAY, 8);
			_ggplayItems[i]._frame = 0;
			beginAngle += 30;
			_ggplayItems[i]._angleCheck += 20;
			break;

		case CGGServices::enumGGItems::GGSERVICES_BACK:
			_ggplayItems[i]._button.SetAnim(SPRITEID_HUD_SETTING, 8);
			_ggplayItems[i]._frame = 17;
			beginAngle += 30;
			_ggplayItems[i]._angleCheck += 20;
			break;

		}
	}

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

int CGGServices::GetCurrentItems()
{
	//return 0;
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		float left, right;
		if (i == GGSERVICES_LEADERBOARD)
		{
			left = _ggplayItems[i]._angleCheck + _offsetAngle;
			right = 140 - _ggplayItems[GGSERVICES_BACK]._angleCheck + _offsetAngle;
		}
		else
		{
			left = _ggplayItems[i]._angleCheck + _offsetAngle;
			right = _ggplayItems[i - 1]._angleCheck + _offsetAngle;
		}
		//PDEBUG("\n index:%d --------- left:%f -- right:%f --- _offsetAngle:%f -- angleCheck:%f", i, left, right, _offsetAngle, _offsetAngle, _ggplayItems[i]._angleCheck);
		if (left > 360)			left -= 360;
		if (left < -360)			left += 360;
		if (right > 360)			right -= 360;
		if (right < -360)			right += 360;

		left = (int)left % 360;
		right = (int)right % 360;

		if ((right <= 90 && left >= 90)
			|| (right <= 90 - 360 && left >= 90 - 360)
			|| (right <= 90 + 360 && left >= 90 + 360))
		{
			return i;
		}
	}
	return -1;
}

void CGGServices::Render(float x, float y) {
	RenderItems(x, y);
}

void CGGServices::Update() {
	if (_state == 0)
	{
		_offsetScroll.Update();

		for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
		{
			_ggplayItems[i]._angle += _offsetScroll.GetCurrentSpeed();
			_ggplayItems[i]._angleCheck += _offsetScroll.GetCurrentSpeed();
			

			if (_ggplayItems[i]._angle > 0)
			{
				_ggplayItems[i]._angle -= 360;
			}
			if (_ggplayItems[i]._angle <= -0)
			{
				_ggplayItems[i]._angle += 360;
			}

			if (_ggplayItems[i]._angleCheck > 0)
			{
				_ggplayItems[i]._angleCheck -= 360;
			}
			if (_ggplayItems[i]._angleCheck <= -0)
			{
				_ggplayItems[i]._angleCheck += 360;
			}
			//PDEBUG("\n _ggplayItems[%d]._angleCheck:%f, _offsetScroll.GetCurrentSpeed():%f",i, _ggplayItems[i]._angleCheck, _offsetScroll.GetCurrentSpeed());
		}

	}
	_currentItems = GetCurrentItems();
	//PDEBUG("\n _currentItems:%d", _currentItems);
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		if (i == _currentItems)
		{
			if (_ggplayItems[i]._button._state == BUTTON_STATE_HIDE)
			{
				_ggplayItems[i]._button.SetState(BUTTON_STATE_NORMAL);
			}
		}
		else
		{
			_ggplayItems[i]._button.SetState(BUTTON_STATE_HIDE);
		}
	}
	UpdateItems();
	//PDEBUG("\n _state:%d", _state);
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
		_ggplayItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_ggplayItems[_currentItems]._opacity = _opacityItems;
	}
	else if (_state == 2)
	{
		_opacityItems -= 2;
		if (_opacityItems <= 0)
		{
			_game->_gameStateGamePlay._mainMenu.CloseGGServices();
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
		_ggplayItems[_currentItems]._rotaIcon = -_offRotaItem * 2;
		_ggplayItems[_currentItems]._opacity = _opacityItems;
	}
}

void CGGServices::UpdateItems() {
	if (_game->_gameStateGamePlay._status == CGameStateGamePlay::enumStatusState::STATE_MENU)
	{

		bool updateIcon = false;
		if (_timeMove > 0)
		{
			
			_timeMove--;
			for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
			{
				_ggplayItems[i]._angle += _speedMove;
				_ggplayItems[i]._angleCheck += _speedMove;
				
				if (_ggplayItems[i]._angle > 0)
				{
					_ggplayItems[i]._angle -= 360;
				}
				if (_ggplayItems[i]._angle <= -0)
				{
					_ggplayItems[i]._angle += 360;
				}

				if (_ggplayItems[i]._angleCheck > 0)
				{
					_ggplayItems[i]._angleCheck -= 360;
				}
				if (_ggplayItems[i]._angleCheck <= -0)
				{
					_ggplayItems[i]._angleCheck += 360;
					
				}
				//PDEBUG("\n GG _speedMove:%f --- _ggplayItems[i]._angleCheck:%f", _speedMove, _ggplayItems[i]._angleCheck);
			}

		}

		

		if (_state == 0)
		{
			
			for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
			{
				if (i == _currentItems)
				{
					int angle = (int)(_ggplayItems[i]._angle + _offsetAngle) % 360;
					int angleCheck = (int)(_ggplayItems[i]._angleCheck + _offsetAngle) % 360;

					float offOpa = CMath::ABSOLUTE_VALUE(angle - 90) % 360;
					float range = 0;
					if (angle > 90)
					{
						range = CMath::ABSOLUTE_VALUE(_ggplayItems[i - 1]._angleCheck - _ggplayItems[i]._angle);
					}
					else
					{
						range = CMath::ABSOLUTE_VALUE(_ggplayItems[i]._angleCheck - _ggplayItems[i]._angle);
					}
					_ggplayItems[i]._opacity = 100 * (1 - offOpa / range);



					float aaa = (_ggplayItems[i]._angle + _offsetAngle) - 90;
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
					float bbb = (_ggplayItems[i]._angle - _ggplayItems[i]._angleCheck);
					_ggplayItems[i]._rotaIcon = 360 * (bbb - aaa) / bbb;
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
				_speedMove = ((_ggplayItems[_targetItems]._angle + _offsetAngle) - 90) / (float)_timeMove;
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
						_targetItems = CGGSERVICES_ITEMS_NUMS - 1;
						hasForce = true;
					}
				}
				else  if (_moveDirection == 2)
				{

					if (_currentItems < CGGSERVICES_ITEMS_NUMS - 1)
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
					float curAngle = (_ggplayItems[_targetItems]._angle + _offsetAngle);
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

void CGGServices::UpdateTouch() {
	for (int i = 0; i < CGame::_this->_num_touches; i++)
	{
		if (TOUCHS(i)->_flag)
		{
			if (TOUCHS(i)->_state == TOUCH_DOWN)
			{
				if (!_offsetScroll.CheckTouch(TOUCHS(i)->_id) && !_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					_beginAngleScroll = CMath::ANGLE(TOUCHS(i)->x, TOUCHS(i)->y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
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
					printf("\n _offAngleScroll =%f", _offAngleScroll);
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
						if (_targetItems != -1 && _targetItems != _currentItems)
						{
							_timeMove = 30;
							float curAngle = (_ggplayItems[_targetItems]._angle + _offsetAngle);
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

void CGGServices::RenderItems(float x, float y) {
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		_ggplayItems[i]._x = k_SCREEN_WIDTH / 2 + 300 * CMath::COS(_ggplayItems[i]._angle + _offsetAngle + _offRotaItem) + x;
		_ggplayItems[i]._y = k_SCREEN_HEIGHT / 2 + 300 * CMath::SIN(_ggplayItems[i]._angle + _offsetAngle + _offRotaItem) + y;
		
		_gp->SetRotate(_ggplayItems[i]._angle + _offsetAngle + _offRotaItem - 90, _ggplayItems[i]._x, _ggplayItems[i]._y);
		_gp->SetOpacity(_opacityItems);
		if (i == _currentItems)
		{
			//PDEBUG("\n _offsetAngle:%f -- _offRotaItem:%f --- _ggplayItems[%d]._angle:%f", _offsetAngle, _offRotaItem, i, _ggplayItems[i]._angle);
			if (i == CGGServices::enumGGItems::GGSERVICES_BACK)
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _ggplayItems[i]._frame + 1, _ggplayItems[i]._x, _ggplayItems[i]._y);
			}
			else {
				_game->GetSpriteVer(SPRITEID_HUD_GOOGLEPLAY)->DrawFrame(_gp, _ggplayItems[i]._frame + 1, _ggplayItems[i]._x, _ggplayItems[i]._y);
			}
		}
		else
		{
			if (i == CGGServices::enumGGItems::GGSERVICES_BACK)
			{
				_game->GetSpriteVer(SPRITEID_HUD_SETTING)->DrawFrame(_gp, _ggplayItems[i]._frame, _ggplayItems[i]._x, _ggplayItems[i]._y);
			}
			else {
				_game->GetSpriteVer(SPRITEID_HUD_GOOGLEPLAY)->DrawFrame(_gp, _ggplayItems[i]._frame, _ggplayItems[i]._x, _ggplayItems[i]._y);
			}
		}
		_gp->ClearRotate();

		_gp->SetOpacity(_ggplayItems[i]._opacity);

		_gp->SetRotate(_ggplayItems[i]._rotaIcon, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_ggplayItems[i]._button.SetPos(k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2);
		_ggplayItems[i]._button.Render(_gp);
		_ggplayItems[i]._button.Update();
		_gp->ClearColorMask();
		_gp->ClearRotate();
		_gp->SetOpacity(100);
		//PDEBUG("\n _currentItems:%d", _currentItems);

	}
}
bool CGGServices::UpdateTouchButton() {
	bool ret = false;
	int index = -1;
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		if (_ggplayItems[i]._button._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}

#if !defined (MAC_OS)
	if (GAME()->_isBackkeyPress)
	{
		_state = 2;
		_stateRotaItem = 1;
		_speedRotaItem = 5;
		_game->_canTouch = false;
		_game->PlaySFX(SFX_WHEEL);
		_game->PlaySoundButton();
		GAME()->_isBackkeyPress = false;
	}
#endif

	if (ret)
	{
		if (_ggplayItems[index]._button.NeedAction())
		{
			switch (index)
			{
			case CGGServices::enumGGItems::GGSERVICES_BACK:
				_state = 2;
				_stateRotaItem = 1;
				_speedRotaItem = 5;
				_game->_canTouch = false;
				_game->PlaySFX(SFX_WHEEL);
				_game->PlaySoundButton();
				break;

			case CGGServices::enumGGItems::GGSERVICES_LEADERBOARD:
				PDEBUG("\n show leaderboard");
				_game->GGPlayShowLeaderBoard();
				_game->show_ads_interrupt = false;
				break;

			case CGGServices::enumGGItems::GGSERVICES_ACHIEVEMENT:
				PDEBUG("\n show achievement");
				_game->GGPlayShowAchievement();
				_game->show_ads_interrupt = false;
				break;

			case CGGServices::enumGGItems::GGSERVICES_SYNC:
				PDEBUG("\n show sync");
				_game->GGPlaySynDriver();
				
				break;

			}
			_ggplayItems[index]._button.SetState(BUTTON_STATE_NORMAL, -1);

			for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
			{
				if (_ggplayItems[i]._button._state != BUTTON_STATE_LOCK && _ggplayItems[i]._button._state != BUTTON_STATE_HIDE)
				{
					_ggplayItems[i]._button.SetState(BUTTON_STATE_NORMAL, -1);
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
			for (int touch = 0; touch < CGGSERVICES_ITEMS_NUMS; touch++)
			{
				if (_game->CheckTouchProcessing(TOUCHS(i)->_id))
				{
					continue;
				}
				if (_ggplayItems[touch]._button._state == BUTTON_STATE_LOCK || _ggplayItems[touch]._button._state == BUTTON_STATE_HIDE)
				{
					continue;
				}

				int x = _ggplayItems[touch]._button.TZ_GetX() - 20;
				int y = _ggplayItems[touch]._button.TZ_GetY() - 20;
				int w = _ggplayItems[touch]._button.TZ_GetWidth() + 40;
				int h = _ggplayItems[touch]._button.TZ_GetHeight() + 40;
				Button *button = &_ggplayItems[touch]._button;
				if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
				{
					if (TOUCHS(i)->_state == TOUCH_DOWN)
					{
						//PDEBUG("\n touch id = %d", touch);
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
						for (int j = 0; j < CGGSERVICES_ITEMS_NUMS; j++)
						{
							if (_ggplayItems[j]._button._state != BUTTON_STATE_LOCK && _ggplayItems[j]._button._state != BUTTON_STATE_HIDE)
							{
								_ggplayItems[j]._button.SetState(BUTTON_STATE_NORMAL, -1);
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
int CGGServices::GetIndexTouch(float x, float y) {
	for (int i = 0; i < CGGSERVICES_ITEMS_NUMS; i++)
	{
		if (CHECK_POINT_IN_CIRCLE(x, y, _ggplayItems[i]._x, _ggplayItems[i]._y, 100))
		{
			return i;
		}
	}
	return -1;
}