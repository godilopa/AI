#include <stdafx.h>
#include "..\include\NewRandomPoint.h"

NewRandomPoint::NewRandomPoint(Character * character, Blackboard * blackboard){
	m_owner = character;
	m_blackboard = blackboard;
}

Status NewRandomPoint::Update()
{
	return Status::eSuccess;
}

void NewRandomPoint::OnEnter(){
	m_blackboard->SetDestPoint({ (float)(rand() % 200), (float)(rand() % 300) });
}

void NewRandomPoint::OnExit(){
}
