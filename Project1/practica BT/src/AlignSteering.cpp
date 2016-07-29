#include <stdafx.h>
#include "../include/AlignSteering.h"
#include "character.h"
#include "params.h"

Acceleration AlignSteering::GetSteering(Character * character) {
	Acceleration acc;
	float desiredRot;
	float rot = character->GetParams().targetRotation - character->GetRot(); //esto es resta de orientation realmente
	//Rotacion deseada entre -180 y 180
	if (rot > 180.0f) {
		rot -= 360.f;
	}
	else if (rot < -180) {
		rot += 360.f;
	}

	if (abs(rot) < character->GetParams().angular_arrive_radius) {
		desiredRot = character->GetParams().max_angular_velocity * rot / character->GetParams().angular_arrive_radius; //Esto es realmente la velocidad angular
	}
	else {
		desiredRot = character->GetParams().max_angular_velocity * rot / abs(rot); //Esto es realmente la velocidad angular
	}

	rot = desiredRot - character->GetAngularVelocity(); //esto es la aceleracion realment

	if (abs(rot) < 0.01f) {
		rot = 0;
	}
	else {
		rot = character->GetParams().max_angular_acceleration * rot / abs(rot); //por mantener el signo
	}

	acc.angular = rot;
	acc.linear = { 0,0 };
	return acc;
}