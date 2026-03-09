#pragma once
#include "Entity.h"
#include "Object.h"
class Texture;
class Projectile :
    public Object
{
public:
    Projectile(int _damage, Layer _targetLayer, Object* _dealer);
    ~Projectile();
    void Update() override;
    void Render(HDC _hdc) override;
public:
    void SetAngle(float _f)
    {
        m_angle = _f;
    }
    void SetDir(Vec2 _dir)
    {
        m_dir = _dir;
        m_dir.Normalize();
    }
    void SetSpeed(float _speed) { m_speed = _speed;}
private:
    Texture* m_pTexture;
    //float m_dir;
    float m_angle;
    Vec2 m_dir;
    float m_speed = 0.f;
    float m_maxSpeed;
    int m_damage;
    Layer m_targetLayer;
    Object* m_dealer;
};

