#pragma once
#include "State.h"

class Boss;
class Animator;

class BossSpawnState : public State
{
public:
    BossSpawnState(Boss* bs);
public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;
private:
    Boss* m_owner;
    Animator* m_animator;
    float m_spawnTimer,m_delayTimer;
    bool m_spawnStart;//ÇÑ±¹¾î
};
