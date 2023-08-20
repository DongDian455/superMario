
#include <cmath>
#include <string>
#include <memory>
#include "Headers/GlobalConfig.hpp"
#include <SFML/Graphics.hpp>
#include "Headers/MapManager.hpp"

void MapManager::convert_map_to_cell(const std::string &filePath)
{
    map_sketch.loadFromFile(filePath);

    map.resize(map_sketch.getSize().x);

    unsigned short map_height = floor(static_cast<float>(map_sketch.getSize().y)) / MAP_SKETCH_LAYER;

    for (unsigned short a = 0; a < map_sketch.getSize().x; a++)
    {

        for (unsigned short b = 0; b < map_height; b++)
        {
            sf::Color pixel = map_sketch.getPixel(a, b);

            map[a][b] = createCellByPixel(pixel);
        }
    }
}

Cell MapManager::createCellByPixel(sf::Color &pixel)
{

    if (sf::Color(182, 73, 0) == pixel)
    {
        return Cell::Brick;
    }
    else if (sf::Color(255, 255, 0) == pixel)
    {
        return Cell::Coin;
    }
    else if (sf::Color(0, 146, 0) == pixel || sf::Color(0, 182, 0) == pixel || sf::Color(0, 219, 0) == pixel)
    {
        return Cell::Pipe;
    }
    else if (sf::Color(255, 73, 85) == pixel || sf::Color(255, 146, 85) == pixel)
    {
        return Cell::QuestionBlock;
    }
    else if (sf::Color(0, 0, 0) == pixel || sf::Color(146, 73, 0) == pixel)
    {
        return Cell::Wall;
    }
    else
    {
        return Cell::Empty;
    }
}

std::pair<short, short> MapManager::get_cell_rect(const unsigned int posX, const unsigned int posY)
{
    unsigned short sprite_x = 0;
    unsigned short sprite_y = 0;

    if (Cell::ActivatedQuestionBlock == map[posX][posY])
    {
        sprite_x = 6;
        sprite_y++;
    }
    else if (Cell::Pipe == map[posX][posY])
    {
        if (sf::Color(0, 182, 0) == map_sketch.getPixel(posX, posY))
        {
            sprite_y = 1;

            if (Cell::Pipe == map[posX - 1][posY])
            {
                sprite_x = 11;
            }
            else
            {
                sprite_x = 10;
            }
        }
        else if (sf::Color(0, 146, 0) == map_sketch.getPixel(posX, posY))
        {
            sprite_y = 0;

            if (sf::Color(0, 146, 0) == map_sketch.getPixel(posX - 1, posY))
            {
                sprite_x = 11;
            }
            else if (sf::Color(0, 146, 0) == map_sketch.getPixel(1 + posX, posY))
            {
                sprite_x = 10;
            }
            else
            {
                sprite_x = 10;

                if (sf::Color(0, 146, 0) == map_sketch.getPixel(posX, posY - 1))
                {
                    sprite_y = 3;
                }
                else
                {
                    sprite_y = 2;
                }
            }
        }
        else if (sf::Color(0, 219, 0) == map_sketch.getPixel(posX, posY))
        {
            if (sf::Color(0, 182, 0) == map_sketch.getPixel(1 + posX, posY))
            {
                sprite_x = 12;
            }
            else
            {
                sprite_x = 11;
            }

            if (sf::Color(0, 219, 0) == map_sketch.getPixel(posX, posY - 1))
            {
                sprite_y = 3;
            }
            else
            {
                sprite_y = 2;
            }
        }
    }
    else if (Cell::Wall == map[posX][posY])
    {
        if (sf::Color(0, 0, 0) == map_sketch.getPixel(posX, posY))
        {
            sprite_x = 2;
        }
        else
        {
            sprite_x = 3;
        }
    }

    return std::pair(sprite_x, sprite_y);
}

std::pair<short, short> MapManager::get_bg_rect(const unsigned int posX, const unsigned int posY, const unsigned int map_height)
{
    unsigned short sprite_x = 0;
    unsigned short sprite_y = 0;

    sf::Color pixel = map_sketch.getPixel(posX, posY + 2 * map_height);
    sf::Color pixel_down = sf::Color(0, 0, 0, 0);
    sf::Color pixel_left = sf::Color(0, 0, 0, 0);
    sf::Color pixel_right = sf::Color(0, 0, 0, 0);
    sf::Color pixel_up = sf::Color(0, 0, 0, 0);

    if (255 == pixel.a)
    {
        if (0 < posX)
        {
            pixel_left = map_sketch.getPixel(posX - 1, posY + 2 * map_height);
        }

        if (0 < posY)
        {
            pixel_up = map_sketch.getPixel(posX, posY + 2 * map_height - 1);
        }

        if (posX < map_sketch.getSize().x - 1)
        {
            pixel_right = map_sketch.getPixel(1 + posX, posY + 2 * map_height);
        }

        if (posY < map_height - 1)
        {
            pixel_down = map_sketch.getPixel(posX, 1 + posY + 2 * map_height);
        }

        if (sf::Color(255, 255, 255) == pixel)
        {
            sprite_x = 8;

            if (sf::Color(255, 255, 255) == pixel_up)
            {
                sprite_y = 1;
            }

            if (sf::Color(255, 255, 255) == pixel_left)
            {
                if (sf::Color(255, 255, 255) != pixel_right)
                {
                    sprite_x = 9;
                }
            }
            else if (sf::Color(255, 255, 255) == pixel_right)
            {
                sprite_x = 7;
            }
        }
        else if (sf::Color(146, 219, 0) == pixel)
        {
            sprite_x = 5;
        }
        else if (sf::Color(146, 146, 0) == pixel)
        {
            sprite_x = 4;
        }
        else if (sf::Color(146, 182, 0) == pixel)
        {
            sprite_x = 6;
        }
        else if (sf::Color(0, 73, 0) == pixel)
        {
            sprite_y = 1;

            if (sf::Color(0, 109, 0) == pixel_left)
            {
                if (sf::Color(0, 109, 0) != pixel_right)
                {
                    sprite_x = 2;
                }
            }
            else if (sf::Color(0, 109, 0) == pixel_right)
            {
                sprite_x = 1;
            }
        }
        else if (sf::Color(0, 109, 0) == pixel)
        {
            sprite_y = 1;

            if (sf::Color(0, 73, 0) == pixel_left)
            {
                sprite_x = 3;
            }
            else if (sf::Color(0, 73, 0) == pixel_right)
            {
                sprite_x = 5;
            }
            else
            {
                sprite_x = 4;
            }
        }
        else if (sf::Color(109, 255, 85) == pixel)
        {
            sprite_x = 12;

            if (sf::Color(109, 255, 85) == pixel_up)
            {
                sprite_y = 1;
            }
        }
        return std::pair(sprite_x, sprite_y);
    }
    else
    {
        return std::pair(-1, -1);
    }
}

int MapManager::get_map_size()
{
    return map.size();
}

void MapManager::draw_map(sf::RenderWindow &i_window, const bool draw_bg)
{
    unsigned short map_end = ceil((SCREEN_WIDTH) / static_cast<float>(CELL_SIZE));
    unsigned short map_start = floor(0 / static_cast<float>(CELL_SIZE));
    unsigned short map_height = floor(static_cast<float>(map_sketch.getSize().y) / MAP_SKETCH_LAYER);

    for (unsigned short a = map_start; a < map_end; a++)
    {
        for (unsigned short b = 0; b < map_height; b++)
        {
            cell_sprite.setPosition(CELL_SIZE * a, CELL_SIZE * b);

            // 获取转换的绘制位置
            std::pair<short, short> spritePos(-1, -1);
            if (draw_bg)
            {
                spritePos = get_bg_rect(a, b, map_height);
            }
            else if (Cell::Empty != map[a][b])
            {
                spritePos = get_cell_rect(a, b);
            }

            if (spritePos.first >= 0)
            {
                // 设置纹理的区域
                cell_sprite.setTextureRect(sf::IntRect(CELL_SIZE * spritePos.first, CELL_SIZE * spritePos.second, CELL_SIZE, CELL_SIZE));
                // 绘制精灵图层
                i_window.draw(cell_sprite);
            }
        }
    }
}

std::pair<short, short> MapManager::get_mario_birth_pos() const
{
    unsigned short map_end = ceil((SCREEN_WIDTH) / static_cast<float>(CELL_SIZE)) * 0.5f;
    unsigned short map_start = 0;
    unsigned short map_height = floor(static_cast<float>(map_sketch.getSize().y) / MAP_SKETCH_LAYER);

    for (unsigned short a = map_start; a < map_end; a++)
    {
        // 从第二层开始遍历
        for (unsigned short b = map_height; b < map_height * 2; b++)
        {

            sf::Color pixel = map_sketch.getPixel(a, b);
            // this color is unqiue in the sketch‘ second layer
            if (sf::Color(255, 0, 0) == pixel)
            {
                return std::pair(CELL_SIZE * a, CELL_SIZE * (b - map_height));
            }
        }
    }

    return std::pair(-1, -1);
}

std::vector<unsigned char> MapManager::map_collision(const std::vector<Cell> &i_check_cells, std::vector<sf::Vector2i> &i_collision_cells, const sf::FloatRect &i_hitbox) const
{
    std::vector<unsigned char> output;

    i_collision_cells.clear();

    for (short a = floor(i_hitbox.top / CELL_SIZE); a <= floor((ceil(i_hitbox.height + i_hitbox.top) - 1) / CELL_SIZE); a++)
    {
        output.push_back(0);

        for (short b = floor(i_hitbox.left / CELL_SIZE); b <= floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE); b++)
        {
            if (0 <= b && b < map.size())
            {
                if (0 <= a && a < map[0].size())
                {
                    if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), map[b][a]))
                    {
                        // Since C++ doesn't support returning 2 vectors, we're gonna change the argument vector.
                        i_collision_cells.push_back(sf::Vector2i(b, a));

                        output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
                    }
                }
            }
            else if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), Cell::Wall))
            {
                output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
            }
        }
    }

    return output;
}

std::vector<unsigned char> MapManager::map_collision(const std::vector<Cell> &i_check_cells, const sf::FloatRect &i_hitbox) const
{
    std::vector<unsigned char> output;
    // 因为map存储的是每个方块，而每个方块占CELL_SIZE的大小，所以这里除以CELL_SIZE就能得到具体在哪个方块
    for (short a = floor(i_hitbox.top / CELL_SIZE); a <= floor((ceil(i_hitbox.height + i_hitbox.top) - 1) / CELL_SIZE); a++)
    {
        output.push_back(0);

        for (short b = floor(i_hitbox.left / CELL_SIZE); b <= floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE); b++)
        {
            if (0 <= b && b < map.size())
            {
                if (0 <= a && a < map[0].size())
                {
                    if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), map[b][a]))
                    {
                        // We're gonna return a vector of numbers. Each number is a binary representation of collisions in a single row.
                        output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
                    }
                }
            }
            // We're assuming that the map borders have walls.
            else if (i_check_cells.end() != std::find(i_check_cells.begin(), i_check_cells.end(), Cell::Wall))
            {
                output[a - floor(i_hitbox.top / CELL_SIZE)] += pow(2, floor((ceil(i_hitbox.left + i_hitbox.width) - 1) / CELL_SIZE) - b);
            }
        }
    }

    return output;
}