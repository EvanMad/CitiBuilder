#include <SFML/Graphics.hpp>

#include "tile.hpp"
#include "animation_handler.hpp"

void Tile::draw(sf::RenderWindow& window, float dt)
{
	this->animHandler.changeAnim(this->tileVariant);
	this->animHandler.update(dt);
	this->sprite.setTextureRect(this->animHandler.bounds);
	window.draw(this->sprite);
	return;
}

int random(int min, int max) //range : [min, max]
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void Tile::update()
{
	return;
}