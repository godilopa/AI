#ifndef SEEK_STEERING_H
#define SEEK_STEERING_H

#include "ISteering.h"
struct Acceleration;

class SeekSteering : public ISteering {
public:
	virtual Acceleration GetSteering(Character * character);
};

#endif
