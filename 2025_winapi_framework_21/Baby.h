#pragma once
#include "Animator.h"
#include "BulletShooter.h"
#include "Object.h"
#include "Entity.h"

class Baby
    : public Entity
{
public:
    Baby(Entity* summoner, bool shoot = true, bool isCanHit = false);
    void Update() override;
    void Render(HDC _hdc) override;
    void virtual ApplyDamage(int _damage, Entity* _dealer) override;
public:
    void SetWaitTime(float _waitTime);
private:
    Texture* m_bTexture;
    Vec2 m_position;
    float m_timer,m_WaitTime;
    bool m_isSpawned,waiting;
    bool m_shootProjectile;
    Animator* m_animator;
    BulletShooter* m_shooter;
    Entity* m_summoner;
};
