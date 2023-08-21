#pragma once

class HitBoxUtils
{
public:
    static bool check_hit_box(const sf::FloatRect &i_hitbox);
    static void check_mario_hit_coin(const sf::FloatRect &i_hitbox);
};
