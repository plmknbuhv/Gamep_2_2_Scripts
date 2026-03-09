#pragma once
#include "PlayerState.h"
#include "ICanAttackState.h"

class PlayerIdleState 
	: public PlayerState, public ICanAttackState
{
public:
	PlayerIdleState(Player* _player);
	~PlayerIdleState();
public:
	void EnterState() override;
	void LateUpdate() override;
};

