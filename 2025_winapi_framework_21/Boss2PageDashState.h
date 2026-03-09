#pragma once
#include "Boss.h"
#include "Player.h"
#include "State.h"
#include "Tween.h"

class Boss2PageDashState : public State
{
public:
    Boss2PageDashState(Boss* bs);

public:
    virtual void EnterState() override;
    virtual void LateUpdate() override;
    virtual void ExitState() override;

private:
    bool m_getParring;
    bool m_calculateUpdate;
    Player* m_player;
    Vector2 m_dir,m_startPlayerPos,m_startBossPos;
    Boss* m_owner;
    Collider* m_collider;
    Animator* m_animator;
    Tween<Vec2>* m_movetoTween;
    vector<Collider*> m_colliders;
    std::function<void(Boss* b, Entity* d)> m_hitCallback;
};
