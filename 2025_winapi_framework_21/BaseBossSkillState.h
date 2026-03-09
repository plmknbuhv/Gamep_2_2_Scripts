#pragma once
#include "State.h"
#include <unordered_map>

class Boss;
class BaseBossSkillState :
    public State
{
public:
	BaseBossSkillState(Boss* _bs);
	~BaseBossSkillState();

public:
	virtual void EnterState() override;

public:
	void AddSkillState(std::string _skillName, State* _skillState);
	void ChooseRandSkill();

public:
	std::unordered_map<std::string, State*> m_skillStateList;
};

