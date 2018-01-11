#include "pch.h"
#include "../../GameCore/CGame.h"

void COffsetScroll::Init(float speed, float mul, float maxSpeed, float minSpeed)
{	
	_mulSpeed = mul;
	_speed = speed;	
	_mulSpeed = mul;
	_maxSpeed = maxSpeed;
	_minSpeed = minSpeed;
	_currentSpeed = 0;
	_touchID = -1;
	SetAccelSpeed(0.92f);
	SetState(0);    
}

void COffsetScroll::AddOffset(float offset)
{	
	_currentSpeed = offset*_speed*_mulSpeed;
	_direction = _currentSpeed;
	if (_currentSpeed > _maxSpeed)
	{
		_currentSpeed = _maxSpeed;
	}
	if (_currentSpeed < -_maxSpeed)
	{
		_currentSpeed = -_maxSpeed;
	}
	SetState(1);
}

void COffsetScroll::SetState(int state)
{
	_state = state;
	switch (_state)
	{
	case 2:

		break;
	}
}
bool COffsetScroll::IsScroll()
{
	return _currentSpeed != 0 || _state != 0;
}
void COffsetScroll::Update()
{
    if (_state == 2)
    {
		_currentSpeed *= _accel;
        if (CMath::ABSOLUTE_VALUE(_currentSpeed) <= _minSpeed)
        {
			_currentSpeed = 0;
			SetState(0);
        }
    }
}

void COffsetScroll::SetBeginScorll(int touchID)
{
	_touchID = touchID;
	
}
bool COffsetScroll::CheckTouch(int touchID)
{
	return _touchID == touchID;
}
void COffsetScroll::ClearTouchScroll()
{
	_touchID = -1;
}