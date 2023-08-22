#include <cmath>

#include <SFML/Graphics.hpp>
#include "Headers/GlobalConfig.hpp"
#include "Headers/MarioStateManager.hpp"
#include "Headers/Mario.hpp"

Mario::Mario() : posX(0), posY(0), flipped(0), state_manager(std::make_shared<StateManager>(StateManager()))
{

    mario_sprite.setTexture(mario_texture);
    mario_sprite.setPosition(posX, posY);
}

void Mario::set_position(const int x, const int y, const bool is_update_pos)
{
    if (x >= 0 && y >= 0)
    {
        if (is_update_pos)
        {
            posX = x;
            posY = y;
        }
        mario_sprite.setPosition(x, y);
    }
}

void Mario::set_flipped(const bool flipped)
{
    this->flipped = flipped;
}

void Mario::update_texture(const std::string &file)
{
    if (cacheFile == file)
    {
        return;
    }
    mario_texture.loadFromFile(file);
    cacheFile = file;
}

sf::FloatRect Mario::get_hit_box()
{
    // The hitbox will be small if Mario is small or crouching.
    if (!state_manager->isPowerUpState() || state_manager->isCrouching())
    {
        return sf::FloatRect(posX, posY, CELL_SIZE, CELL_SIZE);
    }
    else
    {
        return sf::FloatRect(posX, posY, CELL_SIZE, 2 * CELL_SIZE);
    }
}

void Mario::draw_mario(sf::RenderWindow &i_window)
{
    mario_sprite.setPosition(round(posX), round(posY));
    // 绘制前更新状态
    state_manager->update(this, i_window);

    if (state_manager->canDraw())
    {
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
}

void Mario::reset()
{
    posX = 0;
    posY = 0;
    flipped = 0;
    state_manager = std::make_shared<StateManager>(StateManager());
}
