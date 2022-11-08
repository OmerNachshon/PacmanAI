#include "Monster.h"
#include "MonsterChasingState.h"

Monster::Monster()
{
	//init rules
	isChasing = true;
	isRunning = false;

	currentState = new MonsterChasingState();
	currentState->OnStateEnter(this);
}


Monster::~Monster()
{
}

void Monster::DrowMe()
{
}

void Monster::Execute(bool escape) // escape refers to pacman , so if pacman escapes , monster is chasing
{
	if (getIsRunning()) {

		//if monster far 
		if (escape) {
			currentState->MakeTransition(this);
		}
	}
	else if (getIsChasing()) {

		//if role change to running and monster far
		if (!escape) {
			currentState->MakeTransition(this);
		}

	}

}
