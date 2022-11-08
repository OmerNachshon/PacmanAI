#pragma once
#include "State.h"
class State;
class Pacman
{
private:
	State* currentState;
	int points;
	bool isCollectingCoins, isRunning, isChasing;


public:
	Pacman();
	~Pacman();

	State* getCurrentState() { return currentState; }
	bool getIsCollectingCoins() { return isCollectingCoins; }
	bool getIsRunning() { return isRunning; }
	bool getIsChasing() { return isChasing; }
	void setCurrentState(State* pNewState) { currentState = pNewState; }
	void setIsCollectingCoins(bool pIsCollectingCoins) { isCollectingCoins = pIsCollectingCoins; }
	void setIsRunning(bool pIsRunning) { isRunning = pIsRunning; }
	void setIsChasing(bool pIsChasing) { isChasing = pIsChasing; }
	void DrowMe();
	void Execute(bool escape,bool close);
};

