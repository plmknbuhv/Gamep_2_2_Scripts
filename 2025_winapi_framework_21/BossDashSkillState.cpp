#include "pch.h"
#include "Boss1PageDashSkillState.h"

Boss1PageDashSkillState::Boss1PageDashSkillState(StateMachine* _sm, Boss* _bs)
	: State(_sm, _bs)
{
}

void Boss1PageDashSkillState::EnterState()
{
	cout << "EnterBossDashSkillState" << endl;

	GetOwner<Boss>()->GetComponent<StateMachine>()->ChangeState("Idle");
}