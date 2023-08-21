#pragma once

#include <map>
#include <string>
#include "Animation.hpp"

#define toStr(name) (#name)
// 声明一个模板
typedef std::function<void(bool)> FunUpdateAnimation;

class Mario;

class StateMachine;

class PowerState
{
    unsigned short growth_timer;
    bool is_power_up;
    unsigned short invincible_timer;

public:
    PowerState();

    bool isPowerUp();

    void setPower(const bool power);

    void update(Mario *mario,
                const std::string normal,
                const std::string power,
                FunUpdateAnimation *fun);
};

// 状态类
class State
{
protected:
    std::shared_ptr<PowerState> power_up_state;

public:
    virtual void handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window) = 0;
    virtual bool canDrawMario()
    {
        return true;
    }

    virtual bool isCrouching()
    {
        return false;
    }
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

    void setPowerState(bool isPowerUp);

    bool isPowerUpState();
    bool isCrouching();

private:
    std::map<std::string, State *> m_states;
    State *m_currentState;
    std::shared_ptr<PowerState> power_up_state;
};
