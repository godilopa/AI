#ifndef ARRIVE_STEERING_H
#define ARRIVE_STEERING_H

#include "ISteering.h"
struct Acceleration;

class ArriveSteering : public ISteering {
public:
	virtual Acceleration GetSteering(Character * character);
};

#endif

