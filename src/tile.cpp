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
	/* If the population is at the maximum value for the tile,
	 * there is a small chance that the tile will increase its
	 * building stage */
	if ((this->tileType == TileType::RESIDENTIAL ||
		this->tileType == TileType::COMMERCIAL ||
		this->tileType == TileType::INDUSTRIAL) &&
		this->population == this->maxPopPerLevel * (double(this->tileVariant) + 1) &&
		this->tileVariant < this->maxLevels)
	{
		++this->tileVariant;
		//if (rand() % int(1e4) < 1e2 / (double(this->tileVariant) + 1)) ++this->tileVariant;
		std::cout << (random(0, 10) > 5) << std::endl;
		if (random(0, 10) > 5) ++this->tileVariant;
	}

	if ((this->tileType == TileType::RESIDENTIAL ||
		this->tileType == TileType::COMMERCIAL ||
		this->tileType == TileType::INDUSTRIAL))
	{
		++this->tileVariant;
	}

	return;
}

std::string tileTypeToStr(TileType type)
{
	switch (type)
	{
	default:
	case TileType::VOID:             return "Void";
	case TileType::GRASS:            return "Flatten";
	case TileType::FOREST:		     return "Forest";
	case TileType::WATER:            return "Water";
	case TileType::RESIDENTIAL:      return "Redsidential Zone";
	case TileType::COMMERCIAL:       return "Commercial Zome";
	case TileType::INDUSTRIAL:       return "Industrial Zome";
	}
}
