#ifndef __IS_LOW_LIFE_H__
#define __IS_LOW_LIFE_H__

#include "Behavior.h"

class IsLowLife : public Behavior {
public:
	IsLowLife(Character * character, Blackboard * blackboard);
private:
	Status Update();
	void OnEnter();
	void OnExit();
};


#endif