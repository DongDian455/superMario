#include <SFML/Graphics.hpp>
#include "Headers/GlobalConfig.hpp"
#include "Headers/Enemy.hpp"

Enemy::Enemy(const float i_x, const float i_y) : dead(0),
												 horizontal_speed(0),
												 vertical_speed(0),
												 x(i_x),
												 y(i_y)
{
}

bool Enemy::get_dead(const bool i_deletion) const
{
	return dead;
}

void Enemy::die(const unsigned char i_death_type)
{
	dead = 1;
}

sf::FloatRect Enemy::get_hit_box() const
{
	return sf::FloatRect(x, y, CELL_SIZE, CELL_SIZE);
}