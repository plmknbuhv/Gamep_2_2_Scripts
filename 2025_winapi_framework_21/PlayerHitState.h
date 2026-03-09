#pragma once
#include "PlayerState.h"
class PlayerHitState :
    public PlayerState
{
public:
	PlayerHitState(Player* _player);
	~PlayerHitState();
public:
	void EnterState() override;
	void LateUpdate() override;
	void ExitState() override;
private:
	float m_timer;
};

