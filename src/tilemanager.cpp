#pragma once

#include "texture_manager.hpp"
#include "tile_manager.hpp"
#include "animation_handler.hpp"
#include <filesystem>
#include <string>
#include <TGUI/TGUI.hpp>
#include <fstream>
#include "game.hpp"

std::string base_name(std::string const& path)
{
	return path.substr(path.find_last_of("/\\") + 1);
}

std::string remove_extension(const std::string& filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) return filename;
	return filename.substr(0, lastdot);
}

inline bool ends_with(std::string const& value, std::string const& ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}


void TileManager::loadTextures()
{
}

std::map<std::string, Tile> TileManager::loadTiles(TextureManager& texmgr)
{
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
			tile;

			//texmgr = TextureManager();
			sf::Texture tex;
			sf::Image img;
			void* data = (tile.tex.data());
			auto data2 = (&tile.tex);
			std::string baseName = remove_extension(base_name(std::string(fileName)));
			//tex.loadFromMemory(data, tile.texSize);
			//bool check = tex.loadFromMemory(data, size_t(tile.texSize));
			//check;
			texmgr.LoadTextureFromMemory(data, tile.tex.size(), baseName);
			this->tileAtlas[baseName] = Tile(this->tileSize, tile.height, texmgr.getRef(baseName), anims, static_cast<TileType>(tile.tiletype), 50, 0, 1);
			inputFile.close();
		}
	}
	return this->tileAtlas;
}

TileFile::TileFile()
{

}