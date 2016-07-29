#ifndef __NEW_RANDOM_POINT_H__
#define __NEW_RANDOM_POINT_H__

#include "Behavior.h"

class NewRandomPoint : public Behavior {
public:
	NewRandomPoint(Character * character, Blackboard * blackboard);
private:
	Status Update();
	void OnEnter();
	void OnExit();
};


#endif
