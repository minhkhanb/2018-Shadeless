#include "pch.h"
#include "../../GameCore/CGame.h"
#include "../../GameCore/Os_Functions.h"


int _stateShare;
int _timeEffect = 0;
COLOR _color = 0xffffff;
extern "C" void PurchaseProduct(const char *indentify);
void CDialog::SetInitGame(CGame *game)
{
	_game = game;
	_gp = game->_g;
	_typeDialog = CDialog::enumTypeDialog::DIALOG_NONE;
	_buttonChoose = -1;
	_opacity = 100;
	_stateShare = 0;
	stateRateGame = enumStarRateStatus::RATE_STATE_NONE;
	isRated = false;
}

void CDialog::AddDialogPopup(int typeDialog, int typeTrans)
{
	_typeDialog = typeDialog;
	_typeTrans = typeTrans;
	//PLAY_SFX(SFX_POPUP);
	_timeEffect = 0;
	InitTrans(typeTrans);
	//game->_canUpdateTouch = false;
	for (int i = 0; i < 2; i++)
	{
		_button[i].Init(BUTTON_STATE_NORMAL, _offX, _offY, A_HVCENTER);
		_button[i].SetAnim(SPRITEID_HUD_RATE, i * 4);
	}

	switch (_typeDialog)
	{
	case CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING:
		_button[DIALOG_BUTTON_RATE_LATER].SetAnim(SPRITEID_HUD_RATE, 4);
		_button[DIALOG_BUTTON_RATE].SetAnim(SPRITEID_HUD_RATE, 0);
		break;

	}

}

void CDialog::InitTrans(int typeTrans)
{
	_color = 0xffffff;
	switch (_typeTrans)
	{
	case CDialog::enumTypeTrans::ZOOM_IN:
		_state = 1;
		_zoomX = 0;
		_zoomY = 1;
		_offX = 0;
		_offY = 0;
		_opacityScreen = 0;
		_centerX = k_SCREEN_WIDTH / 2;
		_centerY = k_SCREEN_HEIGHT / 2;
		break;

	case CDialog::enumTypeTrans::_MOVE_IN_RIGHT_:
		_state = 1;
		_zoomX = 1;
		_zoomY = 1;
		_offX = -k_SCREEN_WIDTH;
		_offY = 0;
		_opacityScreen = 0;
		_centerX = k_SCREEN_WIDTH / 2;
		_centerY = k_SCREEN_HEIGHT / 2;
		break;

	case CDialog::enumTypeTrans::_MOVE_IN_LEFT_:
		_state = 1;
		_opacity = 0;
		_zoomX = 1;
		_zoomY = 1;
		_offX = k_SCREEN_WIDTH;
		_offY = 0;
		_opacityScreen = 0;
		_centerX = k_SCREEN_WIDTH / 2;
		_centerY = k_SCREEN_HEIGHT / 2;
		break;
	}
}
bool CDialog::IsOpening()
{
	return !(_typeDialog == CDialog::enumTypeDialog::DIALOG_NONE);
}
void CDialog::Render(float x, float y)
{
	if (_typeDialog == CDialog::enumTypeDialog::DIALOG_NONE)
	{
		return;
	}

	PineMatrix4 trans;
	trans = CMath::Matrix4Scale2D(_zoomX, _zoomY, _centerX, _centerY);

	G()->SetColor(0xff000000);
	_gp->SetOpacity(_opacityScreen);
	_gp->FillFullScreen(true);
	_gp->SetOpacity(100);
	_gp->SetScale(_zoomX, _zoomY, _centerX, _centerY);
	switch (_typeDialog)
	{
	case CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING:
	case CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE:
	case CDialog::enumTypeDialog::COMFIRM_SHOW_NOTIFY:
	case CDialog::enumTypeDialog::CONFIRM_RATED:
		RenderDialogPopup(x + _offX, y + _offY, trans);
		break;
	}

	for (int i = 0; i < 2; i++)
	{
		_button[i].SetPos(x + _offX, y + _offY);

		if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_NOTIFY)
		{
			if (i == 0)
			{
				_button[i].SetPos(x + _offX, y + _offY - 95);
			}
			else
			{
				_button[i].SetPos(x + _offX, y + _offY - 55);
			}
		}
		if (_typeDialog == CDialog::enumTypeDialog::CONFIRM_RATED)
		{
			continue;
		}
		_button[i].Render(_gp);
		_button[i].Update();
	}
	_gp->ClearScale();
	_gp->SetOpacity(100);

	if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE)
	{
		RenderShare(x, y, 100);
	}
}

void CDialog::RenderDialogPopup(float x, float y, PineMatrix4 tranf)
{
	PineMatrix4 scale;

	if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_NOTIFY)
	{

	}
	else if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING)
	{
		_game->GetSpriteVer(SPRITEID_HUD_RATE)->DrawFrame(_gp, 0, x, y);
		_game->GetSpriteVer(SPRITEID_HUD_RATE)->DrawFrame(_gp, 1, x, y);
		_game->GetSpriteVer(SPRITEID_HUD_RATE)->DrawFrame(_gp, 2, x, y);

		int otherStar_w = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameWidth(3) + 40 * 0.6f;

		for (int i = 0; i < enumStarRateStatus::RATE_STATE_LOVED; i++)
		{
			_game->GetSpriteVer(SPRITEID_HUD_RATE)->DrawFrame(_gp, 3, x + i * otherStar_w, y);
		}

		for (int i = 0; i < stateRateGame; i++)
		{
			_game->GetSpriteVer(SPRITEID_HUD_RATE)->DrawFrame(_gp, 4, x + i * otherStar_w, y);
		}
	}
	else if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE)
	{
		_game->GetSpriteVer(SPRITEID_HUD_MENU)->DrawFrame(_gp, 7, x, y);

		float scoreX = x + k_SCREEN_WIDTH / 2;
		float scoreY = y + k_SCREEN_HEIGHT / 2;
		_gp->SetColorMask(0xffffd100);
		int charspacing = _game->GetSpriteVer(SPRITEID_FONT_MAIN_NEWSCORE)->_char_spacing;
		_game->GetSpriteVer(SPRITEID_FONT_MAIN_NEWSCORE)->SetCharSpacing(8);
		GET_SPRITE(SPRITEID_FONT_MAIN_NEWSCORE)->DrawAnsiTextFormat(_gp, scoreX, scoreY, _VCENTER | _HCENTER, "%d", _game->_user._hight_score);
		_game->GetSpriteVer(SPRITEID_FONT_MAIN_NEWSCORE)->SetCharSpacing(charspacing);
		_gp->ClearColorMask();
	}
	else if (_typeDialog == CDialog::enumTypeDialog::CONFIRM_RATED)
	{
		int board_x = GET_SPRITE(SPRITEID_HUD_RATE)->GetFModuleX(0, 0);
		int board_w = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameWidth(0);
		int board_h = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameHeight(0) * 0.2f;
		_gp->SetColorMask(0xffffffff);
		_gp->FillRect(board_x, y + k_SCREEN_HEIGHT / 2 * 0.79f, board_w, board_h);
		_gp->ClearColorMask();
		GET_SPRITE(SPRITEID_HUD_RATE)->DrawFrame(_gp, 9, x, y);
		
	}
}

void CDialog::SetStateTrans(int type)
{
	_typeTrans = type;
	_state = 1;
}
bool CDialog::UpdateTrans()
{
	switch (_typeTrans)
	{
	case CDialog::enumTypeTrans::_MOVE_IN_RIGHT_:
		switch (_state)
		{
		case 1:
			_color -= 0x050505;
			if (_color <= 0xaaaaaa)
			{
				_color = 0xaaaaaa;
			}
			_opacity -= 5;
			_opacityScreen += 3;
			if (_opacityScreen > 70)
			{
				_opacityScreen = 70;

			}
			_offX += 35;
			if (_offX >= 35)
			{
				_offX = 35;
				_state = 2;
				if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE)
				{
					//PLAY_SFX(SFX_RESULT_SCREEN_HIGHSCORE);
				}
				else if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_GIFT)
				{
					//PLAY_SFX(SFX_DAILY_GIFT);
				}
				else
				{
					//PLAY_SFX(SFX_POP_UP);
				}
			}
			break;

		case 2:
			_offX -= 7;
			if (_offX <= -10)
			{
				_offX = -10;
				_state = 3;
			}
			break;

		case 3:
			_offX += 3;
			if (_offX >= 0)
			{
				_offX = 0;
				_state = 0;
			}
			break;
		}
		break;

	case CDialog::enumTypeTrans::_MOVE_OUT_RIGHT_:
		switch (_state)
		{
		case 1:
			_color += 0x050505;
			if (_color >= 0xffffff)
			{
				_color = 0xffffff;
			}
			_opacity += 5;
			if (_opacity > 100)
			{
				_opacity = 100;
			}
			_opacityScreen -= 3;
			_offX += 35;
			if (_offX >= k_SCREEN_WIDTH)
			{
				_offX = k_SCREEN_WIDTH;
				_opacity = 100;
				_typeDialog = CDialog::enumTypeDialog::DIALOG_NONE;
			}
			break;
		}
		break;
	case CDialog::enumTypeTrans::ZOOM_IN:
		switch (_state)
		{
		case 1:
			_opacityScreen += 5;
			if (_opacityScreen >= 60)
			{
				_opacityScreen = 60;
			}
			_zoomX += 0.1;
			if (_zoomX >= 1.2)
			{
				_zoomX = 1.2;
				_state = 2;
			}
			break;

		case 2:
			_zoomX -= 0.05;
			if (_zoomX <= 0.9)
			{
				_zoomX = 0.9;
				_state = 3;
			}
			break;

		case 3:
			_zoomX += 0.03;
			if (_zoomX >= 1)
			{
				_zoomX = 1;
				_state = 4;
			}
			break;

		case 5:
			_zoomX -= 0.1;
			_opacityScreen -= 5;
			if (_zoomX <= 0)
			{
				_zoomX = 0;
				_opacityScreen = 0;
				_state = 6;
				_typeDialog = CDialog::enumTypeDialog::DIALOG_NONE;
			}
			break;

		case 6:
		case 7:
			_state++;
			break;
		}
		break;
	}
	return true;
}
bool CDialog::Update()
{
	if (_typeDialog == CDialog::enumTypeDialog::DIALOG_NONE)
	{
		return false;
	}
	//ButtonProcessing();
	UpdateTouch();
	if (_typeDialog == CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE)
	{

		_timeEffect--;
		if (_timeEffect <= 0)
		{
			if (CMath::RANDOM(1, 5) <= 2)
			{
				//PLAY_SFX(CMath::RANDOM(SFX_FIREWORK, SFX_FIREWORK_4));
			}
			_timeEffect = CMath::RANDOM(10, 15);
		}

	}
	return UpdateTrans();
}

bool CDialog::ButtonProcessing()
{
	bool ret = false;
	int index = -1;
	for (int i = 0; i < 2; i++)
	{
		if (_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{

		if (_button[index].NeedAction())
		{
			AcctionButton(index);

			for (int i = 0; i < 2; i++)
			{
				if (_button[i]._state != BUTTON_STATE_LOCK && _button[i]._state != BUTTON_STATE_HIDE)
				{
					_button[i].SetState(BUTTON_STATE_NORMAL);
				}
			}

		}
		return true;
	}
	return false;
}
bool CDialog::UpdateTouch()
{

	bool press = false;
	int index = -1;
	bool ret = false;
	for (int i = 0; i < 2; i++)
	{
		if (_button[i]._state == BUTTON_STATE_ACTIVE)
		{
			ret = true;
			index = i;
			break;
		}
	}
	if (ret)
	{
		if (_button[index].NeedAction())
		{
			AcctionButton(index);
			if (_button[index]._state != BUTTON_STATE_LOCK && _button[index]._state != BUTTON_STATE_HIDE)
			{
				_button[index].SetState(BUTTON_STATE_NORMAL);
			}
		}
	}

	for (int i = GAME()->_num_touches - 1; i >= 0; i--)
	{
		for (int touch = 0; touch < 2; touch++)
		{
			int x = _button[touch].TZ_GetX() - 20;
			int y = _button[touch].TZ_GetY() - 20;
			int w = _button[touch].TZ_GetWidth() + 40;
			int h = _button[touch].TZ_GetHeight() + 40;
			Button *button = &_button[touch];
			//PDEBUG("\n button[%d]_state: %d",touch,button->_state);
			if (button->_state == BUTTON_STATE_LOCK || button->_state == BUTTON_STATE_HIDE)
			{

				continue;
			}
			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, x, y, w, h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					//PDEBUG("\n button[%d]_state: %d", touch, button->_state);
					if (button->_touch_id == -1)
					{
						button->SetState(BUTTON_STATE_SELECT);
						button->_touch_id = TOUCHS(i)->_id;
						press = true;
						PLAY_SFX(SFX_BUTTON);
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					button->SetState(BUTTON_STATE_SELECT);
					button->_touch_id = TOUCHS(i)->_id;
					press = true;
				}
				else if (TOUCHS(i)->_state == TOUCH_UP)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_ACTIVE);
						button->_touch_id = -1;
						press = true;
					}
				}
			}
			else
			{
				if (TOUCHS(i)->_state == TOUCH_UP)
				{

					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
				else if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					if (button->_touch_id == TOUCHS(i)->_id)
					{
						button->SetState(BUTTON_STATE_NORMAL);
						button->_touch_id = -1;
					}
				}
			}

			int otherStar_x = GET_SPRITE(SPRITEID_HUD_RATE)->GetFModuleX(3, 0);
			int otherStar_y = GET_SPRITE(SPRITEID_HUD_RATE)->GetFModuleY(3, 0) - 10;
			int otherStar_w = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameWidth(3) + 40 * 0.6f;
			int otherStar_h = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameHeight(3) + 30;

			//G()->FillRect(otherStar_x, otherStar_y, otherStar_w * enumStarRateStatus::RATE_STATE_LIKED, otherStar_h);

			int endStart_w = GET_SPRITE(SPRITEID_HUD_RATE)->GetFrameWidth(3);

			if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, otherStar_x, otherStar_y, otherStar_w * enumStarRateStatus::RATE_STATE_LIKED, otherStar_h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					PDEBUG("\n otherStar_y:%d", otherStar_y);
					//PDEBUG("\n Inside TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, otherStar_w * enumStarRateStatus::RATE_STATE_HATED);
					if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_HATED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_HATED;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_DISLIKED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_DISLIKED;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_OK))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_OK;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_LIKED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_LIKED;
					}
					isRated = true;
					PDEBUG("\n stateRateGame = %d", stateRateGame);
				}
				if (TOUCHS(i)->_state == TOUCH_DRAG)
				{
					//PDEBUG("\n TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, otherStar_w * enumStarRateStatus::RATE_STATE_HATED);
					if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_HATED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_HATED;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_DISLIKED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_DISLIKED;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_OK))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_OK;
					}
					else if (TOUCHS(i)->x < otherStar_x + (otherStar_w * enumStarRateStatus::RATE_STATE_LIKED))
					{
						stateRateGame = enumStarRateStatus::RATE_STATE_LIKED;
					}
					isRated = true;
					PDEBUG("\n stateRateGame = %d", stateRateGame);
				}
			}
			else if (CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, otherStar_x + otherStar_w * enumStarRateStatus::RATE_STATE_LIKED - 35 + 20, otherStar_y, endStart_w + 40, otherStar_h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					PDEBUG("\n outside TOUCHS(i)->x: %d   otherStar_w * enumStarRateStatus::RATE_STATE_HATED:%d", TOUCHS(i)->x, otherStar_w * enumStarRateStatus::RATE_STATE_HATED);
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

			int modal_x = GET_SPRITE(SPRITEID_HUD_MENU)->GetFModuleX(1, 0) + k_SCREEN_WIDTH / 2;
			int modal_y = GET_SPRITE(SPRITEID_HUD_MENU)->GetFModuleCenterY(1, 0) + k_SCREEN_HEIGHT / 2;
			int modal_w = GET_SPRITE(SPRITEID_HUD_MENU)->GetFrameWidth(1);
			int modal_h = GET_SPRITE(SPRITEID_HUD_MENU)->GetFrameHeight(1);

			if (!CHECK_POINT_IN_RECT(TOUCHS(i)->x, TOUCHS(i)->y, modal_x, modal_y, modal_w, modal_h))
			{
				if (TOUCHS(i)->_state == TOUCH_DOWN)
				{
					if (_typeDialog == CDialog::enumTypeDialog::CONFIRM_RATED)
					{
						_state = 5;
					}

				}
			}

		}
	}
	return press;
}
bool CDialog::TouchButton(int touchX, int touchY, int touchState, int touchID)
{
	bool pressed = false;
	if (!ButtonProcessing())
	{

		for (int touch = 0; touch < 2; touch++)
		{
			if (_button[touch]._state == BUTTON_STATE_LOCK || _button[touch]._state == BUTTON_STATE_HIDE)
			{
				continue;
			}

			int x = _button[touch].TZ_GetX() - 50;
			int y = _button[touch].TZ_GetY() - 70;
			int w = _button[touch].TZ_GetWidth() + 100;
			int h = _button[touch].TZ_GetHeight() + 140;

			Button *button = &_button[touch];

			if (CHECK_POINT_IN_RECT(touchX, touchY, x, y, w, h))
			{
				if (touchState == TOUCH_DOWN)
				{

					pressed = true;
				}
				else if (touchState == TOUCH_DRAG)
				{
					pressed = true;
				}
				else if (touchState == TOUCH_UP)
				{
					button->SetState(BUTTON_STATE_ACTIVE);
				}
			}
			else
			{

			}
		}
	}
	return pressed;
}


extern "C" void	GoToReviewInItune();

char _stringFeed[512];
void CDialog::WaiteProcessing()
{
	if (_typeDialog == CDialog::enumTypeDialog::DIALOG_NONE)
	{
		return;
	}
	if (_typeTrans == CDialog::enumTypeTrans::_WAITE_)
	{

		switch (_typeDialog)
		{
		case CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING:
			//if (_state == 2)
		{
			if (_game->_isWaiting)
			{
				int timeout = time(NULL) - GAME()->_timeOutGame;
				PDEBUG("\n timeout:%d", timeout);
				if (timeout >= 3)
				{
					//game->_user._isShowNotifi = true;
					_game->_isWaiting = false;
					//game->_user._isRating = true;
					SetStateTrans(CDialog::enumTypeTrans::ZOOM_IN);
					_state = 5;
					//game->_timeShowNotify = 20;
					//game->_user._countShowRate = 20;

					_game->_user.DataSave();;
				}
			}
		}
		break;
		case CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE:
			if (!UpdateShare())
			{
#ifdef MAC_OS || ANDROID_OS
				if (GAME()->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL)
				{
					SetStateTrans(CDialog::enumTypeTrans::ZOOM_IN);
					_state = 5;
				}
				else if (GAME()->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE)
				{
					SetStateTrans(CDialog::enumTypeTrans::ZOOM_IN);
					_state = 5;
					//game->_quest.AddCurrentValue(CQuest::enumObjective::Objective_6, 1);
				}
#else
				SetStateTrans(CDialog::enumTypeTrans::_MOVE_OUT_RIGHT_);
				//game->_quest.AddCurrentValue(CQuest::enumObjective::Objective_6, 1);
#endif
				_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_NONE;
			}
			break;
		}
	}

}
extern "C" void OS_BasicPostSocial(const char* feed);
void CDialog::AcctionButton(int index)
{

	switch (_typeDialog)
	{
	case CDialog::enumTypeDialog::COMFIRM_SHOW_NOTIFY:
		if (index == DIALOG_BUTTON_RATE_LATER)
		{
			//game->_user._isShowNotifi = false;
			_state = 5;
		}
		else if (index == DIALOG_BUTTON_RATE)
		{
			//game->_timeShowNotify = 0;
			//game->_user._isShowNotifi = true;
			_game->_user.DataSave();
			_state = 5;
		}
		break;
	case CDialog::enumTypeDialog::COMFIRM_SHOW_RATTING:

		if (index == DIALOG_BUTTON_RATE_LATER)
		{
			_game->countShowRateUs = COUNT_SHOW_RATE;
			_game->_user.DataSave();
			_state = 5;
			stateRateGame = CDialog::enumStarRateStatus::RATE_STATE_NONE;
		}
		else if (index == DIALOG_BUTTON_RATE)
		{
#ifdef MAC_OS
			SetStateTrans(CDialog::enumTypeTrans::_WAITE_);
			GAME()->_timeOutGame = time(NULL);
			GoToReviewInItune();
			game->_isWaiting = true;
#elif ANDROID
			/*SetStateTrans(CDialog::enumTypeTrans::_WAITE_);
			GAME()->_timeOutGame = time(NULL);
			OS_Rating();
			game->_isWaiting = true;*/
			if (isRated)
			{
				_game->_isWaiting = false;
				//_game->_user._isRating = true;
				_state = 5;
				isRated = false;
			}
#else
			PDEBUG("\n isRated ==================:%d", isRated);
			if (isRated)
			{
				//game->_isWaiting = false;
				//_game->_user._isRating = true;
				_state = 5;
				isRated = false;
				_game->_user.DataSave();
			}
#endif
		}
		break;

	case CDialog::enumTypeDialog::COMFIRM_SHOW_SHARE:
		if (index == DIALOG_BUTTON_SHARE_NEXT)
		{
			_state = 5;
		}
		else if (index == DIALOG_BUTTON_SHARE_YEAH)
		{
			SetStateTrans(CDialog::enumTypeTrans::_WAITE_);
			InitShare();
			//PLAY_SFX(SFX_SHARE);
		}
		break;
	}
}

#define SHARE_W		(735.0f)
#define SHARE_H		(736.0f)	

int _offShareX;
int _opaShare;
int _opaccapture;
void CDialog::InitShare()
{
	_stateShare = 1;
	SetStateTrans(CDialog::enumTypeTrans::_WAITE_);
	_opaShare = 0;
	_opaccapture = 80;
}
void CDialog::RenderShare(float x, float y, int opacity)
{
#ifdef _WINDOWS
	return;
#endif
	if (_stateShare == 0)
	{
		return;
	}


	_gp->SetColor(0xff000000);
	int opa = (_opaShare * 0.8)*opacity*0.01;
	if (opa >= 70)
	{
		opa = 70;
	}
	_gp->SetOpacity(opa);
	_gp->FillFullScreen(true);

	_gp->SetOpacity(_opaShare * opacity*0.01);

	x += k_SCREEN_WIDTH / 2 - (SHARE_W / 2);
	float offY = k_SCREEN_HEIGHT / 2 - (SHARE_H / 2);

	_gp->SetClip(x, y + offY, SHARE_W, SHARE_H);
	_gp->SetOpacity(_opaShare * opacity*0.01);
	//#if defined(ANDROID_OS)
	//	_game->GetSpriteVer(SPRITEID_OBJECT_SHAREHIGHSCORE)->DrawFrame(_gp, game->_bg_spriteid_current + 3, x, y + offY);
	//#else
	//	_game->GetSpriteVer(SPRITEID_OBJECT_SHAREHIGHSCORE)->DrawFrame(_gp, game->_bg_spriteid_current, x, y + offY);
	//#endif

		//score
	int stringX = x + k_SCREEN_WIDTH / 2;
	int stringY = y + 275 + offY;

	int charspacing = _game->GetSpriteVer(SPRITEID_FONT_MAIN_BESTSCORE)->_char_spacing;
	_game->GetSpriteVer(SPRITEID_FONT_MAIN_BESTSCORE)->SetCharSpacing(8);
	GET_SPRITE(SPRITEID_FONT_MAIN_BESTSCORE)->DrawAnsiTextFormat(G(), stringX, stringY, _HCENTER | _VCENTER, "%d", _game->_user._hight_score);
	_game->GetSpriteVer(SPRITEID_FONT_MAIN_BESTSCORE)->SetCharSpacing(charspacing);
	_gp->SetOpacity(opacity);
	_gp->ClearClip();

	if (_opaccapture > 0)
	{
		_opaccapture -= 2;
		if (_opaccapture <= 0)
		{
			_opaccapture = 0;
		}
	}
	_gp->SetColor(0xffffffff);
	_gp->SetOpacity(_opaccapture* opacity*0.01);
	_gp->FillFullScreen(true);
	_gp->SetOpacity(opacity);
}
bool CDialog::UpdateShare()
{
	if (_stateShare == 0)
	{
		return false;
	}

	switch (_stateShare)
	{
	case 1:
		_opaShare += 10;
		if (_opaShare >= 100)
		{
			_opaShare = 100;
			_stateShare = 2;
		}
		break;

	case 2:
		_opaShare += 1;
		if (_opaShare >= 140)
		{
			float mx = k_SCREEN_WIDTH / 2 - (SHARE_W / 2.0f);
			float my = k_SCREEN_HEIGHT / 2 - (SHARE_H / 2.0f);


			float offX = 0;
			float offY = 1;
			float offW = 0;
			float offH = 1;
			if (GAME()->_device._version == SupportVersion::RESVER_960_640)
			{
				offX = 60;
				offW = offX * 2;
				offY = 2;
				offH = 2;
				//my += 2;
			}
			else if (GAME()->_device._version == SupportVersion::RESVER_1024_768)
			{
				offX = 93;
				offW = offX * 2;
			}
			//game->_notShowPineAdds = true;
#ifdef MAC_OS
			PineSocial::SaveScreenToDocument("sharePicture", mx + offX, my + offY, SHARE_W - offW, SHARE_H - offH);
#endif
			_stateShare = 3;
		}
		break;

	case 3:
		_opaShare += 1;
		if (_opaShare >= 150)
		{
			_opaShare = 110;
			_stateShare = 4;

			sprintf(_stringFeed, "I scored %d in", _game->_user._hight_score);
#ifdef _WINDOWS
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL;
#elif MAC_OS
			PineSocial::ShareImageWithURLDocument(_stringFeed, "sharePicture");
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_PROCESSING;
#elif ANDROID_OS
			_game->_statusBasicPostSocial = CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE;
			GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_SHARE_SCREEN_SHOT_INTENT);
#endif
		}
		break;

	case 4:
		if (_game->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_COMPLETE
			|| _game->_statusBasicPostSocial == CGame::enumStatusPostSocial::POST_SOCIAL_STATE_CANCEL)
		{
			_opaShare -= 7;
			if (_opaShare <= 0)
			{
				_stateShare = 0;
			}
		}
		break;
	}
	return true;
}