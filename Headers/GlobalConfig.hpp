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
