#ifndef ALIGN_TO_MOVEMENT_STEERING_H
#define ALIGN_TO_MOVEMENT_STEERING_H

#include "ISteering.h"
class AlignSteering;
struct Acceleration;

class AlignToMovementSteering : public ISteering{
public:
	AlignToMovementSteering();
	~AlignToMovementSteering();
	virtual Acceleration GetSteering(Character * character);
private:
	AlignSteering * mAlignSteering;
};

#endif
