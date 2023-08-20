#include <chrono>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Headers/GlobalConfig.hpp"

#include "Headers/Animation.hpp"
#include "Headers/MapManager.hpp"
#include "Headers/MarioState.hpp"
#include "Headers/Mario.hpp"

#include "Headers/MarioState.hpp"

using namespace std;
using namespace std::chrono;

int main()
{

    MapManager::get_instance().convert_map_to_cell("Resources/Images/LevelSketch0.png");

    Mario mario;
    auto mario_pos = MapManager::get_instance().get_mario_birth_pos();
    mario.set_position(mario_pos.first, mario_pos.second);

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE), "Super Mario", sf::Style::Close);
    sf::Color background_color = sf::Color(0, 219, 255);

    sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    sf::Event event;

    unsigned int view_x = 0;

    steady_clock::time_point current_time = steady_clock::now();

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                window.close();
            }
        }

        // 设置帧率
        steady_clock::time_point time_now = steady_clock::now();
        std::chrono::microseconds time_diff = std::chrono::duration_cast<std::chrono::microseconds>(time_now - current_time);
        if (time_diff < FRAME_DURATION)
        {
            continue;
        }
        current_time = time_now;

        // 设置屏幕中心位置
        view_x = std::clamp<int>(round(mario.posX) - 0.5f * (SCREEN_WIDTH - CELL_SIZE), 0, CELL_SIZE * MapManager::get_instance().get_map_size() - SCREEN_WIDTH);
        view.reset(sf::FloatRect(view_x, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        window.setView(view);

        // 绘制内容
        window.clear(background_color);
        MapManager::get_instance().draw_map(window, true, view_x);
        MapManager::get_instance().draw_map(window, false, view_x);
        mario.draw_mario(window);

        window.display();
    }

    return 0;
}