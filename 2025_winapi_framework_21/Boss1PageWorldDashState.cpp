#include "pch.h"
#include "Boss1PageWorldDashState.h"
#include <algorithm>
#include "Boss.h"
#include "TweenManager.h"
#include "Animator.h"
#include "Animation.h"
#include "DashImpactArea.h"
#include "BulletShooter.h"
#include "Overlap.h"
#include "SceneManager.h"
#include "ImpulseManager.h"

Boss1PageWorldDashState::Boss1PageWorldDashState(Boss* bs)
    : State(bs), m_currentDashCount(0), m_dashDuration(0.5f), m_nextDashTime(1.5f), m_currentTime(0.f), 
    m_enableCalculateTime(false), m_initialPos({0.f, 0.f}), m_maxMoveX(WINDOW_WIDTH * 1.25f), m_alreadyDrawArea(false),
    m_damage(15), m_isDashing(false)
{
    m_animator = GetOwner<Boss>()->GetComponent<Animator>();
    m_impactArea = new DashImpactArea();
    
    Vector2 size = GetOwner<Boss>()->GetSize();
    float startX = WINDOW_WIDTH * -0.25f;
    float endX = WINDOW_WIDTH - (GetOwner<Boss>()->GetSize().x / 2) + 20.f;
    
    float visualH = (size.y > 0.f) ? size.y : (134.f * 6.f);
    float bossHalfY = max(0.f, visualH * 0.5f);
    
    float winH = (float)WINDOW_HEIGHT;
    float marginRatio = 0.10f;
    float minGap = max(60.f, winH * 0.1f);

    float safeTop = max(bossHalfY, winH * marginRatio);
    float safeBottom = min(winH - bossHalfY, winH * (1.f - marginRatio));
    
    if (safeTop >= safeBottom)
    {
        safeTop = winH * 0.25f;
        safeBottom = winH * 0.75f;
    }
    
    if (safeBottom - safeTop < minGap)
    {
        float center = winH * 0.5f;
        safeTop = center - minGap * 0.5f;
        safeBottom = center + minGap * 0.5f;
        safeTop = max(safeTop, bossHalfY);
        safeBottom = min(safeBottom, winH - bossHalfY);
    }
    
    float middleY = (safeTop + safeBottom) * 0.5f;
    
    m_dashInfo.push_back({ { startX, safeTop }, { endX, safeTop } });
    m_dashInfo.push_back({ { startX, safeBottom }, { endX, safeBottom } });
    m_dashInfo.push_back({ { startX, middleY }, { endX, middleY } });
}

Boss1PageWorldDashState::~Boss1PageWorldDashState()
{
    GET_SINGLE(SceneManager)->RequestDestroy(m_impactArea);
}

void Boss1PageWorldDashState::EnterState()
{
    State::EnterState();
    
    Vector2 sz = GetOwner<Boss>()->GetSize();
    float h = sz.y > 0.f ? sz.y : (134.f * 6.f);
    if (m_impactArea != nullptr)
    {
        m_impactArea->SetFixedHeight(h);
        m_impactArea->Hide();
    }

    m_initialPos = GetOwner<Boss>()->GetPos();
    m_animator->Play(L"boss_Dash_Ready", PlayMode::Once, 1, 1.f, [this]()
        {
            GetOwner<Boss>()->HitEnable(false);
            m_animator->Play(L"boss_Disappear", PlayMode::Once, 1, 1.2f, [this]()
                {
                    m_enableCalculateTime = true;
                    m_isDashing = true;
                });
            m_animator->GetCurrent()->SetFlipX(true);
            m_animator->GetCurrent()->SetScale(6);
        });
    m_animator->GetCurrent()->SetFlipX(true);
    m_animator->GetCurrent()->SetScale(6);
}

void Boss1PageWorldDashState::LateUpdate()
{
    State::LateUpdate();

    if (m_isDashing)
    {
        std::vector<Collider*> hits;
        if (Overlap::OverlapBox(GetOwner<Boss>()->GetPos(), GetOwner<Boss>()->GetSize(), Layer::PLAYER, hits))
        {
            for (auto hit : hits)
            {
                Entity* entity = dynamic_cast<Entity*>(hit->GetOwner());
                if (entity == nullptr)
                    continue;
                entity->ApplyDamage(m_damage, GetOwner<Boss>());
            }
        }
    }
    
    if (!m_enableCalculateTime)
        return;

    m_currentTime += fDT;

    if (m_currentTime >= m_nextDashTime * (m_currentDashCount + 1))
    {
        if (m_currentDashCount >= m_dashInfo.size())
        {
            m_enableCalculateTime = false;
            GetOwner<Boss>()->SetPos(m_initialPos);
            m_isDashing = false;
            GET_SINGLE(ImpulseManager)->ApplyImpulse(2.f, 1.f);
            m_animator->Play(L"boss_Smash_End", PlayMode::Once, 0, 1.5f, [this]()
                {
                    GetOwner<Boss>()->ChangeState("Idle");
                });
            m_animator->GetCurrent()->SetFlipX(true);
            m_animator->GetCurrent()->SetScale(6);
        }
        else
        {
            GetOwner<Boss>()->HitEnable(false);
            GetOwner<Boss>()->SetPos(m_dashInfo[m_currentDashCount].first);
            Vector2 to = m_dashInfo[m_currentDashCount].second;
            m_enableCalculateTime = false;
            m_animator->Play(L"boss_Dash");
            m_animator->GetCurrent()->SetFlipX(false);
            m_animator->GetCurrent()->SetScale(6);
            GET_SINGLE(TweenManager)->MoveTo(GetOwner<Boss>(), to, m_dashDuration, EaseType::Linear, [this]()
                {
                    m_animator->Play(L"boss_Idle", PlayMode::Loop, 1, 1.f);
                    m_animator->GetCurrent()->SetFlipX(false);
                    GetOwner<Boss>()->HitEnable(true);
                    GET_SINGLE(ImpulseManager)->ApplyImpulse(4.f, 0.7f);
                    GetOwner<Boss>()->GetComponent<BulletShooter>()->ShootAround(200.f, 10, Layer::PLAYER);
                    if (m_impactArea)
                        m_impactArea->Hide();

                    m_currentDashCount++;
                    m_enableCalculateTime = true;
                    m_alreadyDrawArea = false;
                });
        }
    }

    else if (!m_alreadyDrawArea)
    {
        if (m_impactArea)
        {
            int idx = m_currentDashCount;
            int count = (int)m_dashInfo.size();
            if (idx >= 0 && idx < count)
            {
                Vector2 from = m_dashInfo[idx].first;
                Vector2 to = { (float)WINDOW_WIDTH, m_dashInfo[idx].second.y };
                float triggerTime = m_nextDashTime * (idx + 1);
                float remain = triggerTime - m_currentTime;
                if (remain <= 0.f) remain = 0.2f;

                m_impactArea->DrawArea(from, to, remain);
                m_alreadyDrawArea = true;
            }
        }
    }
        
}

void Boss1PageWorldDashState::ExitState()
{
    State::ExitState();
    GetOwner<Boss>()->HitEnable(true);
    m_currentDashCount = 0;
    m_currentTime = 0.f;
    m_enableCalculateTime = false;
    m_alreadyDrawArea = false;
    if (m_impactArea)
        m_impactArea->Hide();
}