#if !_CSCROLL_BAR_H
#define _CSCROLL_BAR_H 1
class CScrollBar
{
public:
	float _max;
	float _min;
	float _speed;
	float _speed_pined;
	float _current;
	float _target;
	float _offset;
	float _offsetMove;
	float _sub_speed;
	bool _collided;
	float _mul_speed;
	float _limit;
	int _state;

	//check touch
	int _touchID;
	bool CheckTouch(int touchID);
	void ClearTouchScroll();

	void Init(int min, int max, float limit, float speed, float mul = 1, bool is_max = true);
	void Update();
	void AddOffset(float offset);
	void SetTarget(float target);
	void ValidateRange();
	void ValidateRangeExt();
	void Pause();
	void SetBeginScorll(int touchID);
	void Set();
	void Resume();
	void End();

	void SetCurrent(float value);
	float GetCurrent(){ return _current; }
	bool IsScrolling(){ return _current != _target; };
	void ReduceOffset(float mul);
	void ForceToTarget();
	float GetDistace();
	void SetState(int state){ _state = state; };

};

#endif