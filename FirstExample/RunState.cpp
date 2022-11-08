#include "RunState.h"
#include "ChaseState.h"
#include "CollectCoinsState.h"



RunState::RunState()
{
}


RunState::~RunState()
{
}

void RunState::MakeTransition(Pacman * owner, int mode)
{
	//Pacman modes: 1= collect coins , 2=running ,3=chasing.

	//Change Role Position: run -> Collect coins
	if (mode == 1) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new CollectCoinsState());
		owner->getCurrentState()->OnStateEnter(owner);
	}

	//Change Role Position: run-> chase
	if (mode == 3) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new ChaseState());
		owner->getCurrentState()->OnStateEnter(owner);
	}
	
}

void RunState::OnStateEnter(Pacman * owner)
{
	owner->setIsRunning(true);
	owner->setIsChasing(false);
	owner->setIsCollectingCoins(false);

}

void RunState::OnStateExit(Pacman * owner)
{
	owner->setIsRunning(false);

}
