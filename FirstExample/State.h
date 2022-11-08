#pragma once
#include "Pacman.h"

class Pacman;
class State
{

public:
	State();
	~State();
	virtual void MakeTransition(Pacman* owner, int mode)=0;
	virtual void OnStateEnter(Pacman* owner)=0;
	virtual void OnStateExit(Pacman* onwer)=0;
};

