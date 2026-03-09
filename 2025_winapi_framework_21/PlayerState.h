#pragma once
#include "State.h"

class Player;
class Animator;
class PlayerState 
	: public State
{
public:
	PlayerState(Player* _player);
	virtual ~PlayerState();
protected:
	Player* m_player;
	Animator* m_animator;
};
