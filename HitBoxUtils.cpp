#include <algorithm>
#include <SFML/Graphics.hpp>

#include "Headers/GlobalConfig.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/HitBoxUtils.hpp"

/// @brief
/// @param i_hitbox
/// @return  ture 有碰撞，fale没碰撞
bool HitBoxUtils::check_mario_hit_box(const sf::FloatRect &i_hitbox)
{

    std::vector<unsigned char> collision;

    collision = MapManager::get_instance().map_collision({Cell::ActivatedQuestionBlock, Cell::Brick, Cell::Pipe, Cell::QuestionBlock, Cell::Wall}, i_hitbox);

    return std::all_of(collision.begin(), collision.end(), [](const unsigned char i_value)
                       { return 0 == i_value; }) == 0;
}