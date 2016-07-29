#include <stdafx.h>
#include "..\include\Behavior.h"

Status Behavior::Update()
{
	return Status();
}

void Behavior::OnEnter()
{
}

void Behavior::OnExit()
{
}

Status Behavior::Tick() {
	if(m_Status == eInvalid) {
		OnEnter();
	}

	m_Status = Update();

	if(m_Status != eRunning){
		OnExit();
	}

	return m_Status;
}
