#include <stdafx.h>
#include "../include/SeekSteering.h"
#include "character.h"
#include "params.h"

Acceleration SeekSteering::GetSteering(Character * character) {
	Acceleration acc;
	USVec2D desiredVel = character->GetParams().targetPosition - character->GetLoc();
	desiredVel = desiredVel.NormSafe() * character->GetParams().max_velocity;
	USVec2D lAcc = desiredVel - character->GetLinearVelocity();
	lAcc = lAcc.NormSafe() * character->GetParams().max_acceleration;

	acc.linear = lAcc;
	acc.angular = 0;
	return acc;
}

