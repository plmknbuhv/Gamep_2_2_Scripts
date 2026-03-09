#pragma once
#include "State.h"
#include "Boss.h"
#include "Animator.h"

class BossChangePhaseState :
    public State
{
public:
    BossChangePhaseState(Boss* bs);
public:
    virtual void EnterState() override;
    virtual void ExitState() override;
private:
    Animator* m_animator;
};

