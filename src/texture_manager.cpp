#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "texture_manager.hpp"
#include <iostream>

class FileStream : public sf::InputStream
{
public:
	bool open(std::string filename);
};

void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
	sf::Texture tex;
	
	tex.loadFromFile(filename);
	tex.setSmooth(false);
	//std::cout << dir << std::endl;

	this->textures[name] = tex;

	return;
}

void TextureManager::LoadTextureFromMemory(void* input, int size,std::string& name)
{
	sf::Texture tex;
	tex.loadFromMemory(input, size_t(size));
	this->textures[name] = tex;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
	return this->textures.at(texture);
}