#pragma once
#include "Monster.h"

class Monster;
class MonsterState
{

public:
	MonsterState();
	~MonsterState();
	
	virtual void MakeTransition(Monster* owner) = 0;
	virtual void OnStateEnter(Monster* owner) = 0;
	virtual void OnStateExit(Monster* onwer) = 0;
};

