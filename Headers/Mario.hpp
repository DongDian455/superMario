#pragma once

#include <SFML/Graphics.hpp>

class StateMachine;

class Mario
{

    sf::Texture mario_texture;

    sf::Sprite mario_sprite;

    StateMachine state_machine;

public:
    Mario();

    // 马里奥的位置
    unsigned int posX;
    unsigned int posY;

    bool flipped;

    void set_position(const int x, const int y);

    void set_flipped(const bool flipped);

    void update_texture(const std::string &file);
    // void set_position_x(const int x);
    // void set_position_y(const int y);

    void draw_mario(sf::RenderWindow &i_window);

    void update_pos(sf::RenderWindow &i_window);

    sf::FloatRect get_hit_box() const;
};