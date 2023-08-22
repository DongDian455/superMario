#pragma once

#include "Animation.hpp"
#include "Enemy.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

constexpr unsigned short GOOMBA_DEATH_DURATION = 32;
// Why do I have so many animation speed constants? I use them only once.
constexpr unsigned short GOOMBA_WALK_ANIMATION_SPEED = 32;
constexpr float GOOMBA_SPEED = 1;

class Goomba : public Enemy
{

	// The underground Goomba looks different than the regular Goomba.
	bool underground;

	unsigned short death_timer;

	sf::Sprite sprite;

	sf::Texture texture;

	Animation walk_animation;

public:
	Goomba(const bool i_underground, const float i_x, const float i_y);

	bool get_dead(const bool i_deletion) const;

	void die(const unsigned char i_death_type);
	void draw(const unsigned i_view_x, sf::RenderWindow &i_window);
	void update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>> &i_enemies, Mario &i_mario);
};
