#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "Group.h"

class Sequence : public Group {
public:
	~Sequence() {
		int a = 1;
	}
protected:
	void OnEnter();
	Status Update();
	void ResetChildren();
	int m_CurrentChild;
};

#endif
