#pragma once
#include "Component.h"
#include "Object.h"

class BulletShooter: public Component
{
public:
    BulletShooter();
    ~BulletShooter();
public:
    void Init() override;
    void LateUpdate() override;
    void Render(HDC _hdc) override;

public:
    void ShootAround(float _speed = 300.f, int _damage = 0, Layer _targetLayer = Layer::DEFAULT, float _distance = 0.f);
};
