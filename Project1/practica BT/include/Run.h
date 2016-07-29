#ifndef __RUN_H__
#define __RUN_H__

#include "Behavior.h"

class Run : public Behavior {
public:
	Run(Character * character, Blackboard * blackboard);
private:
	Status Update();
	void OnEnter();
	void OnExit();
};


#endif
