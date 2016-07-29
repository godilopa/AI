#ifndef __ENEMY_BT_H__
#define __ENEMY_BT_H__

#include "Selector.h"
class Character;

//El nodo raiz es un selector
class EnemyBT : public Selector{
public:
	EnemyBT(Character * character, Blackboard * blackboard);
};

#endif
