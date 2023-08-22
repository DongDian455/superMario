#pragma once

#define toStr(name) (#name)
// 声明一个模板
typedef std::function<void(bool)> FunUpdateAnimation;

class Mario;

class PowerState;
class MoveState;
class DeadState;

// 状态机类
class StateManager
{
public:
    StateManager();

    void update(Mario *mario, sf::RenderWindow &i_window);

    bool canDraw();

    void setPowerState(Mario *mario, bool isPowerUp);

    bool isPowerUpState();
    bool isCrouching();
    ~StateManager() {}

    bool isFalling();
    void updateJumpSpeed(const float i_value);
    void die(const short type, Mario &mario);

private:
    std::shared_ptr<PowerState> power_state;
    std::shared_ptr<MoveState> move_state;
    std::shared_ptr<DeadState> dead_state;
};
