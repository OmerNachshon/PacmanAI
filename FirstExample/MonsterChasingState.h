#pragma once
#include "MonsterState.h"
class MonsterChasingState :
	public MonsterState
{
public:
	MonsterChasingState();
	~MonsterChasingState();

	void MakeTransition(Monster* owner);
	void OnStateEnter(Monster* owner);
	void OnStateExit(Monster* onwer);
};

