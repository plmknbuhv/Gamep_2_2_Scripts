#pragma once
#include "State.h"
#include "Boss.h"
#include "Animator.h"

class Boss1PageSpawnBabyGroup :
    public State
{
public:
    Boss1PageSpawnBabyGroup(Boss* _bs);

public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

public:
    float m_currentTime;
    float m_spawnBabyTime;
    int m_spawnCount;
    int m_currentSpawnCount;
    int m_spawnBabyCount;
    float m_spawnBabyPerY;
    bool m_calculateTime;
    Animator* m_animator;
};

