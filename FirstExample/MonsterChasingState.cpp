#include "MonsterChasingState.h"
#include "MonsterRunningState.h"



MonsterChasingState::MonsterChasingState()
{
}


MonsterChasingState::~MonsterChasingState()
{
}

void MonsterChasingState::MakeTransition(Monster *owner)
{
		//Change Role Position: chase -> run
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new MonsterRunningState());
		owner->getCurrentState()->OnStateEnter(owner);	
}

void MonsterChasingState::OnStateEnter(Monster * owner)
{
	owner->setIsChasing(true);
	owner->setIsRunning(false);
}

void MonsterChasingState::OnStateExit(Monster * owner)
{
	owner->setIsChasing(false);
}
