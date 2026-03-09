#include "pch.h"
#include "Boss1PageSpawnBabyGroup.h"
#include "Baby.h"
#include "SceneManager.h"
#include "Animation.h"
#include "ImpulseManager.h"

Boss1PageSpawnBabyGroup::Boss1PageSpawnBabyGroup(Boss* _bs)
	: State(_bs), m_currentTime(0.f), m_spawnBabyTime(0.5f), m_spawnCount(5), m_currentSpawnCount(0),
	m_spawnBabyCount(10), m_spawnBabyPerY(0.f), m_calculateTime(true)
{
	m_animator = GetOwner<Boss>()->GetComponent<Animator>();
}

void Boss1PageSpawnBabyGroup::EnterState()
{
	State::EnterState();
	m_spawnBabyPerY = WINDOW_HEIGHT / m_spawnBabyCount;
	m_calculateTime = true;
}

void Boss1PageSpawnBabyGroup::LateUpdate()
{
	State::LateUpdate();
	if (m_currentSpawnCount >= m_spawnBabyCount)
		GetOwner<Boss>()->ChangeState("Idle");

	if (!m_calculateTime)
		return;

	m_currentTime += fDT;

	if (m_currentTime >= m_spawnBabyTime)
	{
		m_calculateTime = false;
		m_animator->Play(L"boss_Smash", PlayMode::Once, 1, 2.f, [this]()
			{
				m_calculateTime = true;
				int emptyY = 1 + (std::rand() % m_spawnBabyCount);
				for (int i = 1; i < m_spawnBabyCount; i++)
				{
					if (i == emptyY)
						continue;

					Baby* baby = new Baby(GetOwner<Boss>(), false, true);
					GET_SINGLE(SceneManager)->GetCurScene()->AddObject(baby, Layer::BABY);
					baby->SetPos({ (float)WINDOW_WIDTH, m_spawnBabyPerY * i });
				}
				m_animator->Play(L"boss_Idle", PlayMode::Once);
				m_animator->GetCurrent()->SetScale(6);
				m_animator->GetCurrent()->SetFlipX(true);
			}, 12, 1, [this]() { GET_SINGLE(ImpulseManager)->ApplyImpulse(4.f, 1.f); });
		m_animator->GetCurrent()->SetScale(6);
		m_animator->GetCurrent()->SetFlipX(true);
		m_currentTime = 0.f;
		m_currentSpawnCount++;
	}
}

void Boss1PageSpawnBabyGroup::ExitState()
{
	State::ExitState();
	m_calculateTime = false;
	m_currentTime = 0.f;
	m_currentSpawnCount = 0;
	m_spawnBabyPerY = 0.f;
}
