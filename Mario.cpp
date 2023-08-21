#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Headers/GlobalConfig.hpp"
#include "Headers/MarioState.hpp"
#include "Headers/Mario.hpp"

Mario::Mario() : posX(0), posY(0), flipped(0)
{
    state_machine.setState(toStr(IdelState));
    mario_sprite.setTexture(mario_texture);
    mario_sprite.setPosition(posX, posY);
}

void Mario::set_position(const int x, const int y)
{
    if (x >= 0 && y >= 0)
    {
        posX = x;
        posY = y;
        mario_sprite.setPosition(posX, posY);
    }
}

void Mario::set_flipped(const bool flipped)
{
    this->flipped = flipped;
}

void Mario::update_texture(const std::string &file)
{
    mario_texture.loadFromFile(file);
}

sf::FloatRect Mario::get_hit_box() const
{
    // The hitbox will be small if Mario is small or crouching.
    // if (1 == crouching || 0 == powerup_state)
    // {
    //     return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
    // }
    // else
    // {
    //     return sf::FloatRect(x, y, CELL_SIZE, 2 * CELL_SIZE);
    // }

    return sf::FloatRect(posX, posY, CELL_SIZE, CELL_SIZE);
}

// void Mario::set_position_x(const int x)
// {

//     if (x >= 0)
//     {
//         posX = x;
//     }
// }

// void Mario::set_position_y(const int y)
// {
//     if (y >= 0)
//     {
//         posY = y;
//     }
// }

void Mario::draw_mario(sf::RenderWindow &i_window)
{
    update_pos(i_window);

    

    if (state_machine.canDraw())
    {
        mario_sprite.setPosition(round(posX), round(posY));

        if (0 == flipped)
        {
            mario_sprite.setTextureRect(sf::IntRect(0, 0, mario_texture.getSize().x, mario_texture.getSize().y));
        }
        else
        {
            mario_sprite.setTextureRect(sf::IntRect(mario_texture.getSize().x, 0, -static_cast<int>(mario_texture.getSize().x), mario_texture.getSize().y));
        }
        i_window.draw(mario_sprite);
    }
    else
    {
    }
}

void Mario::update_pos(sf::RenderWindow &i_window)
{

    state_machine.update(this, i_window);
}