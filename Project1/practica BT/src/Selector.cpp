#include <stdafx.h>
#include "..\include\Selector.h"

void Selector::OnEnter(){
	m_CurrentChild = 0;
}

Status Selector::Update(){
	while (true) {
		Status s = m_Children[m_CurrentChild]->Tick();

		if (s != eFail) {
			m_CurrentChild = 0;
			return s;
		}

		++m_CurrentChild;

		if (m_CurrentChild == m_Children.size()) {
			m_CurrentChild = 0;
			return eFail;
		}
	}

	return eInvalid;
}
