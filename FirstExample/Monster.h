#pragma once

class State;
#include "MonsterState.h"
class MonsterState;

class Monster
{
private:
	MonsterState* currentState;
	int points;
	bool isRunning, isChasing;


public:
	Monster();
	~Monster();

	MonsterState* getCurrentState() { return currentState; }
	bool getIsRunning() { return isRunning; }
	bool getIsChasing() { return isChasing; }
	void setCurrentState(MonsterState* pNewState) { currentState = pNewState; }
	void setIsRunning(bool pIsRunning) { isRunning = pIsRunning; }
	void setIsChasing(bool pIsChasing) { isChasing = pIsChasing; }
	void DrowMe();
	void Execute(bool escape);
};

