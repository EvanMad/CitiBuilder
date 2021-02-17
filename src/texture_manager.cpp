#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "texture_manager.hpp"
#include <iostream>
void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
	sf::Texture tex;
	
	tex.loadFromFile(filename);
	tex.setSmooth(false);
	//std::cout << dir << std::endl;

	this->textures[name] = tex;

	return;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
	return this->textures.at(texture);
}