#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "character.h"
#include "blackboard.h"

enum Status {
	eInvalid, 
	eSuccess,
	eFail,
	eRunning
};

class Behavior {
protected:
	virtual Status Update();
	virtual void OnEnter();
	virtual void OnExit();
	Character * m_owner;
	Blackboard * m_blackboard;
public:
	Behavior() { m_Status = Status::eInvalid; }
	void SetStatus(Status status) { m_Status = status; }
	Status Tick();
private:
	Status m_Status;
};


#endif
