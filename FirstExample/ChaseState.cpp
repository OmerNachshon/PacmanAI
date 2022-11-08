#include "ChaseState.h"
#include "RunState.h"
#include "CollectCoinsState.h"



ChaseState::ChaseState()
{
}


ChaseState::~ChaseState()
{
}

void ChaseState::MakeTransition(Pacman *owner, int mode)
{
	//Pacman modes: 1= collect coins , 2=running ,3=chasing.

	//Change Role Position: chase -> Collect coins
	if (mode == 1) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new CollectCoinsState());
		owner->getCurrentState()->OnStateEnter(owner);
	}

	//Change Role Position: chase -> run
	if (mode == 2) {
		owner->getCurrentState()->OnStateExit(owner);
		owner->setCurrentState(new RunState());
		owner->getCurrentState()->OnStateEnter(owner);
	}
}

void ChaseState::OnStateEnter(Pacman * owner)
{

	owner->setIsChasing(true);
	owner->setIsRunning(false);
	owner->setIsCollectingCoins(false);
}

void ChaseState::OnStateExit(Pacman * owner)
{

	owner->setIsChasing(false);

}
