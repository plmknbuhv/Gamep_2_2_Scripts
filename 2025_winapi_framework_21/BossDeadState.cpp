#include "pch.h"
#include "BossDeadState.h"

#include "Animation.h"
#include "Boss.h"
#include "ImpulseManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"

BossDeadState::BossDeadState(Boss* _bs): State(_bs)
{
    m_owner = _bs;
}

BossDeadState::~BossDeadState()
{

}

void BossDeadState::EnterState()
{
    Animator* anim = m_owner->GetComponent<Animator>();
    anim->Play(L"boss_DeadAnimation",PlayMode::Once,1,0.5f,[this]()
    {
        m_owner->SetDead();
        GET_SINGLE(ResourceManager)->Stop(SOUND_CHANNEL::BGM);
        GET_SINGLE(SceneManager)->GetCurScene()->SetNextScene(L"EndingScene");
    });
    anim->GetCurrent()->SetScale(6);
    anim->GetCurrent()->SetFlipX(true);
    GET_SINGLE(ImpulseManager)->ApplyImpulse(5.f, 5.0f);
}
