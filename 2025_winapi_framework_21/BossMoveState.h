#pragma once
#include "State.h"

class Boss;
class BossMoveState
	: public State
{
public:
	BossMoveState(Boss* _bs);
public:
	virtual void EnterState() override;
	virtual void LateUpdate() override;
	virtual void ExitState() override;
};

