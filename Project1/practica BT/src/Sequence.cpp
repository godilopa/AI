#include <stdafx.h>
#include "..\include\Sequence.h"

void Sequence::OnEnter(){
	m_CurrentChild = 0;
}

Status Sequence::Update(){
	while(true) {
		Status s = m_Children[m_CurrentChild]->Tick();

		if(s != eSuccess){
			m_CurrentChild = 0;
			return s;
		}

		++m_CurrentChild;

		if(m_CurrentChild == m_Children.size()){
			m_CurrentChild = 0;
			ResetChildren();
			return eSuccess;
		}
	}

	return eInvalid;
}

void Sequence::ResetChildren(){
	for (int i = 0; i < m_Children.size(); i++) {
		m_Children[i]->SetStatus(Status::eInvalid);
	}
}
