#pragma once
#include "Boss.h"
#include "State.h"
#include "StateMachine.h"

class BossIdleState
    : public State
{
public:
    BossIdleState(Boss* bs);
public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

private:
    float m_skillTimer;
    Boss* m_owner;
};