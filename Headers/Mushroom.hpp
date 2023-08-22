#pragma once

class Mushroom
{
	bool dead;

	char horizontal_direction;

	float vertical_speed;
	float x;
	float y;

	unsigned short start_y;

	sf::Sprite sprite;

	sf::Texture texture;

public:
	Mushroom(const float i_x, const float i_y);

	bool get_dead() const;

	void draw(const unsigned i_view_x, sf::RenderWindow &i_window);
	void set_dead(const bool i_value);
	void update(const unsigned i_view_x);

	sf::FloatRect get_hit_box() const;
};