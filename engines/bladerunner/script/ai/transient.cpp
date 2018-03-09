/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "bladerunner/script/ai_script.h"

namespace BladeRunner {

AIScriptTransient::AIScriptTransient(BladeRunnerEngine *vm) : AIScriptBase(vm) {
}

void AIScriptTransient::Initialize() {
	_animationFrame = 0;
	_animationState = 0;
	_animationStateNext = 0;
	_animationNext = 0;

	Actor_Put_In_Set(kActorTransient, kSetCT03_CT04);
	Actor_Set_At_XYZ(kActorTransient, -171.41, -621.3, 736.52, 580);
	Actor_Set_Goal_Number(kActorTransient, 0);
	Actor_Set_Targetable(kActorTransient, 1);
}

bool AIScriptTransient::Update() {
	if (Global_Variable_Query(kVariableChapter) == 5 && Actor_Query_Which_Set_In(kActorTransient) != 97) {
		Actor_Put_In_Set(kActorTransient, kSetFreeSlotG);
		Actor_Set_At_Waypoint(kActorTransient, 39, 0);
	}
	if ((!Actor_Query_Goal_Number(kActorTransient) || Actor_Query_Goal_Number(kActorTransient) == 10)
			&& Global_Variable_Query(kVariableChapter) == 2) {
		Actor_Set_Goal_Number(kActorTransient, 200);
	}
	if (Game_Flag_Query(169) && Game_Flag_Query(170) && !Game_Flag_Query(171) && !Game_Flag_Query(172)
			&& Global_Variable_Query(kVariableChapter) == 3) {
		Game_Flag_Set(172);
	}
	if (Game_Flag_Query(171) == 1 && Actor_Query_Goal_Number(kActorTransient) != 6
			&& Actor_Query_Goal_Number(kActorTransient) != 599
			&& Global_Variable_Query(kVariableChapter) < 4) {
		Actor_Set_Goal_Number(kActorTransient, 6);
	}
	if (Player_Query_Current_Scene() == 16 && !Game_Flag_Query(492)) {
		Game_Flag_Set(492);
		AI_Countdown_Timer_Reset(kActorTransient, 1);
		AI_Countdown_Timer_Start(kActorTransient, 1, 12);
	}

	return false;
}

void AIScriptTransient::TimerExpired(int timer) {
	if (timer <= 1) {
		if (timer) {
			if (!Actor_Query_Goal_Number(kActorTransient)) {
				Actor_Set_Goal_Number(kActorTransient, 10);
				Actor_Change_Animation_Mode(kActorTransient, 0);
			}
			Actor_Set_Goal_Number(kActorTransient, 10);
			Actor_Set_Targetable(kActorTransient, 0);
			AI_Countdown_Timer_Reset(kActorTransient, 1);
		} else if (Actor_Query_Goal_Number(kActorTransient) == 395 && Actor_Query_Which_Set_In(kActorMcCoy) == 85) {
			AI_Countdown_Timer_Start(kActorTransient, 0, Random_Query(20, 10));
			switch (Random_Query(1, 3) - 1) {
			case 0:
				Sound_Play(356, 50, 0, 0, 50);
				break;
			case 1:
				Sound_Play(357, 50, 0, 0, 50);
				break;
			case 2:
				Sound_Play(358, 50, 0, 0, 50);
				break;
			default:
				break;
			}
		} else if (Actor_Query_Goal_Number(kActorTransient) != 599) {
			Actor_Set_Goal_Number(kActorTransient, 391);
			AI_Countdown_Timer_Reset(kActorTransient, 0);
		}
	}
}

void AIScriptTransient::CompletedMovementTrack() {
	//return false;
}

void AIScriptTransient::ReceivedClue(int clueId, int fromActorId) {
	//return false;
}

void AIScriptTransient::ClickedByPlayer() {
	//return false;
}

void AIScriptTransient::EnteredScene(int sceneId) {
	// return false;
}

void AIScriptTransient::OtherAgentEnteredThisScene(int otherActorId) {
	// return false;
}

void AIScriptTransient::OtherAgentExitedThisScene(int otherActorId) {
	// return false;
}

void AIScriptTransient::OtherAgentEnteredCombatMode(int otherActorId, int combatMode) {
	// return false;
}

void AIScriptTransient::ShotAtAndMissed() {
	// return false;
}

bool AIScriptTransient::ShotAtAndHit() {
	Actor_Set_Frame_Rate_FPS(kActorTransient, 8);
	if (Game_Flag_Query(716))
		_animationState = 11;
	else
		_animationState = 14;

	_animationFrame = 0;

	Actor_Set_Targetable(kActorTransient, 0);
	if (Global_Variable_Query(kVariableChapter) < 4)
		Actor_Set_Goal_Number(kActorTransient, 3);
	else
		Actor_Set_Goal_Number(kActorTransient, 599);

	Game_Flag_Set(169);

	return false;
}

void AIScriptTransient::Retired(int byActorId) {
	Actor_Set_Goal_Number(kActorTransient, 599);

	if (Global_Variable_Query(kVariableChapter) == 4)
		Game_Flag_Set(607);
}

int AIScriptTransient::GetFriendlinessModifierIfGetsClue(int otherActorId, int clueId) {
	return 0;
}

bool AIScriptTransient::GoalChanged(int currentGoalNumber, int newGoalNumber) {
	switch (newGoalNumber) {
	case 2:
		AI_Movement_Track_Flush(kActorTransient);
		AI_Movement_Track_Append(kActorTransient, 51, 0);
		AI_Movement_Track_Append(kActorTransient, 105, 0);
		AI_Movement_Track_Append(kActorTransient, 42, 1);
		AI_Movement_Track_Repeat(kActorTransient);

		return true;
	case 6:
		AI_Movement_Track_Flush(kActorTransient);
		AI_Movement_Track_Append(kActorTransient, 41, 10);
		AI_Movement_Track_Repeat(kActorTransient);

		return true;
	case 200:
		Actor_Put_In_Set(kActorTransient, kSetFreeSlotH);
		Actor_Set_At_Waypoint(kActorTransient, 40, 0);

		return true;
	case 390:
		Actor_Put_In_Set(kActorTransient, kSetUG13);
		Actor_Set_At_XYZ(kActorTransient, -310.0, 55.0, -350.0, 400);
		Actor_Change_Animation_Mode(kActorTransient, 53);
		Actor_Set_Targetable(kActorTransient, 1);
		Game_Flag_Set(716);

		return true;
	case 391:
		Actor_Change_Animation_Mode(kActorTransient, 53);
		return true;
	case 395:
		Actor_Change_Animation_Mode(kActorTransient, 55);
		AI_Countdown_Timer_Start(kActorTransient, 0, Random_Query(30, 40));

		return true;
	case 599:
		AI_Countdown_Timer_Reset(kActorTransient, 0);
		return true;
	}

	return false;
}

bool AIScriptTransient::UpdateAnimation(int *animation, int *frame) {
	switch (_animationState) {
	case 0:
		*animation = 499;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(499)) {
			_animationFrame = 0;
		} else {
			*animation = 497;

			if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(497))
				_animationFrame = 0;
		}
		break;
	case 1:
		*animation = 487;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(487))
			_animationFrame = 0;

		break;
	case 2:
		*animation = 500;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(500))
			_animationFrame = 0;
		break;
	case 3:
		*animation = 501;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(501)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 500;
		}
		break;
	case 4:
		*animation = 502;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(502)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 500;
		}
		break;
	case 5:
		*animation = 503;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(503)) {
			_animationState = 2;
			_animationFrame = 0;
			*animation = 500;
		}
		break;
	case 6:
		*animation = 491;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(491))
			_animationFrame = 0;

		break;
	case 7:
		*animation = 492;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(492)) {
			_animationState = 6;
			_animationFrame = 0;
		}
		break;
	case 8:
		*animation = 493;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(493)) {
			_animationState = 6;
			_animationFrame = 0;
		}
		break;
	case 9:
		*animation = 494;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(494)) {
			_animationState = 6;
			_animationFrame = 0;
		}
		break;
	case 10:
		*animation = 496;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(496)) {
			_animationState = 6;
			_animationFrame = 0;
		}
		break;
	case 11:
		*animation = 495;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(495)) {
			Actor_Set_Frame_Rate_FPS(kActorTransient, 8);
			_animationState = 12;
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(495) - 1;
		}
		break;
	case 12:
		*animation = 495;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(495) - 1;
		break;
	case 14:
		*animation = 489;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(489)) {
			Actor_Set_Goal_Number(kActorTransient, 3);
			_animationState = 15;
			_animationFrame = Slice_Animation_Query_Number_Of_Frames(489) - 1;
			Actor_Set_Targetable(kActorTransient, 0);
			Actor_Retired_Here(kActorTransient, 120, 24, 1, -1);
		}
		break;
	case 15:
		*animation = 489;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(489) - 1;
		break;
	case 16:
		*animation = 504;
		_animationFrame++;
		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(504) - 1)
		_animationFrame = 0;

		break;
	case 17:
		*animation = 505;
		_animationFrame++;

		if (_animationFrame >= Slice_Animation_Query_Number_Of_Frames(505) - 1) {
			*animation = 497;
			_animationFrame = 0;
			_animationState = 0;
			Actor_Change_Animation_Mode(kActorTransient, 0);

			if (Actor_Query_Goal_Number(kActorTransient) == 10 && Player_Query_Current_Scene() == 16) {
				Actor_Set_At_XYZ(kActorTransient, -171.41, -621.30, 728.52, 530);
				Actor_Face_Actor(kActorTransient, kActorMcCoy, 1);
			}
		}
		break;
	case 18:
		*animation = 491;
		_animationFrame++;

		if (_animationFrame - 1 == 4)
			_animationState = 19;

		break;
	case 19:
		*animation = 491;
		_animationFrame--;

		if (_animationFrame + 1 == 4)
			_animationState = 18;

		break;
	default:
		*animation = 399;
		break;
	}

	*frame = _animationFrame;

	return true;
}

bool AIScriptTransient::ChangeAnimationMode(int mode) {
	if (mode != 55)
		Actor_Set_Frame_Rate_FPS(kActorTransient, 8);

	switch (mode) {
	case 0:
		switch (_animationState) {
		case 0:
			return 1;
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 18:
		case 19:
			_animationState = 6;
			_animationFrame = 0;
			break;
		case 16:
			_animationState = 17;
			_animationFrame = 0;
			break;
		default:
			_animationState = 0;
			_animationFrame = 0;
			break;
		}
		break;

	case 1:
		if (_animationState != 1) {
			_animationState = 1;
			_animationFrame = 0;
		}
		break;
	case 2:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 20:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
		return 1;
	case 3:
	case 30:
		if (_animationState - 6 > 4)
			_animationState = 2;
		else
		_animationState = 7;

		_animationFrame = 0;
		break;
	case 12:
	case 31:
		if (_animationState == 6)
			_animationState = 8;
		else
			_animationState = 3;

		_animationFrame = 0;
		break;
	case 13:
	case 32:
		if (_animationState == 6)
			_animationState = 9;
		else
			_animationState = 4;

		_animationFrame = 0;
		break;
	case 14:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
		if (_animationState == 6)
			_animationState = 10;
		else
			_animationState = 5;

		_animationFrame = 0;
		break;
	case 21:
		if (Game_Flag_Query(716))
			_animationState = 11;
		else
			_animationState = 14;

		_animationFrame = 0;
		break;
	case 38:
		_animationState = 16;
		_animationFrame = 0;
		break;
	case 53:
	case 54:
		_animationState = 6;
		_animationFrame = 0;
		break;
	case 55:
		if (_animationState == 6) {
			Actor_Set_Frame_Rate_FPS(kActorTransient, 4);
			_animationState = 18;
			_animationFrame = 3;
		}
		break;
	case 89:
		_animationState = 12;
		_animationFrame = Slice_Animation_Query_Number_Of_Frames(495) - 1;

		break;

	default:
		break;
	}

	return true;
}

void AIScriptTransient::QueryAnimationState(int *animationState, int *animationFrame, int *animationStateNext, int *animationNext) {
	*animationState     = _animationState;
	*animationFrame     = _animationFrame;
	*animationStateNext = _animationStateNext;
	*animationNext      = _animationNext;
}

void AIScriptTransient::SetAnimationState(int animationState, int animationFrame, int animationStateNext, int animationNext) {
	_animationState     = animationState;
	_animationFrame     = animationFrame;
	_animationStateNext = animationStateNext;
	_animationNext      = animationNext;
}

bool AIScriptTransient::ReachedMovementTrackWaypoint(int waypointId) {
	return true;
}

void AIScriptTransient::FledCombat() {
	// return false;
}

} // End of namespace BladeRunner