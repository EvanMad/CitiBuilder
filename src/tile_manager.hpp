#pragma once

#include <map>
#include "tile.hpp"

class TileManager
{
private:
	const static int tileSize = 8;
	void loadTiles();
	void loadTextures();
	std::map<std::string, Tile> tileAtlas;
	TextureManager texmgr;
};