#include "pch.h"
#include "Boss.h"
#include "ResourceManager.h"
#include "Animator.h"
#include "BossDeadState.h"
#include "Collider.h"
#include "StateMachine.h"
#include "BossState.h"
#include "BulletShooter.h"
#include "Health.h"
#include "SceneManager.h"

Boss::Boss()
	: m_pTexture_page1(nullptr), m_pTexture_page2(nullptr), m_pAnimator(nullptr), m_moveTimer(0.f),
	m_moveInterval(2.0f), m_moveSpeed(200.f), m_targetPos{ 0.f, 0.f }, m_initialized(false),
	m_currentBossPhase(BossPhase::Phase1), m_size({ 175, 175 }), m_applyDamage(true)
{
	SetSize(m_size);
	m_pTexture_page1 = GET_SINGLE(ResourceManager)->GetTexture(L"BossPage1");
	m_pTexture_page2 = GET_SINGLE(ResourceManager)->GetTexture(L"BossPage2");
	Collider* collider = AddComponent<Collider>();
	collider->SetEnable(true);
	collider->SetName(L"Enemy");
	collider->SetSize({ 165, 175 });
	AddComponent<BulletShooter>();

	m_health->SetUpHealth(2300);
	
	m_pAnimator = AddComponent<Animator>();
	m_pAnimator->Init();

	CreateAnimator(BossPhase::Phase1);
	CreateState();
}

void Boss::CreateAnimator(BossPhase phase)
{

	//m_pAnimator->CreateAnimation(
	//	L"boss_Idle",
	//	m_pTexture,
	//	{ 0.f, 0.f },
	//	{ 160.f, 128.f },
	//	{ 160.f, 0.f },
	//	8, 0.2f
	//);

	m_pAnimator->ResetAllAnimations();

	#pragma region Single Animation

	m_pAnimator->CreateAnimation(
		L"boss_ChangePhase_Hit",
		m_pTexture_page1,
		{ 0.f, 1608.f },
		{ 123.f, 134.f },
		{ 123.f, 0.f },
		8, 0.2f
	);
	m_pAnimator->CreateAnimation(
		L"boss_ChangePhase_1",
		m_pTexture_page1,
		{ 0.f, 671.f },
		{ 123.f, 134.f },
		{ 123.f, 0.f },
		12, 0.1f
	);
	m_pAnimator->CreateAnimation(
		L"boss_ChangePhase_2",
		m_pTexture_page2,
		{ 0.f, 1475.f },
		{ 123.f, 134.f },
		{ 123.f, 0.f },
		9, 0.1f
	);
	m_pAnimator->CreateAnimation(
		L"boss_DeadAnimation",
		m_pTexture_page2,
		{ 0.f, 1742.f },
		{ 123.f, 134.f },
		{ 123.f, 0.f },
		23, 0.1f
	);



#pragma endregion

	switch (phase)
	{
	case BossPhase::Phase1:
		#pragma region Phase1

		m_pAnimator->CreateAnimation(
			L"boss_Idle",
			m_pTexture_page1,
			{ 0.f, 0.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			11, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Buff",
			m_pTexture_page1,
			{ 0.f, 268.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			15, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Disappear",
			m_pTexture_page1,
			{ 738.f, 1742.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			20, 0.2f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Smash_End",
			m_pTexture_page1,
			{ 0.f, 1475.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			9, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Groggy",
			m_pTexture_page1,
			{ 0.f, 1608.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			8, 0.2f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Dash",
			m_pTexture_page1,
			{ 0.f, 939.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			4, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Smash",
			m_pTexture_page1,
			{ 0.f, 671.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			16, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Lightning",
			m_pTexture_page1,
			{ 0.f, 1072.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			17, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Dash_Ready",
			m_pTexture_page1,
			{ 246.f, 1608.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			6, 0.2f
		);

		#pragma endregion
		break;
	case BossPhase::Phase2:
		#pragma region Phase2

		m_pAnimator->CreateAnimation(
			L"boss_Idle",
			m_pTexture_page2,
			{ 0.f, 0.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			11, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Buff",
			m_pTexture_page2,
			{ 0.f, 268.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			15, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Disappear",
			m_pTexture_page2,
			{ 738.f, 1742.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			12, 0.2f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Smash_End",
			m_pTexture_page2,
			{ 0.f, 1475.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			9, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Groggy",
			m_pTexture_page2,
			{ 0.f, 1608.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			8, 0.2f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Dash",
			m_pTexture_page2,
			{ 0.f, 939.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			4, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Smash",
			m_pTexture_page2,
			{ 0.f, 671.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			16, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Lightning",
			m_pTexture_page2,
			{ 0.f, 1072.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			17, 0.1f
		);
		m_pAnimator->CreateAnimation(
			L"boss_Dash_Ready",
			m_pTexture_page2,
			{ 246.f, 1608.f },
			{ 123.f, 134.f },
			{ 123.f, 0.f },
			6, 0.2f
		);

		#pragma endregion
		break;
	default:
		break;
	}
}

void Boss::CreateState()
{
	m_stateMachine = AddComponent<StateMachine>();
	m_stateMachine->Init();

	m_stateMachine->AddState("ChangePhase", new BossChangePhaseState(this));
	m_stateMachine->AddState("Idle", new BossIdleState(this));
	m_stateMachine->AddState("Spawn", new BossSpawnState(this));
	m_stateMachine->AddState("Dead", new BossDeadState(this));
	CreateSkill(m_stateMachine);
	
	m_stateMachine->ChangeState("Spawn");
	//stateMachine->ChangeState("Idle");
}


void Boss::CreateSkill(StateMachine* stateMachine)
{
	stateMachine->AddState("BaseSkill", new BaseBossSkillState(this));
	BaseBossSkillState* skillState = stateMachine->GetState<BaseBossSkillState>("BaseSkill");

	skillState->AddSkillState("Stage2PageDashState", new Boss2PageDashState(this));
	skillState->AddSkillState("Boss2PageGroundPangState", new Boss2PageGroundPangState(this));
	skillState->AddSkillState("Boss2PageLightingState", new Boss2PageLightingState(this));
	skillState->AddSkillState("Boss1PageSmashSkillState", new Boss1PageSmashSkillState(this));
	skillState->AddSkillState("Boss1PageWorldDashSkillState", new Boss1PageWorldDashState(this));
	skillState->AddSkillState("Boss2PageSpawnBabyState", new Boss2PageSpawnBabyState(this));
	skillState->AddSkillState("Boss1PageSpawnBabyGroup", new Boss1PageSpawnBabyGroup(this));
}

Boss::~Boss()
{
}

void Boss::SetStat(float _skillCooldown)
{
	m_skillCooldown = _skillCooldown;
	m_initialized = true;
}

void Boss::ChangePhase(BossPhase phase)
{
	SetApplyDamage(false);
	m_currentBossPhase = phase;
	GetComponent<StateMachine>()->GetCurrentState()->ExitState();
	GetComponent<Animator>()->Stop();
	ChangeState("ChangePhase");
}
void Boss::StopAll()
{
	SetApplyDamage(false);
	GetComponent<StateMachine>()->GetCurrentState()->ExitState();
	GetComponent<Animator>()->Stop();
	ChangeState("Dead");
	
}


void Boss::ApplyDamage(int damage, Entity* dealer)
{
	if (!m_applyDamage)
		return;
	if(GetHealthCompo()->GetCurrentHealth() <= 0.f)
	{
		cout << "Dead" << endl;
		StopAll();
	}
	Entity::ApplyDamage(damage, dealer);
	if (OnDamageApplied != nullptr)
		OnDamageApplied(this, dealer);
	if (GetHealthCompo()->GetCurrentHealth() <= GetHealthCompo()->GetMaxHealth() / 2 && m_currentBossPhase == BossPhase::Phase1)
		ChangePhase(BossPhase::Phase2);



}

void Boss::EnterCollision(Collider* _other)
{
	cout << "EnterCollision" << endl;
	if (_other->IsTrigger())
	{
		if (_other->GetName() == L"PlayerBullet")
		{
			//GET_SINGLE(SceneManager)->RequestDestroy(this);
			//GET_SINGLE(SceneManager)->RequestDestroy(_other->GetOwner());
		}
	}
	// ¹°¸®
	else
	{

	}
}

Health* Boss::GetHealthCompo()
{
	return m_health;
}