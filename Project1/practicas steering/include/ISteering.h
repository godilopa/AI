#ifndef I_STEERING_H
#define I_STEERING_H

class Character;
struct Acceleration;

class ISteering {
public:
	virtual Acceleration GetSteering(Character * character) = 0;
};

#endif
