#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "map.hpp"
#include "tile.hpp"

void Map::load(const std::string& filename, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas)
{
	std::fstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);
	this->width = width;
	this->height = height;
	for (int pos = 0; pos < this->width * this->height; ++pos)
	{
		this->resources.push_back(255);
		this->selected.push_back(0);
		TileType tileType;
		inputFile.read((char*)&tileType, sizeof(int));
		switch(tileType)
		{
		default:
		case TileType::VOID:
		case TileType::GRASS:
			this->tiles.push_back(tileAtlas.at("grass"));
			break;
		case TileType::FOREST:
			this->tiles.push_back(tileAtlas.at("forest"));
			break;
		case TileType::WATER:
			this->tiles.push_back(tileAtlas.at("water"));
			break;
			/*
		case TileType::RESIDENTIAL:
			this->tiles.push_back(tileAtlas.at("residential"));
			break;
		case TileType::COMMERCIAL:
			this->tiles.push_back(tileAtlas.at("commerical"));
			break;
		case TileType::INDUSTRIAL:
			this->tiles.push_back(tileAtlas.at("industrial"));
			break;
			*/
		case TileType::ROAD:
			this->tiles.push_back(tileAtlas.at("road"));
			break;

		case TileType::RESIDENTIAL_HIGH_DENSITY: this->tiles.push_back(tileAtlas.at("residential_high_density")); break;
		case TileType::RESIDENTIAL_MEDIUM_DENSITY: this->tiles.push_back(tileAtlas.at("residential_medium_density")); break;
		case TileType::RESIDENTIAL_LOW_DENSITY: this->tiles.push_back(tileAtlas.at("residential_low_density")); break;
		case TileType::RESIDENTIAL_TOWERS: this->tiles.push_back(tileAtlas.at("residential_low_density")); break;
		case TileType::RESIDENTIAL_SKYSCRAPERS: this->tiles.push_back(tileAtlas.at("residential_low_density")); break;
		case TileType::RESIDENTIAL_SKYSCRAPERS_HUGE: this->tiles.push_back(tileAtlas.at("residential_low_density")); break;

		}
		Tile& tile = this->tiles.back();
		inputFile.read((char*)&tile.tileVariant, sizeof(int));
		inputFile.read((char*)&tile.regions, sizeof(int));
		inputFile.read((char*)&tile.population, sizeof(int));
		inputFile.read((char*)&tile.storedGoods, sizeof(int));
	}
	inputFile.close();
	return;
}

void Map::LoadFromCitiFile(std::vector<TileSaveStruct> file, unsigned int width, unsigned int height, std::map<std::string, Tile>& tileAtlas){
	this->width = width;
	this->height = height;

	std::map<TileType, std::string> tiletest;
	tiletest.emplace(TileType::GRASS, "grass");
	tiletest.emplace(TileType::FOREST, "forest");
	tiletest.emplace(TileType::WATER, "water");
	tiletest.emplace(TileType::RESIDENTIAL_HIGH_DENSITY, "residential_high_density");
	tiletest.emplace(TileType::RESIDENTIAL_MEDIUM_DENSITY, "residential_medium_density");
	tiletest.emplace(TileType::RESIDENTIAL_LOW_DENSITY, "residential_low_density");
	tiletest.emplace(TileType::RESIDENTIAL_TOWERS, "residential_towers");
	tiletest.emplace(TileType::RESIDENTIAL_SKYSCRAPERS, "residential_skyscrapers");
	tiletest.emplace(TileType::RESIDENTIAL_SKYSCRAPERS_HUGE, "residential_skyscrapers_huge");

	tiletest.emplace(TileType::COMMERCIAL_HIGH_DENSITY, "commercial_high_density");
	tiletest.emplace(TileType::COMMERCIAL_MEDIUM_DENSITY, "commercial_medium_density");
	tiletest.emplace(TileType::COMMERCIAL_LOW_DENSITY, "commercial_low_density");

	tiletest.emplace(TileType::INDUSTRIAL_HIGH_DENSITY, "industrial_high_density");
	tiletest.emplace(TileType::INDUSTRIAL_MEDIUM_DENSITY, "industrial_medium_density");
	tiletest.emplace(TileType::INDUSTRIAL_LOW_DENSITY, "industrial_low_density");

	tiletest.emplace(TileType::SAND, "sand");
	tiletest.emplace(TileType::DIRT, "dirt");
	tiletest.emplace(TileType::WHEAT, "wheat");
	tiletest.emplace(TileType::ROAD, "road");
	tiletest.emplace(TileType::TREE, "tree");

	for (int pos = 0; pos < this->width * this->height; ++pos)
	{
		this->resources.push_back(255);
		this->selected.push_back(0);
		TileType tileType;
		tileType = (TileType)file[pos].tiletype;

		this->tiles.push_back(tileAtlas.at(tiletest.at(tileType)));

		Tile& tile = this->tiles.back();
		tile.tileVariant = file[pos].tileVariant;
		tile.tileVariant = file[pos].regions;
		tile.tileVariant = file[pos].population;
		tile.tileVariant = file[pos].storedGoods;
	}
	return;
}

std::vector<int> Map::save(const std::string& filename)
{
	//std::ofstream outputFile;
	//outputFile.open(filename, std::ios::out | std::ios::binary);

	MapSaveStruct mapsave;
	std::vector<int> test;
	for (auto tile : this->tiles)
	{
		test.push_back((int)tile.tileType);
		test.push_back((int)tile.tileVariant);
		test.push_back((int)tile.regions);
		test.push_back((int)tile.population);
		test.push_back((int)tile.storedGoods);
	}

	return test;

	//outputFile.close();
}

void Map::draw(sf::RenderWindow& window, float dt)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			sf::Vector2i pos;
			pos.x = (x - y) * this->tileSize + this->width * this->tileSize;
			pos.y = (x + y) * this->tileSize * 0.5;
			this->tiles[y * this->width+x].sprite.setPosition(sf::Vector2f(int(pos.x), int(pos.y)));

			if (this->selected[y * this->width + x])
				this->tiles[y * this->width + x].sprite.setColor(sf::Color(0x7d, 0x7d, 0x7d));
			else
				this->tiles[y * this->width + x].sprite.setColor(sf::Color(0xff, 0xff, 0xff));

			this->tiles[y * this->width+x].draw(window, dt);
		}
	}
	return;
}

void Map::updateDirection(TileType tileType)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			int pos = y * this->width + x;

			if (this->tiles[pos].tileType != tileType) continue;

			bool adjacentTiles[3][3] = { {0,0,0},{0,0,0},{0,0,0} };

			/* Check for adjacent tiles of the same type */
			if (x > 0 && y > 0)
				adjacentTiles[0][0] = (this->tiles[(y - 1) * this->width + (x - 1)].tileType == tileType);
			if (y > 0)
				adjacentTiles[0][1] = (this->tiles[(y - 1) * this->width + (x)].tileType == tileType);
			if (x < this->width - 1 && y > 0)
				adjacentTiles[0][2] = (this->tiles[(y - 1) * this->width + (x + 1)].tileType == tileType);
			if (x > 0)
				adjacentTiles[1][0] = (this->tiles[(y)*this->width + (x - 1)].tileType == tileType);
			if (x < width - 1)
				adjacentTiles[1][2] = (this->tiles[(y)*this->width + (x + 1)].tileType == tileType);
			if (x > 0 && y < this->height - 1)
				adjacentTiles[2][0] = (this->tiles[(y + 1) * this->width + (x - 1)].tileType == tileType);
			if (y < this->height - 1)
				adjacentTiles[2][1] = (this->tiles[(y + 1) * this->width + (x)].tileType == tileType);
			if (x < this->width - 1 && y < this->height - 1)
				adjacentTiles[2][2] = (this->tiles[(y + 1) * this->width + (x + 1)].tileType == tileType);

			/* Change the tile variant depending on the tile position */
			if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 2;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 7;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 8;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 9;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 10;
			else if (adjacentTiles[1][0] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[0][1] && adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 1;
			else if (adjacentTiles[2][1] && adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 3;
			else if (adjacentTiles[0][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 4;
			else if (adjacentTiles[1][0] && adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 5;
			else if (adjacentTiles[2][1] && adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 6;
			else if (adjacentTiles[1][0])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[1][2])
				this->tiles[pos].tileVariant = 0;
			else if (adjacentTiles[0][1])
				this->tiles[pos].tileVariant = 1;
			else if (adjacentTiles[2][1])
				this->tiles[pos].tileVariant = 1;
		}
	}

	return;
}

void Map::depthfirstsearch(std::vector<TileType>& whitelist, sf::Vector2i pos, int label, int regionType = 0)
{
	if (pos.x < 0 || pos.x >= this->width) return;
	if (pos.y < 0 || pos.y >= this->height) return;
	if (this->tiles[pos.y * this->width+pos.x].regions[regionType] != 0) return;
	bool found = false;
	for (auto type : whitelist)
	{
		if (type == this->tiles[pos.y * this->width + pos.x].tileType)
		{
			found = true;
			break;
		}
	}
	if (!found) return;

	this->tiles[pos.y * this->width + pos.x].regions[regionType] = label;

	depthfirstsearch(whitelist, pos + sf::Vector2i(-1, 0), label, regionType);
	depthfirstsearch(whitelist, pos + sf::Vector2i(0, 1), label, regionType);
	depthfirstsearch(whitelist, pos + sf::Vector2i(1, 0), label, regionType);
	depthfirstsearch(whitelist, pos + sf::Vector2i(0, -1), label, regionType);
	return;
}

void Map::findConnectedRegions(std::vector<TileType> whitelist, int regionType = 0)
{
	int regions = 1;
	for (auto& tile : this->tiles) tile.regions[regionType] = 0;
	for (int y = 0; y < this->height; ++y)
	{
		int regions = 1;
		for (auto& tile : this->tiles) tile.regions[regionType] = 0;
		for (int y = 0; y < this->height; ++y)
		{
			for (int x = 0; x < this->height; ++x)
			{
				bool found = false;
				for (auto type : whitelist)
				{
					if (type == this->tiles[y * this->width + x].tileType)
					{
						found = true;
						break;
					}
				}
				if (this->tiles[y * this->width + x].regions[regionType] == 0 && found)
				{
					depthfirstsearch(whitelist, sf::Vector2i(x, y), regions++, regionType);
				}
			}
		}
	}
	this->numRegions[regionType] = regions;
}

void Map::clearSelected()
{
	for (auto& tile : this->selected) tile = 0;
	this->numSelected = 0;
	return;
}

void Map::select(sf::Vector2i start, sf::Vector2i end, std::vector<TileType> blacklist)
{
	/* Swap coordinates if necessary */
	if (end.y < start.y) std::swap(start.y, end.y);
	if (end.x < start.x) std::swap(start.x, end.x);

	/* Clamp in range */
	if (end.x >= this->width)        end.x = this->width - 1;
	else if (end.x < 0)                 end.x = 0;
	if (end.y >= this->height)         end.y = this->height - 1;
	else if (end.y < 0)                 end.y = 0;
	if (start.x >= this->width)         start.x = this->width - 1;
	else if (start.x < 0)             start.x = 0;
	if (start.y >= this->height)     start.y = this->height - 1;
	else if (start.y < 0)             start.y = 0;

	for (int y = start.y; y <= end.y; ++y)
	{
		for (int x = start.x; x <= end.x; ++x)
		{
			/* Check if the tile type is in the blacklist. If it is, mark it as
			 * invalid, otherwise select it */
			this->selected[y * this->width + x] = 1;
			++this->numSelected;
			for (auto type : blacklist)
			{
				if (this->tiles[y * this->width + x].tileType == type)
				{
					this->selected[y * this->width + x] = 2;
					--this->numSelected;
					break;
				}
			}
		}
	}

	return;
}

void Map::SelectTile(sf::Vector2i pos)
{
	this->clearSelected();
	if ((pos.x > this->width - 1) || (pos.x < 0) || (pos.y < 0) || (pos.y > this->height - 1)) { return; }
	this->selected[pos.y * this->width + pos.x] = 2;
}

void Map::PlaceSingleTile(sf::Vector2i pos, Tile tile)
{
	this->clearSelected();
	if ((pos.x > this->width-1) || (pos.x < 0) || (pos.y < 0) || (pos.y > this->height-1)) { return; }
	//this->selected[pos.y * this->width + pos.x] = 2;
	this->tiles[pos.y * this->width + pos.x] = tile;
	//this->selected[pos.y * this->width + pos.x] = 1;
}