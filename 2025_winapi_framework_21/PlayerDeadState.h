#pragma once
#include "PlayerState.h"
class PlayerDeadState :
    public PlayerState
{
public:
    PlayerDeadState(Player* _player);
    ~PlayerDeadState();
private:
    void EnterState() override;
};

