#pragma once
#include "State.h"
class Boss1PageDashSkillState :
    public State
{
public:
    Boss1PageDashSkillState(StateMachine* _sm, Boss* _bs);
public:
    virtual void EnterState() override;

public:
    float m_readyToDashTime;
};