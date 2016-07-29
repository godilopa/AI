#include <stdafx.h>
#include "..\include\Run.h"
#include "params.h"

Run::Run(Character * character, Blackboard * blackboard) {
	m_owner = character;
	m_blackboard = blackboard;
}

Status Run::Update() {
	return Status::eFail;
}

void Run::OnEnter() {
	m_owner->SetMaxVel(400);
	m_owner->SetMaxAcc(400);
	m_owner->SetLinearVelocity(400, 400);
}

void Run::OnExit() {
	if (m_owner->GetLife() <= 0) {
		m_owner->SetAngularVelocity(1000);
	}
}