#pragma once

#include <map>
#include <string>
#include "Animation.hpp"

#define toStr(name) (#name)

class Mario;

class StateMachine;

// 状态类
class State
{
public:
    virtual void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window) = 0;
    virtual bool canDrawMario()
    {
        return true;
    };
};

class IdelState : public State
{
public:
    void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window);
};

class MoveState : public State
{
    bool isCanDraw;
    float horizontal_speed;

    float vertical_speed;
    unsigned char jump_timer;

    Animation animation;

    void handleHorizontal(Mario *mario);
    void handleVertical(Mario *mario);

public:
    MoveState();
    void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window) override;
    bool canDrawMario() override;
};

class DeadState : public State
{
public:
    void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window) override;
};

class CrouchingState : public State
{
public:
    void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window) override;
};

// 状态机类
class StateMachine
{
public:
    StateMachine();

    void setState(std::string &&clazz);

    void update(Mario *mario, sf::RenderWindow &i_window);

    bool canDraw();

private:
    std::map<std::string, State *> m_states;
    State *m_currentState;
};
