#include <stdafx.h>
#include "../include/BlendPFOA.h"
#include "../include/ObstacleAvoidanceSteering.h"
#include "../include/PathFollowingSteering.h"
#include "character.h"

BlendPFOA::BlendPFOA(PathFollowingSteering * pfs, ObstacleAvoidanceSteering * oas, float w1, float w2) {
	m_pfSteering = new SteeringAndWeight(pfs, w1);
	m_oaSteering = new SteeringAndWeight(oas, w2);
}

BlendPFOA::~BlendPFOA(){
	delete m_pfSteering;
	delete m_oaSteering;
}

Acceleration BlendPFOA::GetSteering(Character * character) {
	Acceleration acc1;
	Acceleration acc2; //Se sobreescribira para ser la total
	acc1 = m_pfSteering->steering->GetSteering(character);
	acc2 = m_oaSteering->steering->GetSteering(character);
	acc2.linear = acc1.linear * m_pfSteering->weight + acc2.linear * m_oaSteering->weight;
	//Si la aceleracion se pasa de la maxima se limita a la maxima
	if (acc2.linear.Length() > character->GetParams().max_acceleration) {
		acc2.linear = acc2.linear.NormSafe() * character->GetParams().max_acceleration;
	}

	return acc2;
}