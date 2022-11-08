#pragma once
#include "State.h"
class ChaseState :
	public State
{
public:
	ChaseState();
	~ChaseState();
	void MakeTransition(Pacman* owner, int mode);
	void OnStateEnter(Pacman* owner);
	void OnStateExit(Pacman* owner);
};

