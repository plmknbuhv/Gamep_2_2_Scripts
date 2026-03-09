#include "pch.h"
#include "BaseBossSkillState.h"
#include "Boss.h"
#include "StateMachine.h"

BaseBossSkillState::BaseBossSkillState(Boss* _bs)
	: State(_bs), m_skillStateList()
{
}

BaseBossSkillState::~BaseBossSkillState()
{
	m_skillStateList.clear();
}

void BaseBossSkillState::EnterState()
{
	ChooseRandSkill();
}

void BaseBossSkillState::AddSkillState(std::string _skillName, State* _skillState)
{
	GetOwner<Boss>()->GetComponent<StateMachine>()->AddState(_skillName, _skillState);
	if(m_skillStateList.find(typeid(_skillState).name()) != m_skillStateList.end())
	{
		assert(false && L"이미 존재하는 스킬 상태입니다.");
		return;
	}
	m_skillStateList[_skillName] = _skillState;
}

void BaseBossSkillState::ChooseRandSkill()
{
	if (m_skillStateList.empty())
	{
		cout << "No skill states in BaseBossSkillState." << endl;
		return;
	}

	static bool seeded = false;
	if (!seeded) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		seeded = true;
	}

	int idx = std::rand() % m_skillStateList.size();

	auto it = m_skillStateList.begin();
	std::advance(it, idx);

	GetOwner<Boss>()->GetComponent<StateMachine>()->ChangeState(it->first);
}