#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
private:
	std::map<std::string, sf::Texture> textures;

public:
	void loadTexture(const std::string& name, const std::string& filename);
	void LoadTextureFromMemory(void* input, int size, std::string& name);
	sf::Texture& getRef(const std::string& texture);

	TextureManager()
	{

	}
};

#endif // TEXTURE_MANAGER_HPP