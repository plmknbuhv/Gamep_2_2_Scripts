#pragma once
#include "Boss.h"
#include "HealthHitObj.h"

class BossUIManager : public Object
{
public:
    BossUIManager();
    ~BossUIManager();
    void Update() override;
    void Render(HDC _hdc) override;
    
    void SetBoss(Boss* bs);

private:
    Boss* m_boss;
    Health* m_bossHealth;
    Texture* m_BackgroundTex;
    Texture* m_BarTex;
    Texture* m_HitedTex;

    

private:
    float currentHealth;
    float maxHealth;
    float damagedPer;
	std::wstring Persent_text = L"100.0%";
    vector<HealthHitObj*> hitObjs; //todo : 여기에서 매모리 누수가 있을꺼 같은데..

private:
    int uI_WitdhSize = 800;
    int uI_hightSize = 35;
    int uI_barPadingSize = 3;
    int startXPos = 0;
    int startYPos = 0;
    Vector2 pos;

private :
    bool hited;
    bool bossDead;
    float waitTimer;
};
