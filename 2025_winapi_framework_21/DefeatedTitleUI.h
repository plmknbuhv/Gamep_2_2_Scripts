#pragma once
#include "Object.h"

class Texture;

class DefeatedTitleUI:
    public Object
{
public:
    DefeatedTitleUI();
    ~DefeatedTitleUI();
    void Render(HDC _hdc) override;

private:
    Texture* m_pTexture;

    
};
