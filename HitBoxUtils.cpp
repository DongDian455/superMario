#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Headers/GlobalConfig.hpp"
#include "Headers/Animation.hpp"
#include "Headers/MarioStateManager.hpp"
#include "Headers/Mario.hpp"
#include "Headers/GenerateManager.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/HitBoxUtils.hpp"
#include "Headers/AudioManager.hpp"

/// @brief 检测是否与能产生碰撞的元素发生了碰撞
/// @param i_hitbox
/// @return  ture 有碰撞，fale没碰撞
bool HitBoxUtils::check_hit_box(const sf::FloatRect &i_hitbox)
{

    std::vector<unsigned char> collision;

    collision = MapManager::get_instance().map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_hitbox);

    return std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
                       { return 0 == i_value; }) == 0;
}

/// @brief 检测是否撞击出金币
/// @param i_hitbox
/// @return  ture 有碰撞，fale没碰撞
void HitBoxUtils::check_mario_hit_coin(const sf::FloatRect &i_hitbox)
{

    std::vector<sf::Vector2i> cells;
    MapManager::get_instance().map_collision({Cell::QuestionBlock}, cells, i_hitbox);

    // Activating question blocks!!!!
    for (const sf::Vector2i &cell : cells)
    {
        MapManager::get_instance().set_map_cell(cell.x, cell.y, Cell::ActivatedQuestionBlock);

        // It can be either a mushroom or a coin, depending on the color of the pixel in the sketch.
        if (sf::Color(255, 73, 85) == MapManager::get_instance().get_map_sketch_pixel(cell.x, cell.y))
        {
            AudioManager::get_instance().playMushroomEffect();
            GenerateManager::get_instance().add_mushroom(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
        }
        else
        {
            AudioManager::get_instance().playCoinEffect();
            GenerateManager::get_instance().add_question_block_coin(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
        }
    }
}

void HitBoxUtils::check_mario_hit_bricks(const sf::FloatRect &i_hitbox)
{

    std::vector<sf::Vector2i> cells;
    MapManager::get_instance().map_collision({Cell::Brick}, cells, i_hitbox);

    // Activating question blocks!!!!
    for (const sf::Vector2i &cell : cells)
    {
        AudioManager::get_instance().playDestoryBrickEffect();
        MapManager::get_instance().set_map_cell(cell.x, cell.y, Cell::Empty);
        GenerateManager::get_instance().add_brick_particles(CELL_SIZE * cell.x, CELL_SIZE * cell.y);
    }
}