#pragma once
#include "Effect.h"
class PlayerBulletEffect :
    public Effect
{
public:
    PlayerBulletEffect();
    ~PlayerBulletEffect();
protected:
    virtual void CreateEffectAnim() override;
};

