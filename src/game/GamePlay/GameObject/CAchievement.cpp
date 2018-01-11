#include "pch.h"
#include "../../GameCore/CGame.h"

void CAchievement::SetDefaultValue()
{
	_game = GAME();
	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{
		_achievement[i]._currentValue = 0;
		_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_NONE;
	}

	for (int i = 0; i < TOTAL_COLOR_DES_ROW; i++)
	{
		_game->_user._scoreColor[i] = 0;
	}
}
void CAchievement::LoadInfoAchievement()
{
	_game = GAME();
	_gp = G();	
	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{		
		_achievement[i]._maxValue = GDS_Exporter::achievement_values_des[i];
		if (i== CAchievement::enumAchiveID::S_Impressive)
		{
			_achievement[i]._maxValue *= TOTAL_COLOR_DES_ROW;
		}
		
#if defined (MAC_OS)
		sprintf(_achievement[i]._id, "%s", GDS_Exporter::achievement_id_des[i]);
#else
		sprintf(_achievement[i]._id, "%s", GDS_Exporter::achievement_gg_id[i]);
#endif

		if (_achievement[i]._stateComplete == CAchievement::enumAchievementState::ACHIEVEMENT_COMPLETE)
		{			
#if defined MAC_OS
			if (_game->_gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
			{
				_achievement[i]._percent = GetPercent(i);
				PineGameCenterController::UpdateAchievement(_achievement[i]._id, _achievement[i]._percent, true);
				_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_COMPLETE;
			}
#endif
		}
	}	
}

int CAchievement::GetPercent(int i)
{
	int res = 0;
	res = _achievement[i]._currentValue / (float)_achievement[i]._maxValue * 100;
	if (res >= 100)
	{
		res = 100;
	}
	return res;
}

void CAchievement::SetCompleteAchivement(int i)
{	
	if (_achievement[i]._stateComplete == CAchievement::enumAchievementState::ACHIEVEMENT_NONE)
	{
		_achievement[i]._currentValue = _achievement[i]._maxValue;
		int newPercent = 0;
		newPercent = GetPercent(i);
		if (newPercent >_achievement[i]._percent)
		{
			_achievement[i]._percent = newPercent;

#if defined MAC_OS
			if (_game->_gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
			{
				PineGameCenterController::UpdateAchievement(_achievement[i]._id, _achievement[i]._percent, true);
                if (CheckComplete(i))
                {
					_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_END_SUBMIT;
                }
			}
			else
#endif
			{
				if (CheckComplete(i))
				{
					PDEBUG("\n ++++++++++++++++++++++++++++++++++++++++++++achievement complete");
					_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_COMPLETE;
#if defined (ANDROID_OS)
					if(_achievement->_id != "none")
						_game->GGPlaySubmitAchievement(_achievement->_id);
#endif
				}
			}
		}
	}
	
}
void CAchievement::UpdateAchievement() {
	bool issubmit = false;
	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{
		if (_achievement[i]._stateComplete == CAchievement::enumAchievementState::ACHIEVEMENT_COMPLETE)
		{
			issubmit = true;
			_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_END_SUBMIT;
#if defined(ANDROID_OS)
			GAME()->_async_task_manager._list_id_group_achievement_need_check_unlock[i] = true;
#endif
		}
	}
	if (issubmit)
	{
#if defined(ANDROID_OS)
		PDEBUG("\n---------------------------------------------------------UpdateStatusAchievement");
		GAME()->_async_task_manager.AddCommand(CAsyncTasks::enumTypeCommand::COMMAND_TYPE_ASYN_GOOGLE_PLAY_UNLOCK_ACHIEVEMENT);
#endif
	}
}
void CAchievement::CheckAchievementComplete()
{ 
	printf("\n ===============");
	for (int i = 0; i < TOTAL_ACHIEVEMENT_ID_DES_ROW; i++)
	{
		if (_achievement[i]._stateComplete == CAchievement::enumAchievementState::ACHIEVEMENT_NONE)
		{			
			int newPercent = 0;
			int res = 0;
			switch (i)
			{
			case CAchievement::enumAchiveID::S_Impressive:
				for (int k = 0; k < TOTAL_COLOR_DES_ROW; k++)
				{
					res += _game->_user._scoreColor[k] > 50 ? 50 : _game->_user._scoreColor[k];
				}				
				_achievement[i]._currentValue = res;
				break;

			case CAchievement::enumAchiveID::S_ShadelessApprentice:
			case CAchievement::enumAchiveID::S_ShadelessLegendary:
				_achievement[i]._currentValue = _game->_user.numPlayGame;
				break;

			case CAchievement::enumAchiveID::S_Cheater:

				break;

			case CAchievement::enumAchiveID::S_Addicted:
				_achievement[i]._currentValue = _game->_numGameArow;
				break;


			case CAchievement::enumAchiveID::S_FastEyes:
				_achievement[i]._currentValue = _game->_gameStateGamePlay._gamePlay.GetScoreTime();
				break;
			
			case CAchievement::enumAchiveID::S_NeverBackDown:
			{
				Pine_Date_Time time;
				time.setDateByLocalDate();
				int day = time.getDay() - _game->_user._dayResetGame;
				if (day == 1)
				{
					_game->_user._dayPlayGame++;
					_game->_user._dayResetGame = day;
				}
				else if(day > 1)
				{
					_game->_user._dayResetGame = day;
					_game->_user._dayPlayGame = 1;
				}
				_achievement[i]._currentValue = _game->_user._dayPlayGame;
			}
				break;
			
			//check score
			case CAchievement::enumAchiveID::S_ShadeRookie:
			case CAchievement::enumAchiveID::S_ShadePro:
			case CAchievement::enumAchiveID::S_ShadeMaster:
			case CAchievement::enumAchiveID::S_ShadeVeteran:
			case CAchievement::enumAchiveID::S_ShadeGod:
				_achievement[i]._currentValue = _game->_user._highScore;				
				break;
			}
			newPercent = GetPercent(i);
			if (newPercent > _achievement[i]._percent)
			{
				_achievement[i]._percent = newPercent;

#if defined MAC_OS
				if (_game->_gcSocial->GetState() == PINESOCIAL_STATE_COMPLETED)
				{
					PineGameCenterController::UpdateAchievement(_achievement[i]._id, _achievement[i]._percent, true);
					if (CheckComplete(i))
					{
						_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_END_SUBMIT;
					}
				}
				else
#endif
				{
					if (CheckComplete(i))
					{
						_achievement[i]._stateComplete = CAchievement::enumAchievementState::ACHIEVEMENT_COMPLETE;
					}
				}
			}
		}
		else
		{
			_achievement[i]._percent = GetPercent(i);
		}
		printf("\n achievement: %s => %d", _achievement[i]._id, _achievement[i]._percent);
	}
}
bool CAchievement::CheckComplete(int i)
{
	if (_achievement[i]._percent >= 100)
	{
		return true;
	}
	return false;
}