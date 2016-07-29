#include <stdafx.h>
#include "../include/ArriveSteering.h"
#include "character.h"
#include "params.h"

Acceleration ArriveSteering::GetSteering(Character * character) {
	Acceleration acc;
	USVec2D desiredVel = character->GetParams().targetPosition - character->GetLoc();
	float distance= desiredVel.Length();
	//Interpolacion lineal a partir del radio
	if (distance < character->GetParams().arrive_radius) {
		desiredVel = desiredVel.NormSafe() * character->GetParams().max_velocity * (distance / character->GetParams().arrive_radius);
	}
	else {
		desiredVel = desiredVel.NormSafe() * character->GetParams().max_velocity;
	}

	USVec2D lAcc = desiredVel - character->GetLinearVelocity();

	lAcc = lAcc.NormSafe() * character->GetParams().max_acceleration;

	acc.linear= lAcc;
	acc.angular = 0;
	return acc;
}