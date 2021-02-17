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
	void depthfirstsearch(std::vector<TileType>& whitelist, sf::Vector2i pos, int label, int type);
	
public:
	unsigned int width;
	unsigned int height;

	std::vector<Tile> tiles;
	std::vector<int> resources;

	unsigned int tileSize;
	unsigned int numSelected;
	unsigned int numRegions[1];

	void load(const std::string & filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas);
	void LoadFromCitiFile(std::vector<TileSaveStruct> file, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas);

	std::vector<int> save(const std::string& filename);
	void draw(sf::RenderWindow& window, float dt);
	void findConnectedRegions(std::vector<TileType> whitelist, int type);
	void updateDirection(TileType tileType);

	std::vector<char> selected;
	void select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist);
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

	Map(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
	{
		this->numSelected = 0;
		this->tileSize = 8;
		load(filename, width, height, tileAtlas);
	}
};


#endif //MAP_HPP