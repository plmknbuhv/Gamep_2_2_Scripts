#pragma once
#include "PlayerState.h"
class RangedAttackState 
	: public PlayerState
{
public:
	RangedAttackState(Player* _player);
	~RangedAttackState();
public:
	void EnterState() override;
	void LateUpdate() override;
private:
	float m_timer;
	int m_attackCount;
};

