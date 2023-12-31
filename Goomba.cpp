
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Headers/GlobalConfig.hpp"
#include "Headers/Animation.hpp"
#include "Headers/MapManager.hpp"

#include "Headers/MarioStateManager.hpp"
#include "Headers/Mario.hpp"
#include "Headers/Enemy.hpp"
#include "Headers/HitBoxUtils.hpp"
#include "Headers/Goomba.hpp"

Goomba::Goomba(const float i_x, const float i_y) : Enemy(i_x, i_y),
												   death_timer(GOOMBA_DEATH_DURATION),
												   walk_animation(CELL_SIZE, "Resources/Images/GoombaWalk.png", GOOMBA_WALK_ANIMATION_SPEED)
{
	horizontal_speed = -GOOMBA_SPEED;

	texture.loadFromFile("Resources/Images/GoombaDeath0.png");
}

bool Goomba::get_dead(const bool i_deletion) const
{
	if (1 == i_deletion)
	{
		return dead;
	}
	else
	{
		return dead || GOOMBA_DEATH_DURATION > death_timer;
	}
}

void Goomba::die(const unsigned char i_death_type)
{
	switch (i_death_type)
	{
	case 0:
	{

		dead = 1;

		break;
	}
	case 1:
	{

		death_timer--;

		break;
	}
	case 2:
	{
		if (GOOMBA_DEATH_DURATION == death_timer)
		{

			vertical_speed = 0.5f * MARIO_JUMP_SPEED;
			texture.loadFromFile("Resources/Images/UndergroundGoombaDeath1.png");
		}
	}
	}
}

void Goomba::draw(const unsigned i_view_x, sf::RenderWindow &i_window)
{

	if (-CELL_SIZE < round(y) && round(x) > static_cast<int>(i_view_x) - CELL_SIZE && round(x) < SCREEN_WIDTH + i_view_x && round(y) < SCREEN_HEIGHT)
	{
		if (GOOMBA_DEATH_DURATION > death_timer)
		{
			sprite.setPosition(round(x), round(y));
			sprite.setTexture(texture);

			i_window.draw(sprite);
		}
		else
		{
			walk_animation.set_position(round(x), round(y));
			walk_animation.draw(i_window);
		}
	}
}

void Goomba::update(const unsigned i_view_x, const std::vector<std::shared_ptr<Enemy>> &i_enemies)
{

	bool is_can_update = -CELL_SIZE < y && x >= static_cast<int>(i_view_x) - CELL_SIZE - ENTITY_UPDATE_AREA && x < ENTITY_UPDATE_AREA + SCREEN_WIDTH + i_view_x && y < SCREEN_HEIGHT;

	if (!is_can_update)
	{
		return;
	}

	sf::FloatRect hit_box = get_hit_box();

	vertical_speed = std::min(GRAVITY + vertical_speed, MAX_VERTICAL_SPEED);

	hit_box.top += vertical_speed;

	if (HitBoxUtils::check_hit_box(hit_box))
	{
		if (0 > vertical_speed)
		{
			y = CELL_SIZE * (1 + floor((vertical_speed + y) / CELL_SIZE));
		}
		else
		{
			y = CELL_SIZE * (ceil((vertical_speed + y) / CELL_SIZE) - 1);
		}

		vertical_speed = 0;
	}
	else
	{
		bool changed = 0;

		if (0 == get_dead(0))
		{
			for (unsigned short a = 0; a < i_enemies.size(); a++)
			{
				if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
				{
					changed = 1;

					if (0 > vertical_speed)
					{
						y = i_enemies[a]->get_hit_box().height + i_enemies[a]->get_hit_box().top;
					}
					else
					{
						y = i_enemies[a]->get_hit_box().top - CELL_SIZE;
					}

					vertical_speed = 0;

					break;
				}
			}
		}

		if (0 == changed)
		{
			y += vertical_speed;
		}
	}

	if (0 == get_dead(0))
	{
		hit_box = get_hit_box();
		hit_box.left += horizontal_speed;

		if (HitBoxUtils::check_hit_box(hit_box))
		{
			if (0 < horizontal_speed)
			{
				x = CELL_SIZE * (ceil((horizontal_speed + x) / CELL_SIZE) - 1);
			}
			else
			{
				x = CELL_SIZE * (1 + floor((horizontal_speed + x) / CELL_SIZE));
			}

			horizontal_speed *= -1;
		}
		else
		{
			bool changed = 0;

			for (unsigned short a = 0; a < i_enemies.size(); a++)
			{
				if (shared_from_this() != i_enemies[a] && 0 == i_enemies[a]->get_dead(0) && 1 == hit_box.intersects(i_enemies[a]->get_hit_box()))
				{
					changed = 1;

					horizontal_speed *= -1;

					break;
				}
			}

			if (0 == changed)
			{
				x += horizontal_speed;
			}
		}

		walk_animation.update();
	}
	else if (GOOMBA_DEATH_DURATION > death_timer)
	{
		if (0 < death_timer)
		{
			death_timer--;
		}
		else
		{
			die(0);
		}
	}

	if (SCREEN_HEIGHT <= y)
	{
		die(0);
	}
}
