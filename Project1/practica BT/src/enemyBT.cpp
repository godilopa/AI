#include <stdafx.h>
#include "..\include\enemyBT.h"
#include "..\include\MoveTo.h"
#include "..\include\NewRandomPoint.h"
#include "..\include\IsLowLife.h"
#include "..\include\Run.h"
#include "..\include\Sequence.h"
#include "..\character.h"

EnemyBT::EnemyBT(Character * character, Blackboard * blackboard){
	m_blackboard = blackboard;
	m_owner = character;
	Sequence * seq1 = new Sequence();
	Sequence * seq2 = new Sequence();
	seq1->AddBehaviour(new IsLowLife(character, blackboard));
	seq1->AddBehaviour(new Run(character, blackboard));
	seq2->AddBehaviour(new MoveTo(character, blackboard));
	seq2->AddBehaviour(new NewRandomPoint(character, blackboard));
	AddBehaviour(seq1);
	AddBehaviour(seq2);
}
