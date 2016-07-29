#ifndef __BLACKBOARD_H__
#define __BLACKBOARD_H__

class Blackboard{
public:
	void SetDestPoint(USVec2D destPoint) { m_destPoint = destPoint; }
	USVec2D GetDestPoint() { return m_destPoint; }
private:
	USVec2D m_destPoint;
};

#endif
