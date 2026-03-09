#include "pch.h"
#include "Boss2PageDashState.h"
#include "Animation.h"
#include "Boss.h"
#include "BulletShooter.h"
#include "Collider.h"
#include "Health.h"
#include "Overlap.h"
#include "Player.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Tween.h"
#include "TweenManager.h"
#include "ImpulseManager.h"
#include "MeleePlayer.h"
#include "RangedPlayer.h"


Boss2PageDashState::Boss2PageDashState(Boss* bs)
: State(bs),m_player(nullptr), m_getParring(false), m_calculateUpdate(false)
{
    m_owner = bs;
    m_collider = m_owner->GetComponent<Collider>();
    m_animator = m_owner->GetComponent<Animator>();
    m_colliders.clear();
    
}

void Boss2PageDashState::EnterState()
{
    State::EnterState();

    m_player = GET_SINGLE(SceneManager)->GetCurScene()->GetNearbyObject<RangedPlayer>(GetOwner<Boss>(), Layer::PLAYER);
    if (m_player->GetComponent<Health>()->GetCurrentHealth() <= 0)
    {
        m_player = GET_SINGLE(SceneManager)->GetCurScene()->GetNearbyObject<MeleePlayer>(GetOwner<Boss>(), Layer::PLAYER);
    }

    m_animator->Play(L"boss_Dash_Ready", PlayMode::Once, 1, 1.f, [this]()
        {
            m_hitCallback = GetOwner<Boss>()->OnDamageApplied = [this](Boss* boss, Entity* dealer)
                { 
                    Player* dealerPlayer = static_cast<Player*>(dealer);
                    if (m_getParring || dealerPlayer->GetPlayerType() != PlayerType::Melee)
                        return;
                    m_getParring = true;
                    GET_SINGLE(ImpulseManager)->ApplyImpulse(5.f, 0.7f);
                    m_hitCallback = NULL;
                    boss->ApplyDamage(20, dealer);
                };
            //PlayerManager
            m_startPlayerPos = m_player->GetPos();
            m_startBossPos = m_owner->GetPos();
            m_dir = m_startPlayerPos - m_startBossPos;
            m_animator->Play(L"boss_Dash",PlayMode::Loop,1,2);
            m_animator->GetCurrent()->SetScale(6);
            m_animator->GetCurrent()->SetFlipX(m_dir.x < 0.f);
            m_dir.Normalize();
            m_dir *= 0.9f; // speed
            m_owner->GetComponent<BulletShooter>()->ShootAround(250.f, 10, Layer::PLAYER);
            // Vector2 size = m_animator->GetCurrent()->GetScale();
            // m_collider->SetEnable(false);
            m_collider->SetTrigger(true);
            // m_collider->SetSize({10,10});
            m_owner->SetPos(m_startBossPos);
            m_movetoTween = static_cast<Tween<Vec2>*>(
                GET_SINGLE(TweenManager) -> MoveTo(m_owner,m_startPlayerPos,0.5f,EaseType::Linear));
            GET_SINGLE(ResourceManager)->Play(L"BossDash");
            m_colliders.clear();
            m_calculateUpdate = true;
        });
    m_animator->GetCurrent()->SetScale(6);
    m_animator->GetCurrent()->SetFlipX(true);
}

void Boss2PageDashState::LateUpdate()
{
    State::LateUpdate();
    if (!m_calculateUpdate)
        return;

    vector<Collider*> colliders;
    if (Overlap::OverlapBox(m_owner->GetPos(), { 100,100 }, Layer::PLAYER, colliders))
    {
        for (Collider* collider : colliders)
        {
            if (std::find(m_colliders.begin(),m_colliders.end(),collider) == m_colliders.end())
            {
                Object* obj = collider->GetOwner();
                Player* plr = static_cast<Player*>(obj);
                cout << "Case" << endl;
                plr->ApplyDamage(10,m_owner);
                m_colliders.push_back(collider);
            }
        }
    }
    if (Vector2::Distance(m_owner->GetPos(), m_startPlayerPos) <= 0.1f || m_getParring)
    {
        cout << "Arrive" << endl;
        m_calculateUpdate = false;
        if (m_movetoTween != nullptr)
        {
            GET_SINGLE(TweenManager)->RemoveTween(m_movetoTween);
            m_movetoTween = nullptr;
        }
        GET_SINGLE(TweenManager) -> MoveTo(m_owner,m_startBossPos,1.0f,EaseType::EaseInExpo, [this]()
            {
                m_owner->ChangeState("Idle");
                return;
            });
    }
}


void Boss2PageDashState::ExitState()
{
    State::ExitState();
    // m_collider->SetEnable(true);
    m_hitCallback = nullptr;
    m_getParring = false;
    m_calculateUpdate = false;
}
