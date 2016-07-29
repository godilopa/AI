#ifndef SELECTOR_H
#define SELECTOR_H

#include "Group.h"

class Selector : public Group {
protected:
	void OnEnter();
	Status Update();
	int m_CurrentChild;
};

#endif
