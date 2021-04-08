#pragma once
#ifndef  GAME_HPP
#define GAME_HPP

#include <stack>
#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include <map>
#include <string>
#include "tile.hpp"
#include "gui.hpp"
#include <TGUI/TGUI.hpp>

class GameState;

class Game
{
private:
	void loadTextures();
	void loadTiles();
	void loadStyleSheets();
	void loadFonts();
public:
	tgui::GuiSFML gui;
	const static int tileSize = 8;
	std::stack<GameState*> states;
	sf::RenderWindow window;

	TextureManager texmgr;
	std::vector<sf::Texture> icons;
	sf::Sprite background;

	std::vector<Tile> tileAtlas;
	//std::map<std::string, Tile> tileAtlas;
	std::map<std::string, GuiStyle> stylesheets;
	std::map<std::string, sf::Font> fonts;

	void pushState(GameState* state);
	void popState();
	void ChangeState(GameState* state);
	GameState* peekState();


	void gameLoop();

	Game();
	~Game();
};

#endif // ! GAME_HPP
