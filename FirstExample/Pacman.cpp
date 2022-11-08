#include "Pacman.h"
#include "CollectCoinsState.h"
#include "RunState.h"

Pacman::Pacman()
{
	//init rules
	isChasing = false;
	isRunning = true;
	isCollectingCoins = false;

	currentState = new RunState();
	currentState->OnStateEnter(this);
}


Pacman::~Pacman()
{
}

void Pacman::DrowMe()
{
}

void Pacman::Execute(bool escape,bool close)		// if pacman is escaping , then monster is chasing
{
	//Pacman modes: 1= collect coins , 2=running ,3=chasing.
	if (getIsCollectingCoins()) { //1

		//if escape = false , pacman needs to chase monsters
		if (!escape) {
			currentState->MakeTransition(this, 3);
		}
		// else , if monster is close , pacman needs to run
		else if(close) {
			currentState->MakeTransition(this, 2);

		}
	}
	else if (getIsRunning()) { //2

		//if escape = false , pacman needs to chase
		if (!escape) {
			currentState->MakeTransition(this, 3);
		}
		// if escape is true and monsters are far , pacman needs to collect coins
		else if (!close) {
			currentState->MakeTransition(this, 1);

		}

	}
	else if (getIsChasing()) {//3

		//escape = true , pacman needs to run
		if (escape) {
			currentState->MakeTransition(this, 2);
		}
		//escape=false , if monsters are far away  , start collecting coins
		else if (!close) {
			currentState->MakeTransition(this, 1);

		}
	}

}
