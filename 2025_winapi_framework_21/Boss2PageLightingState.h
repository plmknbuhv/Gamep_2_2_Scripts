#pragma once
#include "Boss.h"
#include "Player.h"
#include "State.h"

class Boss2PageLightingState: public State
{
public:
    Boss2PageLightingState(Boss* bs);

public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

private:
    Boss* m_owner;
    bool m_isEnd;
    Animator* m_animator;
    float m_skillTimer;
};
