#pragma once
#include "PlayerState.h"
#include "ICanAttackState.h"

class PlayerMoveState 
	: public PlayerState, public ICanAttackState
{
public:
	PlayerMoveState(Player* _player);
	~PlayerMoveState();
public:
	void EnterState() override;
	void LateUpdate() override;
 private:
	void ClampPlayerPosition();
	void HandleAnimation(Vec2 _dir);
	void AnimMoveFront();
	void AnimMoveBack();
private:
	int m_moveSign;
	float m_moveSpeed;
};

