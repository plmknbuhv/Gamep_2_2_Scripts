#pragma once
#include "Object.h"

class Texture;

class PlayerDeathBim : public Object
{
public:
    PlayerDeathBim();
    ~PlayerDeathBim();
    void Render(HDC _hdc) override;

private:
    Texture* m_pTexture;
    
};
