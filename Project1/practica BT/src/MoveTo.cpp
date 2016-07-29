#include <stdafx.h>
#include "..\include\MoveTo.h"

#define ACCEPTABLERADIUS 10

MoveTo::MoveTo(Character * character, Blackboard * blackboard){
	m_owner = character;
	m_blackboard = blackboard;
}

Status MoveTo::Update()
{
	Status st = Status::eRunning;
	USVec2D distance = m_owner->GetParams().targetPosition - m_owner->GetLoc();
	float dist = distance.Length();
	if (dist < ACCEPTABLERADIUS) {
		st = Status::eSuccess;
	}
	return st;
}

void MoveTo::OnEnter(){
	m_owner->SetTargetPosition(m_blackboard->GetDestPoint());
}

void MoveTo::OnExit(){

}
