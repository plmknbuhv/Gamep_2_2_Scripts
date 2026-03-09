#include "pch.h"
#include "BulletShooter.h"

#include "Projectile.h"
#include "SceneManager.h"


BulletShooter::BulletShooter()
{
}


BulletShooter::~BulletShooter()
{
}

void BulletShooter::Init()
{
}

void BulletShooter::LateUpdate()
{
}

void BulletShooter::Render(HDC _hdc)
{
}

void BulletShooter::ShootAround(float _speed, int _damage, Layer _targetLayer, float _distance)
{
    float angle = 0.f;
    Object* owner = GetOwner();
    for (int i = 0; i < 18; ++i)
    {
        Projectile* proj = new Projectile(_damage, _targetLayer, GetOwner());
        if (owner)
        {
            Vec2 ownerPos = owner->GetPos();

            float radianAngle = angle * PI / 180.f;

            Vec2 spawnOffset = {
                _distance * cos(radianAngle),
                _distance * sin(radianAngle)
            };

            Vec2 spawnPos = ownerPos + spawnOffset;

            proj->SetPos(spawnPos);
        }else
        {
            cout << "Owner is NULL" << endl;
        }
        proj->SetSize({ 20.f,20.f });
        proj->SetAngle(angle * PI / 180.f);
        proj->SetDir({ cos(angle * PI / 180.f), sin(angle * PI / 180.f) });

        GET_SINGLE(SceneManager)->GetCurScene()
            ->AddObject(proj, Layer::ENEMY_PROJECTILE);

        angle += 20.f;
    }
}


