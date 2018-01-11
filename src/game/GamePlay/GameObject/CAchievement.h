#if !CACHIEVEMENT_H
#define CACHIEVEMENT_H 1

#define MAX_ACHIEVEMENT_VER_0		50
struct Achievement
{
	BYTE _stateComplete;
	int _percent;
	int _currentValue;
	int _maxValue;
	char _id[512];
};
class CAchievement
{
private:
	CGame *_game;
	Graphic *_gp;

public:
	enum enumAchievementState
	{
		ACHIEVEMENT_NONE,
		ACHIEVEMENT_COMPLETE,
		ACHIEVEMENT_END_SUBMIT,
	};
	enum enumAchiveID
	{
		S_GoodStart,
		S_Impressive,
		S_ShadelessApprentice,
		S_ShadelessLegendary,
		S_Cheater,
		S_Addicted,
		S_Colorful,
		S_FastEyes,
		S_FamiliarFace,
		S_NeverBackDown,
		S_ShowOff,
		S_ShadeRookie,
		S_ShadePro,
		S_ShadeMaster,
		S_ShadeVeteran,
		S_ShadeGod,
	};
	Achievement _achievement[MAX_ACHIEVEMENT_VER_0];
	int GetPercent(int index);
	void SetDefaultValue();
	void LoadInfoAchievement();
	void CheckAchievementComplete();
	bool CheckComplete(int index);
	void SetCompleteAchivement(int index);
	void UpdateAchievement();

	
};

#endif
