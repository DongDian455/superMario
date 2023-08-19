#pragma once

#include <vector>
#include <array>

constexpr unsigned short MAP_SKETCH_LAYER = 3;

enum Cell
{
    ActivatedQuestionBlock,
    Brick,
    Coin,
    Empty,
    Pipe,
    QuestionBlock,
    Wall
};

typedef std::vector<std::array<Cell, SCREEN_HEIGHT / CELL_SIZE>> GameMap;

class MapManager
{
    sf::Image map_sketch;
    sf::Texture map_texture;
    sf::Sprite cell_sprite;

    GameMap map;

public:
    MapManager();

    void convert_map_to_cell(const std::string &filePath);
    std::pair<short,short> convert_cell_to_rect(const short posX ,const short posY);
    std::pair<short,short> convert_bg_to_rect(const short posX ,const short posY,const short map_height);

    int get_map_width();

    void draw_map(sf::RenderWindow &i_window,const bool draw_bg);


    Cell createCellByPixel(sf::Color &pixel);
};
