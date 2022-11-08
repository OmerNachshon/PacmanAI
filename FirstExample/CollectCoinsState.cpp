#include "CollectCoinsState.h"
#include "ChaseState.h"
#include "RunState.h"



CollectCoinsState::CollectCoinsState()
{
}


CollectCoinsState::~CollectCoinsState()
{
}

void CollectCoinsState::OnStateEnter(Pacman * owner)
{
	owner->setIsCollectingCoins(true);
	owner->setIsRunning(false);
	owner->setIsChasing(false);
}

void CollectCoinsState::OnStateExit(Pacman * owner)
{
	owner->setIsCollectingCoins(false);
}


void CollectCoinsState::MakeTransition(Pacman * owner, int mode)
{
	//Pacman modes: 1= collect coins , 2=running ,3=chasing.

	//Change Role Position: run -> Collect coins
	if (mode == 2) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new RunState());
		owner->getCurrentState()->OnStateEnter(owner);
	}

	//Change Role Position: run-> chase
	if (mode == 3) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new ChaseState());
		owner->getCurrentState()->OnStateEnter(owner);
	}

}