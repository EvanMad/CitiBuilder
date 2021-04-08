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
	int iconSize;
	std::vector<unsigned char> icon;
	int animCount;
	TileFile();
};

class TileManager
{
private:
	const static int tileSize = 8;
public:
	std::vector<Tile> loadTiles(TextureManager& texmgr, std::vector<sf::Texture>& icons);
	void loadTextures();
	std::vector<Tile> tileAtlas;
	TextureManager texmgr;
};