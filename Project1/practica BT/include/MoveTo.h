#ifndef __MOVETO_H__
#define __MOVETO_H__

#include "Behavior.h"

class MoveTo : public Behavior {
public:
	MoveTo(Character * character, Blackboard * blackboard);
private:
	Status Update();
	void OnEnter();
	void OnExit();
};


#endif