#pragma once
#include "Boss.h"
#include "State.h"

class Boss2PageSpawnBabyState : public State
{
public:
    Boss2PageSpawnBabyState(Boss* bs);

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
