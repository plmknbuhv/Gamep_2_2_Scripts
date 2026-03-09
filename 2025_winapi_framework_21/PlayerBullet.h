#pragma once
#include "Object.h"
#include "Player.h"

class Texture;
class Entity;
class Animator;
class PlayerBullet :
    public Object
{
public:
    PlayerBullet(Player* _owner);
    ~PlayerBullet();
public: 
    void Update() override;
    void Render(HDC _hdc) override;
    void EnterCollision(Collider* _other) override;
private:
    void Move();
private:
    Player* m_owner;
    Texture* m_pTexture;
    Animator* m_animator;
    float m_timer;

    const float m_lifeTime = 3;
};

