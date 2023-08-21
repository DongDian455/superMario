#pragma once

#include "Mushroom.hpp"

class GenerateManager
{

    Animation coin_animation;
    Animation question_block_animation;

    std::vector<MoveCell> brick_particles;
    std::vector<MoveCell> question_block_coins;

    std::vector<Mushroom> mushrooms;

private:
    GenerateManager() : coin_animation(CELL_SIZE, "Resources/Images/Coin.png", COIN_ANIMATION_SPEED),
                        question_block_animation(CELL_SIZE, "Resources/Images/QuestionBlock.png", QUESTION_BLOCK_ANIMATION_SPEED)
    {
    }

    ~GenerateManager() {}
    GenerateManager(const GenerateManager &) = delete;
    GenerateManager &operator=(const GenerateManager &) = delete;

    void update(const unsigned int i_view_x, Mario &mario);

public:
    static GenerateManager &get_instance()
    {
        static GenerateManager instance;
        return instance;
    }

    // 碰撞产出的元素
    void add_particles(const unsigned short i_x, const unsigned short i_y);
    void add_question_block_coin(const unsigned short i_x, const unsigned short i_y);
    void add_mushroom(const unsigned short i_x, const unsigned short i_y);

    void draw_coin_animation(sf::RenderWindow &i_window, unsigned int x, unsigned int y);
    void draw_question_animation(sf::RenderWindow &i_window, unsigned int x, unsigned int y);

    // 绘制生成物体的信息
    void draw_info(sf::RenderWindow &i_window, const unsigned int i_view_x, Mario &mario);
};