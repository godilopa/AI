#ifndef GROUP_H
#define GROUP_H

#include "Behavior.h"
#include <vector>

class Group : public Behavior {
public:
	//Borramos todos los nodos en cadena de abajo arriba
	virtual ~Group() {
		for(int i = 0; i < m_Children.size(); i++){
			if (((Group *)m_Children[i])->m_Children.size() != 0) {
				for (int j = 0; j < ((Group *)m_Children[i])->m_Children.size(); j++) {
					delete ((Group *)m_Children[i])->m_Children[j];
				}
			}

			delete m_Children[i];
		}
	}
	void AddBehaviour(Behavior * behavior) { m_Children.push_back(behavior); }
protected:
	typedef std::vector<Behavior*> Behaviors;
	Behaviors m_Children;
};



#endif
