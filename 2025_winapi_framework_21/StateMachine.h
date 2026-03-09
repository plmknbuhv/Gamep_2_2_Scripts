#pragma once
#include "Component.h"
#include <unordered_map>
#include <type_traits>

class State;
class StateMachine
    : public Component
{
public:
    StateMachine();
    ~StateMachine();

public:
    void Init() override;
    void LateUpdate() override;
    void Render(HDC _hdc) override;
    
public:
    void AddState(std::string _stateName, State* _state);
    template<typename T>
    T* GetState(std::string stateName)
    {
        static_assert(std::is_base_of<State, T>::value, "State?¥ê??? ?????? ??");
        return dynamic_cast<T*>(m_stateList[stateName]);
    }
    void ChangeState(std::string _stateName);
    State* GetCurrentState();
    void ResetStateList();

private:
    State* m_currentState;
    State* m_previousState;
    std::unordered_map<std::string, State*> m_stateList;
};

