#pragma once

#include <map>
#include "tile.hpp"

struct TileFile
{
public:
	int size;
	int height;
	int cost;
	int tiletype;
	int texSize;
	std::vector<unsigned char> tex;
	int animCount;
	TileFile();
};

class TileManager
{
private:
	const static int tileSize = 8;
public:
	std::map<std::string, Tile> loadTiles(TextureManager& texmgr);
	void loadTextures();
	std::map<std::string, Tile> tileAtlas;
	TextureManager texmgr;
};