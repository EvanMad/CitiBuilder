#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "map.hpp"
#include "tile.hpp"
#include "game.hpp"

void Map::load(const std::string& filename, unsigned int width, unsigned int height, std::vector<Tile>& tileAtlas)
{
	std::fstream inputFile;
	inputFile.open(filename, std::ios::in | std::ios::binary);
	this->width = width;
	this->height = height;
	int dump;
	inputFile.read((char*)&dump, sizeof(int));
	for (int pos = 0; pos < this->width * this->height; ++pos)
	{
		this->resources.push_back(255);
		this->selected.push_back(0);
		int tileType;
		inputFile.read((char*)&tileType, sizeof(int));

		this->tiles.push_back(tileAtlas[tileType]);

		Tile& tile = this->tiles.back();
		inputFile.read((char*)&tile.tileVariant, sizeof(int));
		inputFile.read((char*)&tile.regions, sizeof(int));
		inputFile.read((char*)&tile.population, sizeof(int));
		inputFile.read((char*)&tile.storedGoods, sizeof(int));
	}
	inputFile.close();
	return;
}

void Map::LoadFromMemory(std::vector<unsigned char> data, unsigned int width, unsigned int height)
{

}

void Map::LoadFromCitiFile(std::vector<TileSaveStruct> file, unsigned int width, unsigned int height, std::vector<Tile>& tileAtlas){
	this->width = width;
	this->height = height;

	for (int pos = 0; pos < this->width * this->height; ++pos)
	{
		this->resources.push_back(255);
		this->selected.push_back(0);
		int tileType;
		//tileType = (TileType)file[pos].tiletype;
		tileType = file[pos].tiletype;

		this->tiles.push_back(tileAtlas[tileType]);

		Tile& tile = this->tiles.back();
		tile.tileVariant = file[pos].tileVariant;
		tile.tileVariant = file[pos].regions;
		tile.tileVariant = file[pos].population;
		tile.tileVariant = file[pos].storedGoods;
	}
	return;
}

std::vector<int> Map::save()
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

void Map::updateDirection(int tileType)
{
	for (int y = 0; y < this->height; ++y)
	{
		for (int x = 0; x < this->width; ++x)
		{
			int pos = y * this->width + x;

			if (this->tiles[pos].tileType != tileType) continue;

			bool adjacentTiles[3][3] = { {0,0,0},{0,0,0},{0,0,0} };
			std::vector<Tile> adjacentTilesArray;

			/* Check for adjacent tiles of the same type */
			// I removed some code here that adds to this vector, basically I just was playing around with the tile borders idea (like beaches connecting to water) but never put too much thought to it
			if (x > 0 && y > 0)
				//adjacentTilesArray.push_back(this->tiles[(y - 1) * this->width + (x - 1)]);
				adjacentTiles[0][0] = (this->tiles[(y - 1) * this->width + (x - 1)].tileType == tileType);
			if (y > 0)
				//adjacentTilesArray.push_back(this->tiles[(y - 1) * this->width + (x - 1)]);
				adjacentTiles[0][1] = (this->tiles[(y - 1) * this->width + (x)].tileType == tileType);
			if (x < this->width - 1 && y > 0)
				//adjacentTilesArray.push_back(this->tiles[(y - 1) * this->width + (x + 1)]);
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

			for (auto tile : adjacentTilesArray)
			{
				std::cout << tile.tileType << std::endl;
			}

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

void Map::depthfirstsearch(std::vector<int>& whitelist, sf::Vector2i pos, int label, int regionType = 0)
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

void Map::findConnectedRegions(std::vector<int> whitelist, int regionType = 0)
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

void Map::select(sf::Vector2i start, sf::Vector2i end, std::vector<int> blacklist)
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