
#include <vector>
#include <SFML/Graphics.hpp>

#include "Headers/Animation.hpp"
#include "Headers/GlobalConfig.hpp"
#include "Headers/GenerateManager.hpp"

void GenerateManager::add_particles(const unsigned short i_x, const unsigned short i_y)
{
    // Adding brick particles.
    // I was too lazy to add randomness.
    // It still looks cool, in my opinion.
    brick_particles.push_back(MoveCell(i_x, i_y, -0.25f * BRICK_PARTICLE_SPEED, -1.5f * BRICK_PARTICLE_SPEED));
    brick_particles.push_back(MoveCell(i_x + 0.5f * CELL_SIZE, i_y, 0.25f * BRICK_PARTICLE_SPEED, -1.5f * BRICK_PARTICLE_SPEED));
    brick_particles.push_back(MoveCell(i_x, i_y + 0.5f * CELL_SIZE, -0.5f * BRICK_PARTICLE_SPEED, -BRICK_PARTICLE_SPEED));
    brick_particles.push_back(MoveCell(i_x + 0.5f * CELL_SIZE, i_y + 0.5f * CELL_SIZE, 0.5f * BRICK_PARTICLE_SPEED, -BRICK_PARTICLE_SPEED));
}

void GenerateManager::add_question_block_coin(const unsigned short i_x, const unsigned short i_y)
{
    question_block_coins.push_back(MoveCell(i_x, i_y, 0, COIN_JUMP_SPEED));
}

void GenerateManager::add_mushroom(const unsigned short i_x, const unsigned short i_y)
{
    mushrooms.push_back(Mushroom(i_x, i_y));
}

void GenerateManager::update(const unsigned int i_view_x)
{

    for (Mushroom &mushroom : mushrooms)
    {
        mushroom.update(i_view_x);
    }

    for (auto &question_block_coin : question_block_coins)
    {
        question_block_coin.vertical_speed += GRAVITY;

        question_block_coin.y += question_block_coin.vertical_speed;
    }

    for (auto &brick_particle : brick_particles)
    {
        brick_particle.vertical_speed += GRAVITY;

        brick_particle.x += brick_particle.horizontal_speed;
        brick_particle.y += brick_particle.vertical_speed;
    }

    brick_particles.erase(remove_if(brick_particles.begin(), brick_particles.end(), [](const auto &i_brick_particle)
                                    { return SCREEN_HEIGHT <= i_brick_particle.y; }),
                          brick_particles.end());

    question_block_coins.erase(remove_if(question_block_coins.begin(), question_block_coins.end(), [](const auto &i_question_block_coin)
                                         { return 0 <= i_question_block_coin.vertical_speed; }),
                               question_block_coins.end());

    coin_animation.update();
    question_block_animation.update();
}

void GenerateManager::draw_info(sf::RenderWindow &i_window, const unsigned int i_view_x)
{
    update(i_view_x);

    for (const auto &question_block_coin : question_block_coins)
    {
        coin_animation.set_position(question_block_coin.x, question_block_coin.y);
        coin_animation.draw(i_window);
    }

    for (Mushroom &mushroom : mushrooms)
    {
        mushroom.draw(i_view_x, i_window);
    }
}

void GenerateManager::draw_coin_animation(sf::RenderWindow &i_window, unsigned int x, unsigned int y)
{
    coin_animation.set_position(x, y);
    coin_animation.draw(i_window);
}

void GenerateManager::draw_question_animation(sf::RenderWindow &i_window, unsigned int x, unsigned int y)
{
    question_block_animation.set_position(x, y);
    question_block_animation.draw(i_window);
}
