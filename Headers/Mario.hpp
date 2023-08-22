#pragma once

#include <SFML/Graphics.hpp>

class StateManager;

class Mario
{
    std::string cacheFile;

    sf::Texture mario_texture;

    sf::Sprite mario_sprite;

public:
    StateManager state_manager;
    Mario();

    // 马里奥的位置
    int posX;
    int posY;

    bool flipped;

    /// @brief 更新马里奥的位置
    /// @param x
    /// @param y
    /// @param is_update_pos true更新精灵图和真实位置， false只更新精灵图位置
    void set_position(const int x, const int y, const bool is_update_pos = true);

    void set_flipped(const bool flipped);

    void update_texture(const std::string &file);
    // void set_position_x(const int x);
    // void set_position_y(const int y);

    void draw_mario(sf::RenderWindow &i_window);


    sf::FloatRect get_hit_box();
};