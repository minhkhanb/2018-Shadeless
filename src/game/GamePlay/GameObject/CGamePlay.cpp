#include "pch.h"
#include "../../GameCore/CGame.h"

float _rateR, _rateG, _rateB;
void CGamePlay::InitScoreTime()
{
	for (int i = 0; i < MAX_SCORE_TIME; i++)
	{
		_scoreTime[i]._state = 0;
	}
}
void CGamePlay::AddScoreTime(int score)
{
	printf("\n =====");
	for (int i = 0; i < MAX_SCORE_TIME; i++)
	{
		if (_scoreTime[i]._state == 0)
		{
			_scoreTime[i]._time = 1;
			_scoreTime[i]._score = 0;
			_scoreTime[i]._state = 1;
			break;
		}		
	}
	for (int i = 0; i < MAX_SCORE_TIME; i++)
	{
		if (_scoreTime[i]._state == 1 && _scoreTime[i]._time<= 3*60)
		{			
			_scoreTime[i]._score += score;
			printf("\n score[%d] => %d", i, _scoreTime[i]._score);
		}
	}
}
void CGamePlay::UpdateScore()
{
	for (int i = 0; i < MAX_SCORE_TIME; i++)
	{
		if (_scoreTime[i]._state == 1 && _stateGame == CGamePlay::enumStateGame::GAME_DOWN_RADIUS)
		{
			_scoreTime[i]._time++;
		}
	}
}
int CGamePlay::GetScoreTime()
{
	int res = 0;
	for (int i = 0; i < MAX_SCORE_TIME; i++)
	{
		if (_scoreTime[i]._state == 1 && _scoreTime[i]._score > res)
		{
			res = _scoreTime[i]._score;
		}
	}
	return res;
}

void CGamePlay::Init()
{
	_game = GAME();
	_gp = G();
	_score = 0;
	_touchID = -1;
	_touchIDOld = -1;
	_ruleDes = CGamePlay::enumRuleCircle::CIRCLE_SIZE_MAX;
	_level = GetLevelDes(_score);	

	InitColorDes();
	_indexColor = GetIndexColor(GetColorDes(_game->_user._curColorUse, COLOR_DES_BG_COLOR));
	_bgColor = _colorDes[_indexColor];

	_beginGame = false;
	NewLocation(true);
	_stateScore = 0;	
	_timeTouch = 100;
	_stateTutorial = 0;
	_opacityScore = 0;
	_tapNoScore = 0;
	
}
float CGamePlay::ReadDesParam(int rule, int level, int param)
{	
	if (rule == CGamePlay::enumRuleCircle::CIRCLE_SIZE_MAX)
	{
		return GDS_Exporter::level_des_1[level*TOTAL_LEVEL_DES_1 + param];
	}
	else if (rule == CGamePlay::enumRuleCircle::CIRCLE_SIZE_NORMAL)
	{
		return  GDS_Exporter::level_des_2[level*TOTAL_LEVEL_DES_2 + param];
	}
	else if (rule == CGamePlay::enumRuleCircle::CIRCLE_SIZE_MIN)
	{
		return  GDS_Exporter::level_des_3[level*TOTAL_LEVEL_DES_3 + param];
	}
	return 0;
}
const COLOR CGamePlay::GetColorDes(int i, int param)
{
	return GDS_Exporter::color_des[i*TOTAL_COLOR_DES + param];
}
void CGamePlay::InitColorDes()
{
	int maxR, maxG, maxB;
	int minR, minG, minB;
	ConvertHEXtoRGR(GetColorDes(_game->_user._curColorUse, COLOR_DES_MAX_COLOR), maxR, maxG, maxB);
	ConvertHEXtoRGR(GetColorDes(_game->_user._curColorUse, COLOR_DES_MIN_COLOR), minR, minG, minB);

	_rateR = (minR - maxR) / (float)(MAX_COLOR_DES - 1);
	_rateG = (minG- maxG ) / (float)(MAX_COLOR_DES - 1);
	_rateB = (minB - maxB ) / (float)(MAX_COLOR_DES - 1);

	for (int i = 0; i < MAX_COLOR_DES; i++)
	{
		_colorDes[i] = ConvertRGBtoHEX(minR - i*_rateR, minG - i*_rateG, minB - i*_rateB);
	}
}
void CGamePlay::SetNewColor()
{
	InitColorDes();
	_indexColor = GetIndexColor(GetColorDes(_game->_user._curColorUse, COLOR_DES_BG_COLOR));
	_bgColor = _colorDes[_indexColor];
	
	_indexColor = GetIndexColor(GetColorDes(_game->_user._curColorUse, COLOR_DES_CIRCLE_COLOR));
	_circle._color = _colorDes[_indexColor];
}
int CGamePlay::GetIndexColor()
{
	return CMath::RANDOM(0, MAX_COLOR_DES - 1);
}
int CGamePlay::GetIndexColor(COLOR color)
{
	for (int i = MAX_COLOR_DES -1; i >= 0; i--)
	{
		if (color < _colorDes[i])
		{
			return i;
		}
	} 
	return -1;
}

int CGamePlay::GetIndexColor(int rand, bool foreGand)
{
	int _up_down = CMath::RANDOM(0, 1) ? _TOP : _BOTTOM;
	int rateUp = _indexColor - rand;
	int rateDown = _indexColor + rand;
	if (foreGand)
	{
		if (rateDown <= MAX_COLOR_DES - 1)
		{
			return rateDown;
		}
		else
		{			
			return MAX_COLOR_DES - 1;
		}
	}
	else
	{
		if (_up_down == _TOP)
		{
			if (rateUp >= 0)
			{
				_rangeRate = _TOP;
				return  rateUp;
			}
			else
			{
				if (rateDown <= MAX_COLOR_DES - 1)
				{
					_rangeRate = _BOTTOM;
					return rateDown;
				}
				else
				{
					return 0;
				}
			}
		}
		else if (_up_down == _BOTTOM)
		{
			if (rateDown <= MAX_COLOR_DES - 1)
			{
				_rangeRate = _BOTTOM;
				return rateDown;
			}
			else
			{
				if (rateUp >= 0)
				{
					_rangeRate = _TOP;
					return  rateUp;
				}
				else
				{
					return MAX_COLOR_DES - 1;
				}
			}
		}
	}
	return -1;
}
void CGamePlay::NewLocation(bool isStart)
{
	_touchIDOld = _touchID;
	_level = GetLevelDes(_score);

	//positon & radius
	if (isStart)
	{
		_circle._x = k_SCREEN_WIDTH / 2;
		_circle._y = k_SCREEN_HEIGHT / 2;
		_circle._radius = 350;
	}
	else
	{
		GetNewPosision(_circle._x, _circle._y);
		_circle._radius = _circle._oldRaidus + ReadDesParam(_ruleDes, _level, LEVEL_DES_1_SPEED_DOWN)*10;
	}
	_circle._oldRaidus = _circle._radius;
	_maxRadius = GetMaxRange(0 + _game->_pointLeftTop.X, 0, _game->_screenWidth, _game->_screenHeight, _circle._x, _circle._y);
	_percent = _circle._radius / _maxRadius;
	_maxRadiusGlow = 350.0f;
	_percentGlow = _circle._radius / _maxRadiusGlow;
	if (_percentGlow*100 >= 70)
	{
		_ruleDes = CGamePlay::enumRuleCircle::CIRCLE_SIZE_MAX;
	}
	else if(_percentGlow*100 >= 40)
	{
		_ruleDes = CGamePlay::enumRuleCircle::CIRCLE_SIZE_NORMAL;
	}
	else
	{
		_ruleDes = CGamePlay::enumRuleCircle::CIRCLE_SIZE_MIN;
	}

	//color
	int min = ReadDesParam(_ruleDes, _level, LEVEL_DES_1_MIN_RANGE);
	int max = ReadDesParam(_ruleDes, _level, LEVEL_DES_1_MAX_RANGE);
	_rangeDes = CMath::RANDOM(min, max);	

	if (isStart)
	{
		_indexColor = GetIndexColor(GetColorDes(_game->_user._curColorUse, COLOR_DES_CIRCLE_COLOR));
	}
	else
	{		
		_indexColor = GetIndexColor(_rangeDes);		
	}	
	if (_rangeDes == 1 && !isStart && _indexColor != 0 && _indexColor != MAX_COLOR_DES - 1)
	{
		int R, G, B;
		ConvertHEXtoRGR(_colorDes[_indexColor], R, G, B);
		if (_rangeRate == _TOP)
		{
			R += _rateR * 1;
			G += _rateG * 1;
			B += _rateB * 1;
			if (R > 255) R = 255;
			if (G > 255) G = 255;
			if (B > 255) B = 255;
		}
		else if(_rangeRate == _BOTTOM)
		{
			R -= _rateR * 1;
			G -= _rateG * 1;
			B -= _rateB * 1;
			if (R < 0) R = 0;
			if (G < 0) G = 0;
			if (B < 0) B = 0;
		}		
 		_circle._color = ConvertRGBtoHEX(R, G, B);
	}
	else
	{
		_circle._color = _colorDes[_indexColor];
	}
	

	_circle._upRadius = ReadDesParam(_ruleDes, _level, LEVEL_DES_1_SPEED_UP);
	_circle._downRadius = ReadDesParam(_ruleDes, _level, LEVEL_DES_1_SPEED_DOWN);
	SetState(CGamePlay::enumStateGame::GAME_START);

	
}
void CGamePlay::GetNewPosision(int oldX, int oldY)
{
	int posX = CMath::RANDOM(0, k_SCREEN_WIDTH - 0);
	int posY = CMath::RANDOM(0, k_SCREEN_HEIGHT - 0);
	int distance = _circle._oldRaidus < 100 ? 100 : _circle._oldRaidus;
	int numNew = 0;
	do
	{		
		printf("\n ============= GetNewPosision ============= ");
		numNew++;
		if (_ruleDes == CGamePlay::enumRuleCircle::CIRCLE_SIZE_MIN)
		{			
			posX = CMath::RANDOM(distance, k_SCREEN_WIDTH - distance * 2);
			posY = CMath::RANDOM(distance, k_SCREEN_HEIGHT - distance * 2);
		}
		else
		{
			posX = CMath::RANDOM(0, k_SCREEN_WIDTH - 0);
			posY = CMath::RANDOM(0, k_SCREEN_HEIGHT - 0);
		}	
	} while (CheckCircleOverlapCircle(posX, posY, distance, oldX, oldY, distance) && numNew < 10);

	_circle._x = posX;
	_circle._y = posY;
}
void CGamePlay::Render(float x, float y)
{
	//render bg
	_gp->SetColor(_bgColor);
	_gp->FillRect(0 + _game->_pointLeftTop.X, 0, _game->_screenWidth, _game->_screenHeight, true);

	//render circle;
	if (_percent >0)
	{
		_gp->SetColor(_circle._color);
		_gp->FillCircle(_circle._x, _circle._y, _circle._radius, 20, true);

		if (_stateGame == CGamePlay::enumStateGame::GAME_UP_RADIUS)
		{
			int r, g, b;
			int index = GetIndexColor(20, true);
			
			_gp->SetScale(_percentGlow, _percentGlow, _circle._x, _circle._y);
			_game->GetSpriteVer(SPRITEID_OBJECT_CIRCLE)->DrawFrame(_gp, 0, _circle._x, _circle._y);
			_gp->ClearScale(); 			
		}	
	}

	
	_gp->SetOpacity(_opacityScore);
	_gp->SetScale(1.5, 1.5, k_SCREEN_WIDTH / 2, 70);
	_game->GetSpriteVer(SPRITEID_FONTS_BEST_SCORE)->DrawAnsiTextFormat(_gp, k_SCREEN_WIDTH / 2, 70, _HCENTER | _VCENTER, "%d", _score);	
	_gp->ClearScale();
	_gp->SetOpacity(100);

	if (_stateGame == CGamePlay::enumStateGame::GAME_BONUS_SCORE)
	{
		_gp->SetOpacity(_opaBonus);
		_gp->SetScale(_zoomBonus, _zoomBonus, k_SCREEN_WIDTH / 2, POS_Y(180));
		if (_rangeDes == 1)
		{
			_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 28, k_SCREEN_WIDTH / 2, POS_Y(180));
		}
		else
		{
			_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 24, k_SCREEN_WIDTH / 2, POS_Y(180));
		}	
		_gp->SetColorMask(CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_TEXT_COLOR));
		if (_rangeDes == 1)
		{
			_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 29, k_SCREEN_WIDTH / 2, POS_Y(180));
		}
		else
		{
			_game->GetSpriteVer(SPRITEID_HUD_MAINMENU)->DrawFrame(_gp, 25, k_SCREEN_WIDTH / 2, POS_Y(180));
		}
		_gp->ClearColorMask();		
		_gp->ClearScale();
		_gp->SetOpacity(100);
	}

	if (_game->_user._stepTutorial<100 && _stateTutorial!=0)
	{
		RenderTutorial(x, y);
		UpdateTutorial();
	}

	//_game->GetSpriteVer(SPRITEID_FONTS_BEST_SCORE)->DrawAnsiTextFormat(_gp, 50, k_SCREEN_HEIGHT - 50, _HCENTER | _VCENTER, "%d", _rangeDes);
}

void CGamePlay::Update()
{
	if (!_beginGame)
	{
		return;
	}
	switch (_stateGame)
	{
	case CGamePlay::enumStateGame::GAME_START:
		SetState(CGamePlay::enumStateGame::GAME_DOWN_RADIUS);
		break;

	case CGamePlay::enumStateGame::GAME_DOWN_RADIUS:
		_circle._radius -= _circle._downRadius;
		_circle._oldRaidus = _circle._radius;
		if ((_percent*100) <= 0)
		{
			SetState(CGamePlay::enumStateGame::GAME_OVER);
		}
		break;

	case CGamePlay::enumStateGame::GAME_UP_RADIUS:
		_circle._radius += _circle._upRadius;
		if ((_percent * 100) >= 100)
		{
			SetState(CGamePlay::enumStateGame::GAME_START_NEW);
		}		
		break;

	case CGamePlay::enumStateGame::GAME_OVER:	
		_circle._radius += _speedRadius;
		if (_circle._radius >= 350)
		{
			_circle._radius = 350;
		}
		_opacityScore -= 5;
		if (isMoveToTarget(_circle._x, _circle._y,k_SCREEN_WIDTH/2,k_SCREEN_HEIGHT/2, _speedMove)&& _circle._radius >= 350)
		{
			SetState(CGamePlay::enumStateGame::GAME_END);
		}
		break;

	case CGamePlay::enumStateGame::GAME_BONUS_SCORE:
		switch (_stateBonus)
		{
		case 1:
			_opaBonus += 5;
			_zoomBonus -= 0.3;
			if (_zoomBonus <= 0.95)
			{
				_zoomBonus = 0.95;
				_stateBonus = 2;
				_game->PlaySFX(SFX_SHADELESS);
			}
			break;

		case 2:
			_zoomBonus += 0.02;
			if (_zoomBonus >= 1.05)
			{
				_zoomBonus = 1.05;
				_stateBonus = 3;
			}
			break;

		case 3:
			_zoomBonus -= 0.01;
			if (_zoomBonus <= 1)
			{
				_zoomBonus = 1;
				_stateBonus = 4;
				_timeDelay = 0;
			}
			break;

		case 4:
			_timeDelay++;
			if (_timeDelay >= 4)
			{
				_timeDelay = 0;
				_score++;
				_game->PlaySFX(SFX_SCORE);
				if (_score >= _nextScore)
				{
					_score = _nextScore;
					_stateBonus = 5;
				}
			}			
			break;

		case 5:
			_opaBonus -= 5;
			_zoomBonus += 0.3;
			if (_opaBonus <=0)
			{
				_stateBonus = 0;
				//_bgColor = _circle._color;
				_bgColor = _colorDes[_indexColor];
				NewLocation();
			}
			break;
		}
		break;
	}
	_percent = _circle._radius / _maxRadius;
	_percentGlow = _circle._radius / _maxRadiusGlow;
	if (_percent<0)
	{
		_percent = 0;
	}
	if (_stateScore ==1)
	{
		_opacityScore += 5;
		if (_opacityScore>100)
		{
			_opacityScore = 100;
			_stateScore = 2;
		}
	}
	UpdateScore();
}

void CGamePlay::SetState(BYTE state)
{
	_stateGame = state;
	switch (state)
	{
	case CGamePlay::enumStateGame::GAME_BONUS_SCORE:
		_stateBonus = 1;
		_zoomBonus = 5;
		_opaBonus = 0;
		if (_rangeDes == 1)
		{
			_nextScore = _score + 20;
		}
		else
		{
			_nextScore = _score + 10;
		}
		break;

	case CGamePlay::enumStateGame::GAME_END:		
		_game->_gameStateGamePlay._mainMenu.InitResult_Logo(_score, false);
		_game->_gameStateGamePlay._status = CGameStateGamePlay::enumStatusState::STATE_MENU;
		Init();
		_game->_user.DataSave();
		break;

	case CGamePlay::enumStateGame::GAME_OVER:
		_timeDelay = 30;
		_game->_user.DataSave();
		_speedMove = CMath::DISTANCE(_circle._x, _circle._y, k_SCREEN_WIDTH / 2, k_SCREEN_HEIGHT / 2) / 40.0f;		
		_speedRadius = 350 / 40.0f;
		if (_speedMove<1)
		{
			_speedMove = 1;
		}
		_bgColor = CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_BG_COLOR);
		_circle._color = CGamePlay::GetColorDes(_game->_user._curColorUse, COLOR_DES_CIRCLE_COLOR);
		break;

	case CGamePlay::enumStateGame::GAME_DOWN_RADIUS:
		if (_stateScore == 0)
		{
			_stateScore = 1;
		}
		break;

	case CGamePlay::enumStateGame::GAME_START_NEW:
		_score++;
		_game->PlaySFX(SFX_SCORE);
		
		if (_rangeDes <= 2)
		{
			AddScoreTime(11);
			SetState(CGamePlay::enumStateGame::GAME_BONUS_SCORE);
		}
		else
		{
			AddScoreTime(1);
			_bgColor = _circle._color;
			_bgColor = _colorDes[_indexColor];
			NewLocation();
		}
		break;

	case CGamePlay::enumStateGame::GAME_UP_RADIUS:
		if (_tapNoScore > 5)
		{
			_game->_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_Cheater);
		}
		_tapNoScore = 0; 
		_game->PlaySoundButton();
		break;
	}
}

int CGamePlay::GetLevelDes(int score)
{
	for (int i = 0; i < TOTAL_LEVEL_DES_1_ROW; i++)
	{
		int scoreDes = ReadDesParam(_ruleDes, i, LEVEL_DES_1_SCORE);
		if (score <= scoreDes)
		{
			return i;
		}
	}
	return TOTAL_LEVEL_DES_1_ROW - 1;
}

void CGamePlay::UpdateTouch()
{    
	_timeTouch++;
   
	for (int i = 0; i < _game->_num_touches; i++)
	{
		if (_game->_touches[i]->_flag)
		{
			switch (_game->_touches[i]->_state)
			{
			case TOUCH_DOWN:
				if ((_touchID == -1 || _touchID == _game->_touches[i]->_id)
					&& (_touchIDOld == -1 || _touchID != _touchIDOld))
				{
					if(_game->_user._stepTutorial<100)
					{		
						if (_stateTutorial == 2)
						{
							_touchID = _game->_touches[i]->_id;
						}
					}
					else if (CHECK_POINT_IN_CIRCLE(_game->_touches[i]->x, _game->_touches[i]->y, _circle._x, _circle._y, (_circle._radius > 100) ? _circle._radius : 100) && _timeTouch > 20)
					{
						if (_stateGame == CGamePlay::enumStateGame::GAME_DOWN_RADIUS || _stateGame == CGamePlay::enumStateGame::GAME_START)
						{
                             printf("\n_timeTouch = %d", _timeTouch);
							_timeTouch = 0;							
							_touchID = _game->_touches[i]->_id;
							SetState(CGamePlay::enumStateGame::GAME_UP_RADIUS);

							if (!_beginGame)
							{
								_beginGame = true;
							}
						}
					}
					else
					{
                        _touchID = _game->_touches[i]->_id;
					}
				}
				break;

			case TOUCH_DRAG:

				break;

			case TOUCH_UP:
				if (_touchID == _game->_touches[i]->_id)
				{
					if (_game->_user._stepTutorial<100 && _stateTutorial == 2)
					{
						_stateTutorial = 3;
						_game->PlaySoundButton();
					}
					_touchID = -1;
					_touchIDOld = -1;
                    
                    if (!CHECK_POINT_IN_CIRCLE(_game->_touches[i]->x, _game->_touches[i]->y, _circle._x, _circle._y, (_circle._radius > 100) ? _circle._radius : 100))
                    {
                        _tapNoScore++;
                    }
				}
				break;
			}
		}
	}
	if (_game->CanUseTouch())
	{
		_touchID = -1;
		_touchIDOld = -1;
	}
}

float CGamePlay::GetMaxRange(float x, float y, float w, float h, int cX, int cY)
{
	PinePoint point[4];
	//top_left
	point[0].X = x;
	point[0].Y = y;

	//top_right
	point[1].X = x + w;
	point[1].Y = y;

	//bottom_left
	point[2].X = 0;
	point[2].Y = y + h;

	//bottom_right
	point[3].X = x + w;
	point[3].Y = y + h;

	float max = 0;
	for (int i = 0; i < 4; i++)
	{
		float range = CMath::DISTANCE(cX,cY, point[i].X, point[i].Y);
		if (range > max)
		{
			max = range;
		}
	}
	return max;
}

COLOR CGamePlay::ConvertRGBtoHEX(int R, int G, int B)
{
	return ((255 & 0xff) << 24) + ((R & 0xff) << 16) + ((G & 0xff) << 8) + (B & 0xff);
}
void CGamePlay::ConvertHEXtoRGR(COLOR color, int &R, int &G, int &B)
{
	R = ((color >> 16) & 0xFF);  // Extract the RR byte
	G = ((color >> 8) & 0xFF);   // Extract the GG byte
	B = ((color)& 0xFF);		// Extract the BB byte	
}

bool CGamePlay::CheckRectOverlapCircle(int x, int y, int w, int h, int cX, int cY, int R)
{
	float px = cX;
	float py = cY;

	if (px < x)
		px = x;
	else if (px >  x + w)
		px = x + w;

	if (py < y)
		py = y;
	else if (py > y + h)
		py = y + h;

	float dx = cX - px;
	float dy = cY - py;

	return (dx*dx + dy*dy) <= R*R;
}

bool CGamePlay::CheckCircleFillFullRect(int x, int y, int w, int h, int cX, int cY, int R, int over)
{
	float dd_1 = CMath::DistancePOW2(cX, cY, x, y);
	float dd_2 = CMath::DistancePOW2(cX, cY, x + w, y);
	float dd_3 = CMath::DistancePOW2(cX, cY, x + w, y + h);
	float dd_4 = CMath::DistancePOW2(cX, cY, x, y + h);
	float range = R*R - over*over;
	return  (range > dd_1 && range > dd_2 && range > dd_3 && range > dd_4);
}

bool CGamePlay::CheckCircleOverlapCircle(int cX1, int cY1, int R1, int cX2, int cY2, int R2)
{
	return CMath::DistancePOW2(cX1, cY1, cX2, cY2) < ((R1*R1) + (R2 * R2));
}
bool CGamePlay::isMoveToTarget(float &x, float &y, float targetX, float targetY, float speed)
{
	bool ret = false;
	float distance = CMath::DistancePOW2(targetX, targetY, x, y);
	if (distance < (speed * speed))
	{
		x = targetX;
		y = targetY;
		ret = true;
	}
	else
	{
		float angle_move = CMath::ANGLE(targetX, targetY, x, y) - 180;
		float vy = speed*MSIN(angle_move);
		float vx = speed*MCOS(angle_move);
		x += vx;
		y += vy;

	}
	return ret;
}

int _posHandX;
void CGamePlay::SetTutorial(int step)
{
	_offTutorial = 200;
	_stateTutorial = 1;
	_step = step;
	_game->_user._stepTutorial = step;
	if (step == 0 || step==1)
	{
		_posHandX = k_SCREEN_WIDTH + 50 - _game->_pointLeftTop.X;
	}
}
void CGamePlay::RenderTutorial(float x, float y)
{
	if (_step == 4)
	{
		_game->GetSpriteVer(SPRITEID_OBJECT_CIRCLE)->DrawFrame(_gp, 6, k_SCREEN_WIDTH / 2 + x, y + k_SCREEN_HEIGHT - POS_Y(140) + _offTutorial);
	}
	else
	{
		_game->GetSpriteVer(SPRITEID_OBJECT_CIRCLE)->DrawFrame(_gp, 1 + _step, k_SCREEN_WIDTH / 2 + x, y + k_SCREEN_HEIGHT - POS_Y(140) + _offTutorial);
	}	
	_game->GetSpriteVer(SPRITEID_OBJECT_CIRCLE)->DrawFrame(_gp, 5,  x + _posHandX, y + k_SCREEN_HEIGHT/2);
}
void CGamePlay::UpdateTutorial()
{
	switch (_stateTutorial)
	{
	case 1:
		_offTutorial -= 20;
		if (_offTutorial <= 0)
		{
			_stateTutorial = 2;
		}
		if (_step == 0 || _step == 1)
		{
			_posHandX -= 30;
			if (_posHandX <= 350 - _game->_pointLeftTop.X)
			{
				_posHandX = 350 - _game->_pointLeftTop.X;
			}
		}
		break;

	case 3:
		_offTutorial += 20;
		_posHandX += 30;
		if (_offTutorial >= 500)
		{
			if (_step == 4)
			{
				_stateTutorial = 0;
				_game->_user._stepTutorial = 100;
				_beginGame = true;
				_game->_achievement.SetCompleteAchivement(CAchievement::enumAchiveID::S_GoodStart);
				_game->_user.DataSave();
			}
			else
			{
				if (_step == 0)
				{
					_stateTutorial = 4;
				}
				else if (_step == 1)
				{
					_stateTutorial = 5;
				}
				else
				{
					SetTutorial(_game->_user._stepTutorial + 1);
				}
			}
		}
		break;

	case 4:
		_circle._radius -= 1;
		if (_circle._radius <= 250)
		{
			_circle._radius = 250;
			SetTutorial(_game->_user._stepTutorial + 1);
		}
		break;

	case 5:
		_circle._radius += 5;
		if (_circle._radius >= 350)
		{
			_circle._radius = 350;
			SetTutorial(_game->_user._stepTutorial + 1);
		}
		break;
	}
}