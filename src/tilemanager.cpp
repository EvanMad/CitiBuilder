#pragma once

#include "texture_manager.hpp"
#include "tile_manager.hpp"
#include "animation_handler.hpp"
#include <filesystem>
#include <string>
#include <TGUI/TGUI.hpp>
#include <fstream>
#include "game.hpp"
#include "util.hpp"

void TileManager::loadTextures()
{
}

bool compareTile(const Tile* lhs, const Tile* rhs)
{
	return lhs->tileType < rhs->tileType;
}

std::vector<Tile> TileManager::loadTiles(TextureManager& texmgr, std::vector<sf::Texture>& icons)
{
	auto localIcons = &icons;
	tileAtlas.resize(256);
	icons.resize(256);
	Animation staticAnim(0, 0, 1.0f);
	for (const auto& entry : std::filesystem::directory_iterator("content/tiles"))
	{
		tgui::String fileName = tgui::String(entry.path());
		if (ends_with(std::string(fileName), ".tile"))
		{
			TileFile tile;
			std::fstream inputFile;
			inputFile.open(entry.path(), std::ios::in | std::ios::binary);
			//inputFile.read(reinterpret_cast<char*>(&tile), sizeof(TileFile));
			int dump;
			inputFile.read((char*)&tile.size, sizeof(int));
			inputFile.read((char*)&tile.height, sizeof(int));
			inputFile.read((char*)&tile.cost, sizeof(int));
			inputFile.read((char*)&tile.tiletype, sizeof(int));
			inputFile.read((char*)&tile.texSize, sizeof(int));
			inputFile.read((char*)&dump, sizeof(char));
			//std::vector<unsigned char> texture(tile.texSize);
			tile.tex.resize(tile.texSize);
			inputFile.read((char*)tile.tex.data(), tile.texSize-1);
			//inputFile.read((char*)&dump, sizeof(char));
			//inputFile.read((char*)&dump, sizeof(char));
			//inputFile.read((char*)&dump, sizeof(char));

			inputFile.read((char*)&tile.iconSize, sizeof(int));
			tile.icon.resize(tile.iconSize);
			inputFile.read((char*)&dump, sizeof(char));
			inputFile.read((char*)tile.icon.data(), tile.iconSize - 1);
			tile;

			//temp anim sutff
			std::vector<Animation> anims;
			//anims.resize(tile.animCount);
			auto pos = inputFile.tellg();
			inputFile.read((char*)&tile.animCount, sizeof(int));
			for (int i = 0; i < tile.animCount; i++)
			{
				float startFrame;
				float endFrame;
				float duration;
				inputFile.read((char*)&startFrame, sizeof(float));
				inputFile.read((char*)&endFrame, sizeof(float));
				inputFile.read((char*)&duration, sizeof(float));
				anims.push_back(Animation(startFrame, endFrame, duration));
			}

			//texmgr = TextureManager();
			sf::Texture tex;
			sf::Image img;
			void* texdata = (tile.tex.data());
			std::string baseName = remove_extension(base_name(std::string(fileName)));
			std::string iconName = baseName + "_icon";
			void* iconData = (tile.icon.data());
			texmgr.LoadTextureFromMemory(texdata, tile.tex.size(), baseName);
			texmgr.LoadTextureFromMemory(iconData, tile.icon.size(), iconName);
			//this->tileAtlas.push_back(Tile(this->tileSize, tile.height, texmgr.getRef(baseName), anims, (tile.tiletype), 50, 0, 1));
			this->tileAtlas[tile.tiletype] = Tile(this->tileSize, tile.height, texmgr.getRef(baseName), anims, (tile.tiletype), 50, 0, 1);
			icons[tile.tiletype] = texmgr.getRef(iconName);
			//this->tileAtlas.insert(Tile(this->tileSize, tile.height, texmgr.getRef(baseName), anims, (tile.tiletype), 50, 0, 1), 0);

			//auto itPos = this->tileAtlas.begin() + tile.tiletype;
			//auto place = this->tileAtlas.insert(itPos, Tile(this->tileSize, tile.height, texmgr.getRef(baseName), anims, (tile.tiletype), 50, 0, 1));

			inputFile.close();
		}
	}
	//std::sort(this->tileAtlas.begin(), this->tileAtlas.end(), [](const Tile& lhs, const Tile& rhs) {
		//return lhs.tileType < rhs.tileType;
		//});
	//std::sort(&tileAtlas.begin(), &tileAtlas.end(), compareTile);
	return this->tileAtlas;
}

TileFile::TileFile()
{

}