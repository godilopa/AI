#include <stdafx.h>
#include "../include/PathFollowingSteering.h"
#include "../include/SeekSteering.h"
#include "character.h"

PathFollowingSteering::PathFollowingSteering(Path * path) : mPath(path) {
	mSeekSteering = new SeekSteering();
}

PathFollowingSteering::~PathFollowingSteering() {
	delete mSeekSteering;
}

Acceleration PathFollowingSteering::GetSteering(Character * character) {
	USVec2D pointInPath = mPath->GetNearestPathPosition(character->GetLoc());
	USVec2D nextPointInPath = mPath->GetNextPathPosition(pointInPath, character->GetParams().lookAhead);
	character->SetTargetPosition(nextPointInPath);
	return mSeekSteering->GetSteering(character);
}