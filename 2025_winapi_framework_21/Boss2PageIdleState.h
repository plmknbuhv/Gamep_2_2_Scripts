#pragma once
#include "Player.h"
#include "State.h"

class Boss;
class Animator;

class Boss2PageIdleState : public State
{

public:
    Boss2PageIdleState(Boss* bs);

public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

private:
    Player* m_player;
    Boss* m_owner;
	Animator* m_animator;
    
};

