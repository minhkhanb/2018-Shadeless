//quoc.nguyen
#if !_COFFSET_SCROLL_BAR_H
#define _COFFSET_SCROLL_BAR_H 1
class COffsetScroll
{
public:

	float _currentSpeed;
	float _speed;	
	float _accel;
	float _mulSpeed;
	int _state,_direction, _touchID;
	float _maxSpeed, _minSpeed;

	void SetAccelSpeed(float accel) { _accel = accel; };
	void Init(float speed, float mul, float maxSpeed, float minSpeed);
	void Update();
	void AddOffset(float offset);	
	bool IsScroll();


	//check touch	
	bool CheckTouch(int touchID);
	void ClearTouchScroll();
	void SetBeginScorll(int touchID);

	void SetState(int state);	
	float GetCurrentSpeed(){ return _currentSpeed; }
	void SetMulSpeed(float value){ _mulSpeed = value; };

};

#endif