#include <stdafx.h>
#include "../include/ObstacleAvoidanceSteering.h"
#include "../character.h"

#define IMAGEHALFWIDTH 42

Acceleration ObstacleAvoidanceSteering::GetSteering(Character * character) {
	Acceleration acc;
	acc.linear = { 0,0 };
	acc.angular = 0;
	std::list<Obstacle> obs = character->GetParams().obstacles;

	for (std::list<Obstacle>::iterator it = obs.begin(); it != obs.end(); ++it) {
		USVec2D lookAheadVector = character->GetLinearVelocity().NormSafe() * character->GetParams().lookAhead;
		USVec2D characterToObstacle = it->center - character->GetLoc();
		float projC2OverLA = characterToObstacle.Dot(character->GetLinearVelocity().NormSafe());
		USVec2D pointAhead;

		if (projC2OverLA > lookAheadVector.Length()) {
			pointAhead = character->GetLoc() + lookAheadVector;
		}
		else if (projC2OverLA < 0) { //Cnd ya ha pasado el obstaculo
			return acc;
		}
		else{
			pointAhead = character->GetLoc() + character->GetLinearVelocity().NormSafe() * projC2OverLA;
		}

		USVec2D diff = pointAhead - it->center;

		if (diff.Length() < it->radius + IMAGEHALFWIDTH) { //Si hay colision
			USVec2D dirToAvoid;
			//Calculo para saber si la direccion es la misma, < 0.001, deberia ser 0, evitar errores de calculo
			if (abs(1 - abs(lookAheadVector.NormSafe().Dot(characterToObstacle.NormSafe()))) < 0.001) {
				dirToAvoid = { -lookAheadVector.mY, lookAheadVector.mX }; //Vector perpendicular
				acc.linear = dirToAvoid.NormSafe() * diff.Length();
				return acc;
			}

			USVec3D characterToObstacle3D = { characterToObstacle, 0 };
			USVec3D diff3D = { diff, 0 };
			USVec3D dirToAvoid3D = characterToObstacle3D.Cross(diff3D).Cross({ characterToObstacle, 0 });
			dirToAvoid = { dirToAvoid3D.mX, dirToAvoid3D.mY };
			acc.linear = dirToAvoid.NormSafe() * diff.Length();
		}
	}

	return acc;
}