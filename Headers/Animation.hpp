#pragma once

class Animation
{

    unsigned short animation_speed;
    
    bool flipped;
    unsigned short frame_width;
    unsigned short total_frames;
    unsigned short current_frame;

    unsigned short animation_iterator;
    sf::Sprite sprite;
    sf::Texture texture;

public:
    Animation(const unsigned short i_frame_width, const std::string &i_texture_location, const unsigned short i_animation_speed = 1);

    void set_position(const short i_x, const short i_y);
    void update();
    void draw(sf::RenderWindow &i_window);
    void set_animation_speed(const unsigned short i_animation_speed);
    void set_flipped(const bool i_value);
    void set_texture_location(const std::string &i_texture_location);
};
