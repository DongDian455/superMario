
#include <iostream>
#include <cmath>

#include <SFML/Graphics.hpp>
#include "Headers/GlobalConfig.hpp"
#include "Headers/MarioState.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Animation.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/HitBoxUtils.hpp"

void IdelState::handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window)
{

    mario->update_texture("Resources/Images/MarioIdle.png");

    if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        i_state_machine.setState(toStr(MoveState));
        i_state_machine.update(mario, i_window);
    }
    else if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
             1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        i_state_machine.setState(toStr(MoveState));
        i_state_machine.update(mario, i_window);
    }
    else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        i_state_machine.setState(toStr(MoveState));
        i_state_machine.update(mario, i_window);
    }
}

MoveState::MoveState() : isCanDraw(0), horizontal_speed(0), vertical_speed(0), jump_timer(0), animation(CELL_SIZE, "Resources/Images/MarioWalk.png", MARIO_WALK_ANIMATION_SPEED)
{
}

bool MoveState::canDrawMario()
{

    return isCanDraw;
}

void MoveState::handleHorizontal(Mario *mario)
{
    if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        horizontal_speed = std::max(horizontal_speed - MARIO_WALK_ACCELERATION, -MARIO_WALK_SPEED);
    }
    else if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
             1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        horizontal_speed = std::min(MARIO_WALK_ACCELERATION + horizontal_speed, MARIO_WALK_SPEED);
    }
    else if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (0 < horizontal_speed)
        {
            horizontal_speed = std::max<float>(0, horizontal_speed - MARIO_WALK_ACCELERATION);
        }
        else if (0 > horizontal_speed)
        {
            horizontal_speed = std::min<float>(0, MARIO_WALK_ACCELERATION + horizontal_speed);
        }
    }

    // 移动之后要检测是否发生了碰撞，如果碰撞了需要重新设置移动位置
    sf::FloatRect hit_box = mario->get_hit_box();
    hit_box.left += horizontal_speed;

    if (HitBoxUtils::check_hit_box(hit_box))
    {
        if (0 < horizontal_speed)
        {
            (mario->posX) = CELL_SIZE * (ceil((horizontal_speed + (mario->posX)) / CELL_SIZE) - 1);
        }
        else if (0 > horizontal_speed)
        {
            (mario->posX) = CELL_SIZE * (1 + floor((horizontal_speed + (mario->posX)) / CELL_SIZE));
        }

        horizontal_speed = 0;
    }
    else
    {
        // 没有碰撞就直接更新位置
        (mario->posX) += horizontal_speed;
    }
}

void MoveState::handleVertical(Mario *mario)
{
    sf::FloatRect hit_box = mario->get_hit_box();
    hit_box.top++;
    if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (0 == vertical_speed && 1 == HitBoxUtils::check_hit_box(hit_box))
        {
            //  速度为0，并且是在地上的时候才能添加速度和重置跳高时间
            vertical_speed = MARIO_JUMP_SPEED;
            jump_timer = MARIO_JUMP_TIMER;
        }
        else if (0 < jump_timer) // The longer we press the jump button, the higher Mario jumps.
        {
            vertical_speed = MARIO_JUMP_SPEED;

            jump_timer--;
        }
        else
        {
            vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
        }
    }
    else
    {
        vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

        jump_timer = 0;
    }

    hit_box = mario->get_hit_box();
    hit_box.top += vertical_speed;

    if (HitBoxUtils::check_hit_box(hit_box))
    {
        // 有碰撞
        if (0 > vertical_speed)
        {
            //检测是否碰撞到金币
            HitBoxUtils::check_mario_hit_coin(hit_box);

            // 上升
            mario->posY = CELL_SIZE * (1 + floor((vertical_speed + mario->posY) / CELL_SIZE));
        }
        else if (0 < vertical_speed)
        {
            // 下落
            mario->posY = CELL_SIZE * (ceil((vertical_speed + mario->posY) / CELL_SIZE) - 1);
        }
        vertical_speed = 0;
        jump_timer = 0;
    }
    else
    {
        // 更新马里奥垂直位置
        (mario->posY) += vertical_speed;
    }
}

void MoveState::handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window)
{

    isCanDraw = 1;

    handleHorizontal(mario);
    handleVertical(mario);

    // 是否反转方向
    if (0 < horizontal_speed)
    {
        mario->set_flipped(0);
    }
    else if (0 > horizontal_speed)
    {
        mario->set_flipped(1);
    }
    // 再次检测碰撞
    sf::FloatRect hit_box = mario->get_hit_box();
    hit_box.top++;
    bool on_ground = HitBoxUtils::check_hit_box(hit_box);
    if (horizontal_speed == 0 && on_ground == 1)
    {
        i_state_machine.setState(toStr(IdelState));
    }
    else if (0 == on_ground)
    {
        mario->update_texture("Resources/Images/MarioJump.png");
    }
    else if ((0 < horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
              1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
             (0 > horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
              1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

    {
        mario->update_texture("Resources/Images/MarioBrake.png");
    }
    else
    {
        isCanDraw = 0;
        // 动画更新
        animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
        animation.update();

        animation.set_flipped(mario->flipped);
        animation.set_position(round(mario->posX), round(mario->posY));
        animation.draw(i_window);
    }
}
void DeadState::handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window)
{
}

void CrouchingState::handle(StateMachine &i_state_machine, Mario *mario, sf::RenderWindow &i_window)
{
}

StateMachine::StateMachine()
{
    m_states[toStr(IdelState)] = new IdelState();
    m_states[toStr(MoveState)] = new MoveState();
    m_states[toStr(CrouchingState)] = new CrouchingState();
    m_states[toStr(DeadState)] = new DeadState();
}

void StateMachine::setState(std::string &&clazz)
{
    m_currentState = m_states[clazz];
    std::cout << "setState:" << clazz << std::endl;
}

void StateMachine::update(Mario *mario, sf::RenderWindow &i_window)
{
    m_currentState->handle(*this, mario, i_window);
}

bool StateMachine::canDraw()
{
    return m_currentState->canDrawMario();
}
