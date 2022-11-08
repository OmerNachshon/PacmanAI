#pragma once
#include "State.h"
class RunState :
	public State
{
public:
	RunState();
	~RunState();
	void MakeTransition(Pacman* owner, int mode);
	void OnStateEnter(Pacman* owner);
	void OnStateExit(Pacman* owner);
};


