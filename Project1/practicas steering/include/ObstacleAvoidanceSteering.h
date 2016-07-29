#ifndef OBSTACLE_AVOIDANCE_STEERING_H
#define OBSTACLE_AVOIDANCE_STEERING_H

#include "ISteering.h"
struct Acceleration;

class ObstacleAvoidanceSteering : public ISteering {
public:
	virtual Acceleration GetSteering(Character * character);
};

#endif
