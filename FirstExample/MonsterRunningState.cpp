#include "MonsterRunningState.h"
#include "MonsterChasingState.h"



MonsterRunningState::MonsterRunningState()
{
}


MonsterRunningState::~MonsterRunningState()
{
}

void MonsterRunningState::MakeTransition(Monster * owner)
{
	//Change Role Position: run-> chase
	owner->getCurrentState()->OnStateExit(owner);
	owner->setCurrentState(new MonsterChasingState());
	owner->getCurrentState()->OnStateEnter(owner);
}

void MonsterRunningState::OnStateEnter(Monster * owner)
{
	owner->setIsRunning(true);
	owner->setIsChasing(false);
}

void MonsterRunningState::OnStateExit(Monster * owner)
{
	owner->setIsRunning(false);
}
