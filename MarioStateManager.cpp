

#include <cmath>
#include <memory>

#include "Headers/GlobalConfig.hpp"
#include <SFML/Graphics.hpp>
#include "Headers/Animation.hpp"

#include "Headers/Mario.hpp"

#include "Headers/HitBoxUtils.hpp"
#include "Headers/AudioManager.hpp"
#include "Headers/MarioStateManager.hpp"

class PowerState
{
    unsigned short growth_timer;
    unsigned short invincible_timer;
    bool is_power_up;

public:
    PowerState() : growth_timer(0), invincible_timer(0), is_power_up(0)
    {
    }

    bool isInvincible()
    {
        return invincible_timer / MARIO_BLINKING % 2 != 0;
    }

    bool isPowerUp()
    {
        return is_power_up;
    }

    void setPower(Mario *mario, const bool power)
    {

        is_power_up = power;
        if (isPowerUp())
        {
            invincible_timer = 0;
            AudioManager::get_instance().playPowerUpEffect();
            mario->set_position(mario->posX, mario->posY -= CELL_SIZE);

            printf("变大\n");
            growth_timer = MARIO_GROWTH_DURATION;
        }
        else
        {
            printf("变小\n");
        }
    }

    void update(Mario *mario,
                const std::string normal,
                const std::string power,
                FunUpdateAnimation *fun)
    {
        if (isPowerUp())
        {
            if (0 < growth_timer)
            {
                growth_timer--;
            }

            bool draw_big = 0 == growth_timer / MARIO_BLINKING % 2;

            if (draw_big)
            {
                mario->update_texture(power);
                if (fun != nullptr)
                {
                    (*fun)(true);
                }
            }
            else
            {
                mario->set_position(round(mario->posX), round(CELL_SIZE + mario->posY), false);
                mario->update_texture(normal);
                if (fun != nullptr)
                {
                    (*fun)(false);
                }
            }
        }
        else
        {
            bool is_draw = true;
            if (invincible_timer > 0)
            {
                invincible_timer--;

                is_draw = invincible_timer / MARIO_BLINKING % 2;
            }

            if (is_draw)
            {
                mario->update_texture(normal);
                if (fun != nullptr)
                {
                    (*fun)(false);
                }
            }
        }
    }

    bool toDie(Mario &mario)
    {
        bool is_dead = 0 == growth_timer && 0 == invincible_timer && is_power_up == 0;

        if (is_power_up)
        {
            AudioManager::get_instance().playPowerDownEffect();
            is_power_up = 0;
            mario.set_position(mario.posX, mario.posY += CELL_SIZE);
            invincible_timer = MARIO_INVINCIBILITY_DURATION;
        }

        return is_dead;
    }
};

class MoveState
{
    bool isCanDraw;

    bool crouching;

    float horizontal_speed;

    float vertical_speed;
    unsigned char jump_timer;

    Animation walk_animation;
    Animation big_walk_animation;
    std::shared_ptr<PowerState> power_up_state;

    void handleHorizontal(Mario *mario)
    {

        if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
            1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && crouching == 0)
        {
            horizontal_speed = std::max(horizontal_speed - MARIO_WALK_ACCELERATION, -MARIO_WALK_SPEED);
        }
        else if (0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                 1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && crouching == 0)
        {
            horizontal_speed = std::min(MARIO_WALK_ACCELERATION + horizontal_speed, MARIO_WALK_SPEED);
        }
        else if ((0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || crouching)
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

    void handleVertical(Mario *mario)
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
                AudioManager::get_instance().playJumpEffect();
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
                // 检测是否碰撞到金币
                HitBoxUtils::check_mario_hit_coin(hit_box);

                if (power_up_state->isPowerUp())
                {
                    // 检测是否碰撞木块
                    HitBoxUtils::check_mario_hit_bricks(hit_box);
                }

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

    void handleCrouch(Mario *mario)
    {

        if (power_up_state->isPowerUp())
        {

            if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (0 == crouching)
                {
                    // 蹲下
                    crouching = 1;

                    mario->set_position(mario->posX, mario->posY + CELL_SIZE);
                    mario->update_texture("Resources/Images/BigMarioCrouch.png");
                }
            }
            else if (crouching)
            {
                // 起身
                crouching = 0;

                mario->set_position(mario->posX, mario->posY - CELL_SIZE);
            }
        }
    }

public:
    MoveState(std::shared_ptr<PowerState> &p) : isCanDraw(0),
                                                crouching(0),
                                                horizontal_speed(0),
                                                vertical_speed(0),
                                                jump_timer(0),
                                                walk_animation(CELL_SIZE, "Resources/Images/MarioWalk.png", MARIO_WALK_ANIMATION_SPEED),
                                                big_walk_animation(CELL_SIZE, "Resources/Images/BigMarioWalk.png", MARIO_WALK_ANIMATION_SPEED),
                                                power_up_state(p)

    {
    }
    void handle(Mario *mario, sf::RenderWindow &i_window)
    {
        isCanDraw = 1;

        handleCrouch(mario);
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

        if (crouching)
        {
            mario->update_texture("Resources/Images/BigMarioCrouch.png");
        }
        else if (horizontal_speed == 0 && on_ground == 1)
        {
            power_up_state->update(mario, "Resources/Images/MarioIdle.png", "Resources/Images/BigMarioIdle.png", nullptr);
        }

        else if (0 == on_ground)
        {
            power_up_state->update(mario, "Resources/Images/MarioJump.png", "Resources/Images/BigMarioJump.png", nullptr);
        }
        else if ((0 < horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
                  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
                 (0 > horizontal_speed && 0 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                  1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))

        {
            power_up_state->update(mario, "Resources/Images/MarioBrake.png", "Resources/Images/BigMarioBrake.png", nullptr);
        }
        else
        {
            isCanDraw = 0;

            FunUpdateAnimation func = [this, mario, &i_window](bool isPowerUp) -> void
            {
                if (isPowerUp)
                {
                    // 动画更新
                    big_walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
                    big_walk_animation.update();

                    big_walk_animation.set_flipped(mario->flipped);
                    big_walk_animation.set_position(round(mario->posX), round(mario->posY));
                    big_walk_animation.draw(i_window);
                }
                else
                {
                    // 动画更新
                    walk_animation.set_animation_speed(MARIO_WALK_ANIMATION_SPEED * MARIO_WALK_SPEED / abs(horizontal_speed));
                    walk_animation.update();

                    walk_animation.set_flipped(mario->flipped);
                    walk_animation.set_position(round(mario->posX), round(mario->posY));
                    walk_animation.draw(i_window);
                }
            };

            power_up_state->update(mario, "Resources/Images/MarioBrake.png", "Resources/Images/BigMarioBrake.png", &func);
        }

        if (mario->posY >= SCREEN_HEIGHT - mario->get_hit_box().height)
        {
            (mario->state_manager)->die(0, *mario);
        }
    }
    bool canDrawMario()
    {
        return isCanDraw && !power_up_state->isInvincible();
    };

    bool isCrouching()
    {
        return crouching;
    };

    bool isFalling()
    {
        return vertical_speed > 0;
    };
    void updateJumpSpeed(const float i_value)
    {
        vertical_speed = i_value;
    };
};

class DeadState
{
    float vertical_speed;
    unsigned short death_timer;

public:
    DeadState() : death_timer(MARIO_DEATH_DURATION)
    {
    }

    void update(Mario &mario)
    {

        if (mario.posY > SCREEN_HEIGHT)
        {
            return;
        }
        if (0 == death_timer)
        {
            vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);
            mario.posY += vertical_speed;
        }
        else if (1 == death_timer)
        {
            vertical_speed = MARIO_JUMP_SPEED;
        }

        death_timer = std::max(0, death_timer - 1);
    }
};

StateManager::StateManager() : power_state(std::make_shared<PowerState>(PowerState())),
                               move_state(std::make_shared<MoveState>(MoveState(power_state))),
                               dead_state(nullptr)
{
}

void StateManager::update(Mario *mario, sf::RenderWindow &i_window)
{
    if (dead_state != nullptr)
    {
        dead_state->update(*mario);
    }
    else
    {
        move_state->handle(mario, i_window);
    }
}

bool StateManager::canDraw()
{
    if (dead_state != nullptr)
    {
        return true;
    }
    else
    {
        return move_state->canDrawMario();
    }
}

void StateManager::setPowerState(Mario *mario, bool isPowerUp)
{
    power_state->setPower(mario, isPowerUp);
}

bool StateManager::isPowerUpState()
{
    return power_state->isPowerUp();
}

bool StateManager::isCrouching()
{
    if (dead_state != nullptr)
    {
        return false;
    }
    else
    {
        return move_state->isCrouching();
    }
}

bool StateManager::isFalling()
{
    if (dead_state != nullptr)
    {
        return false;
    }
    else
    {
        return move_state->isFalling();
    }
}
void StateManager::updateJumpSpeed(const float i_value)
{
    if (dead_state == nullptr)
    {
        move_state->updateJumpSpeed(i_value);
    }
}

void StateManager::die(const short type, Mario &mario)
{
    if (dead_state != nullptr)
    {
        return;
    }

    if (0 == type)
    {
        // 立马死亡
        if (power_state->isPowerUp())
        {
            mario.update_texture("Resources/Images/BigMarioDeath.png");
        }
        else
        {
            mario.update_texture("Resources/Images/MarioDeath.png");
        }

        dead_state = std::make_shared<DeadState>(DeadState());
    }
    else if (power_state->toDie(mario))
    {

        AudioManager::get_instance().playDeadEffect();
        mario.update_texture("Resources/Images/MarioDeath.png");
        dead_state = std::make_shared<DeadState>(DeadState());
    }
}
