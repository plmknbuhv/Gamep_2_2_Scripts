#pragma once
#include "Object.h"
#include "Texture.h"

class HealthHitObj : public Object
{
public:
    HealthHitObj();
    ~HealthHitObj();
    void Update() override;
    void Render(HDC _hdc) override;
    void SetPosScale(Vector2 _pos , Vector2 _size);
    bool GetDestroy();
    void SetDestroy();
private:
    /*Vector2 m_pos;
    Vector2 m_size;*/
    bool m_isDestroy;
    bool m_changeImage;
    float m_texSize;
    float m_spawnedTime;

    
    Texture* m_HitTex;
    Texture* m_DestroyTex;
};
