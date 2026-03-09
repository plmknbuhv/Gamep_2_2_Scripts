#pragma once
#include "Boss.h"
#include "State.h"

class BossDeadState : public State
{
public:
   BossDeadState(Boss* _bs);
    ~BossDeadState();
private:
    void EnterState() override;
private:
    Boss* m_owner;

};
