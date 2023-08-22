#pragma once

#include <vector>
#include <array>

class Animation;

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
    // 地图信息
    sf::Image map_sketch;
    // 地图精灵
    sf::Texture map_texture;
    // 元素精灵
    sf::Sprite cell_sprite;
    // 地图元素信息
    GameMap map;

    Animation coin_animation;
    Animation question_block_animation;

    std::vector<MoveCell> brick_particles;
    std::vector<MoveCell> question_block_coins;

private:
    MapManager() : coin_animation(CELL_SIZE, "Resources/Images/Coin.png", COIN_ANIMATION_SPEED),
                   question_block_animation(CELL_SIZE, "Resources/Images/QuestionBlock.png", QUESTION_BLOCK_ANIMATION_SPEED)
    {

        map_texture.loadFromFile("Resources/Images/Map.png");
        cell_sprite.setTexture(map_texture);
    }

    ~MapManager() {}
    MapManager(const MapManager &) = delete;
    MapManager &operator=(const MapManager &) = delete;

public:
    static MapManager &get_instance()
    {
        static MapManager instance;
        return instance;
    }
    // 将图片素材转换成元素
    void convert_map_to_cell(const std::string &filePath);
    // 根据像素转换成对应的Cell
    Cell createCellByPixel(sf::Color &pixel);

    // 获取元素的绘制区域
    std::pair<short, short> get_cell_rect(const unsigned int posX, const unsigned int posY);
    // 获取背景的绘制区域
    std::pair<short, short> get_bg_rect(const unsigned int posX, const unsigned int posY, const unsigned int map_height);

    /// @brief 初始化敌人和马里奥
    /// @return 返回马里奥的位置
    std::pair<short, short> init_mario_and_enemy_pos() const;

    // 获取整个地图元素个数
    int get_map_size();

    // 绘制地图
    void draw_map(sf::RenderWindow &i_window, const bool draw_bg, const unsigned int i_view_x);

    /// @brief 碰撞检测
    /// @param i_check_cells  需要检测的类型
    /// @param i_hitbox  检测范围
    /// @return  true 有交集，false没有交集
    bool map_collision(const std::vector<Cell> &i_check_cells, const sf::FloatRect &i_hitbox) const;
    // 收集碰撞的元素到i_collision_cells
    void map_collision(const std::vector<Cell> &i_check_cells, std::vector<sf::Vector2i> &i_collision_cells, const sf::FloatRect &i_hitbox) const;

    void set_map_cell(const unsigned short i_x, const unsigned short i_y, const Cell &i_cell);

    sf::Color get_map_sketch_pixel(const unsigned short i_x, const unsigned short i_y) const;
};
