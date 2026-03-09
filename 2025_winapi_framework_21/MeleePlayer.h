#pragma once
#include "Player.h"
class MeleePlayer 
	: public Player
{
public:
	MeleePlayer();
	~MeleePlayer() override;
public:
	void Update() override;
protected:
	void CreateAnimation() override;
	void CreateState() override;
public:
	void MoveInput(Vec2& _dir) override;
	void ApplyDamage(int _damage, Entity* _dealer) override;
};

