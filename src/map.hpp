#ifndef MAP_HPP
#define MAP_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <map>
#include <vector>

#include "tile.hpp"

struct TileSaveStruct
{
	int tiletype;
	int tileVariant;
	int regions;
	int population;
	int storedGoods;
};

struct MapSaveStruct
{
	std::vector<TileSaveStruct> tiles;
};

class Map
{
private:
	void depthfirstsearch(std::vector<int>& whitelist, sf::Vector2i pos, int label, int type);
	
public:
	unsigned int width;
	unsigned int height;

	std::vector<Tile> tiles;
	std::vector<int> resources;

	unsigned int tileSize;
	unsigned int numSelected;
	unsigned int numRegions[1];

	void load(const std::string & filename, unsigned int width, unsigned int height, std::vector<Tile>& tileAtlas);
	void LoadFromCitiFile(std::vector<TileSaveStruct> file, unsigned int width, unsigned int height, std::vector<Tile>& tileAtlas);
	void LoadFromMemory(std::vector<unsigned char> data, unsigned int width, unsigned int height);

	std::vector<int> save();
	void draw(sf::RenderWindow& window, float dt);
	void findConnectedRegions(std::vector<int> whitelist, int type);
	void updateDirection(int tileType);

	std::vector<char> selected;
	void select(sf::Vector2i start, sf::Vector2i end, std::vector<int> blacklist);
	void SelectTile(sf::Vector2i pos);
	void PlaceSingleTile(sf::Vector2i pos, Tile tile);
	void clearSelected();

	Map()
	{
		this->numSelected = 0;
		this->tileSize = 8;
		this->width = 0;
		this->height = 0;
		this->numRegions[0] = 1;
	}

	Map(const std::string& filename, unsigned int width, unsigned int height, std::vector<Tile>& tileAtlas)
	{
		this->numSelected = 0;
		this->tileSize = 8;
		load(filename, width, height, tileAtlas);
	}
};


#endif //MAP_HPP