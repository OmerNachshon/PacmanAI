#pragma once
#include "MonsterState.h"
class MonsterRunningState :
	public MonsterState
{
public:
	MonsterRunningState();
	~MonsterRunningState();

	void MakeTransition(Monster* owner);
	void OnStateEnter(Monster* owner);
	void OnStateExit(Monster* onwer);
};

