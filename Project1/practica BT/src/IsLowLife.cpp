#include <stdafx.h>
#include "..\include\IsLowLife.h"

IsLowLife::IsLowLife(Character * character, Blackboard * blackboard) {
	m_owner = character;
	m_blackboard = blackboard;
}

Status IsLowLife::Update(){
	if (m_owner->GetLife() < 5) {
		return Status::eSuccess;
	}
	else {
		return Status::eFail;
	}
}

void IsLowLife::OnEnter() {

}

void IsLowLife::OnExit() {
}