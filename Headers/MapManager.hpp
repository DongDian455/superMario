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
    //地图信息
    sf::Image map_sketch;
    //地图精灵
    sf::Texture map_texture;
    //元素精灵
    sf::Sprite cell_sprite;

    GameMap map;

public:
    MapManager();
    //将图片素材转换成元素
    void convert_map_to_cell(const std::string &filePath);
    //根据像素转换成对应的Cell
    Cell createCellByPixel(sf::Color &pixel);

    //获取元素的绘制区域
    std::pair<short,short> get_cell_rect(const short posX ,const short posY);
    //获取背景的绘制区域
    std::pair<short,short> get_bg_rect(const short posX ,const short posY,const short map_height);

    //获取整个地图元素个数
    int get_map_size();
    
    //绘制地图
    void draw_map(sf::RenderWindow &i_window,const bool draw_bg);

};
