#pragma once
#include "Boss.h"
#include "Player.h"
#include "State.h"

class Boss2PageGroundPangState : public State
{
public:
    Boss2PageGroundPangState(Boss* bs);

public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

private:
    Player* m_player;
    Boss* m_owner;
    bool m_isEnd;
    Collider* m_collider;
    Animator* m_animator;
    float m_skillTimer;
};