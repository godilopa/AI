#ifndef ALIGN_STEERING_H
#define ALIGN_STEERING_H

#include "ISteering.h"
struct Acceleration;

class AlignSteering : public ISteering{
public:
	virtual Acceleration GetSteering(Character * character);
};

#endif

