#include <stdafx.h>
#include "../include/AlignToMovementSteering.h"
#include "../include/AlignSteering.h"
#include "character.h"
#include "params.h"

#define RAD2DEG 57.2957795

AlignToMovementSteering::AlignToMovementSteering() {
	mAlignSteering = new AlignSteering();
}

AlignToMovementSteering::~AlignToMovementSteering() {
	delete mAlignSteering;
}

Acceleration AlignToMovementSteering::GetSteering(Character * character) {
	USVec2D vel = character->GetLinearVelocity().NormSafe();

	if (vel.mX != 0) {
		character->SetTargetRotation(atan2f(vel.mY, vel.mX) * RAD2DEG);
	}

	return mAlignSteering->GetSteering(character);
}