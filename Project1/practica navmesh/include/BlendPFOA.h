#ifndef BLEND_PF_OA_H
#define BLEND_PF_OA_HRING_H

#include "ISteering.h"
class PathFollowingSteering;
class ObstacleAvoidanceSteering;

struct SteeringAndWeight {
	SteeringAndWeight(ISteering * steering, float weight) : steering(steering), weight(weight) {}
	ISteering * steering;
	float weight;
};

class BlendPFOA : public ISteering {
public:
	BlendPFOA(PathFollowingSteering * pfs, ObstacleAvoidanceSteering * oas, float w1, float w2);
	~BlendPFOA();
	virtual Acceleration GetSteering(Character * character);
private:
	SteeringAndWeight * m_pfSteering;
	SteeringAndWeight * m_oaSteering;
};

#endif

