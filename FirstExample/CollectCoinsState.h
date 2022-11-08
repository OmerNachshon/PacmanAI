#pragma once
#include "State.h"
class CollectCoinsState :
	public State
{
public:
	CollectCoinsState();
	~CollectCoinsState();

	 void MakeTransition(Pacman* owner, int mode);
	 void OnStateEnter(Pacman* owner);
	 void OnStateExit(Pacman* owner);
};

