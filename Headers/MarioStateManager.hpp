#pragma once

#define toStr(name) (#name)
// 声明一个模板
typedef std::function<void(bool)> FunUpdateAnimation;

class Mario;

class PowerState;
class MoveState;
class DeadState;

// 状态管理类
class StateManager
{
public:
    StateManager();
    ~StateManager() {}

    // 更新状态
    void update(Mario *mario, sf::RenderWindow &i_window);

    bool canDraw();

    void setPowerState(Mario *mario, bool isPowerUp);
    //是否为变大状态
    bool isPowerUpState();
    //是否为蹲下状态
    bool isCrouching();
    //是否为下降状态
    bool isFalling();
    //更新跳跃速度
    void updateJumpSpeed(const float i_value);
    //切换死亡
    void die(const short type, Mario &mario);

private:
    std::shared_ptr<PowerState> power_state;
    std::shared_ptr<MoveState> move_state;
    std::shared_ptr<DeadState> dead_state;
};
