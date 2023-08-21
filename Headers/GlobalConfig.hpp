#pragma once

#include <chrono>

constexpr unsigned short SCREEN_HEIGHT = 240;
constexpr unsigned short SCREEN_WIDTH = 320;
constexpr unsigned char SCREEN_RESIZE = 2;

constexpr std::chrono::microseconds FRAME_DURATION(16667);

constexpr unsigned short CELL_SIZE = 16;

constexpr float MARIO_WALK_ACCELERATION = 0.25f;
constexpr float MARIO_WALK_SPEED = 2;
constexpr unsigned short MARIO_WALK_ANIMATION_SPEED = 4;

constexpr float GRAVITY = 0.25f;
constexpr float MARIO_JUMP_SPEED = -4;
constexpr float MAX_VERTICAL_SPEED = 8;
constexpr unsigned char MARIO_JUMP_TIMER = 8;

constexpr unsigned short COIN_ANIMATION_SPEED = 4;
constexpr unsigned short QUESTION_BLOCK_ANIMATION_SPEED = 8;

constexpr float BRICK_PARTICLE_SPEED = 4;
constexpr float COIN_JUMP_SPEED = -6;
//屏幕外的绘制范围
constexpr unsigned short ENTITY_UPDATE_AREA = 64;

constexpr float MUSHROOM_SPEED = 1;

struct MoveCell
{
    float horizontal_speed;
    float vertical_speed;
    float x;
    float y;

    MoveCell(const float i_x, const float i_y, const float i_horizontal_speed = 0, const float i_vertical_speed = 0) : horizontal_speed(i_horizontal_speed),
                                                                                                                       vertical_speed(i_vertical_speed),
                                                                                                                       x(i_x),
                                                                                                                       y(i_y)
    {
    }
};