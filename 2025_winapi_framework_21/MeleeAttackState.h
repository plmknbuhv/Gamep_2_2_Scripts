#pragma once
#include "PlayerState.h"
#include "Entity.h"

class MeleeAttackState 
	: public PlayerState
{
public:
	MeleeAttackState(Player* _player);
	~MeleeAttackState();
public:
	void EnterState() override;
	void Attack();
	void ExitState() override;
private:
	float m_timer;
	std::vector<Entity*> m_alreadyHitEntity;
};

