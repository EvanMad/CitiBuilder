#include <stack>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "game.hpp"
#include "game_state.hpp"
#include "texture_manager.hpp"
#include "animation_handler.hpp"
#include "tile.hpp"
#include <iostream>
#include <TGUI/TGUI.hpp>
#include <filesystem>
#include "util.hpp"

void Game::pushState(GameState* state)
{
	this->states.push(state);
	return;
}

void Game::popState()
{
	delete this->states.top();
	this->states.pop();
	return;
}

void Game::ChangeState(GameState* state)
{
	if (!this->states.empty())
		popState();
	pushState(state);
	return;
}

GameState* Game::peekState()
{
	if (this->states.empty()) return nullptr;
	return this->states.top();
}

void Game::gameLoop()
{
	sf::Clock clock;
	while (this->window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		if (peekState() == nullptr) continue;
		peekState()->handleInput();
		peekState()->update(dt);
		this->window.clear(sf::Color::Black);
		peekState()->draw(dt);
		this->window.display();
	}
}

Game::Game()
{
	this->loadTextures();
	this->loadTiles();

	this->window.create(sf::VideoMode(1920, 1080), "CitiBuiler V0.2 :)");
	//this->window.setMouseCursorVisible(false)

	tgui::GuiSFML gui{window};
	this->gui.setTarget(window);

	this->window.setFramerateLimit(60);

	this->background.setTexture(this->texmgr.getRef("background"));
	this->loadFonts();
	this->loadStyleSheets();
}

Game::~Game()
{
	while (!this->states.empty()) popState();
}

void Game::loadTextures()
{
	texmgr.loadTexture("background", "res/background2.png");

	/*
	texmgr.loadTexture("grass", "res/grass.png");
	texmgr.loadTexture("forest", "res/forest.png");
	texmgr.loadTexture("water", "res/water.png");
	texmgr.loadTexture("deepwater", "res/deepwater.png");
	texmgr.loadTexture("residential", "res/residential.png");
	texmgr.loadTexture("commercial", "res/commercial.png");
	texmgr.loadTexture("industrial", "res/industrial.png");
	texmgr.loadTexture("road", "res/road.png");
	texmgr.loadTexture("rail", "res/rail.png");
	texmgr.loadTexture("wheat", "res/wheat.png");
	texmgr.loadTexture("woodfence", "res/fence.png");
	texmgr.loadTexture("station", "res/station.png");

	texmgr.loadTexture("tree", "res/tree.png");
	texmgr.loadTexture("dirt", "res/dirt.png");
	texmgr.loadTexture("sand", "res/sand.png");

	//ResidentialSubTexs
	texmgr.loadTexture("residential_low_density", "res/residential/residential_low_density.png");
	texmgr.loadTexture("residential_medium_density", "res/residential/residential_medium_density.png");
	texmgr.loadTexture("residential_high_density", "res/residential/residential_high_density.png");
	texmgr.loadTexture("residential_towers", "res/residential/residential_towers.png");
	texmgr.loadTexture("residential_skyscrapers", "res/residential/residential_skyscrapers.png");
	texmgr.loadTexture("residential_skyscrapers_huge", "res/residential/residential_skyscrapers_huge.png");

	//IndustrialSubTexs
	texmgr.loadTexture("industrial_low_density", "res/industrial/industrial_low_density.png");
	texmgr.loadTexture("industrial_medium_density", "res/industrial/industrial_medium_density.png");
	texmgr.loadTexture("industrial_high_density", "res/industrial/industrial_high_density.png");
	
	//CommercialSubTexs
	texmgr.loadTexture("commercial_low_density", "res/commercial/commercial_low_density.png");
	texmgr.loadTexture("commercial_medium_density", "res/commercial/commercial_medium_density.png");
	texmgr.loadTexture("commercial_high_density", "res/commercial/commercial_high_density.png");

	texmgr.loadTexture("cursor", "res/cursor.png");
	*/
}

#include "tile_manager.hpp"
void Game::loadTiles()
{
	
	TileManager tiler;
	this->tileAtlas = tiler.loadTiles(texmgr, this->icons);
	Animation staticAnim(0, 0, 1.0f);

	/*
	std::cout << "test" << std::endl;
	Animation staticAnim(0, 0, 1.0f);
	this->tileAtlas["grass"] = Tile(this->tileSize, 1, texmgr.getRef("grass"), { staticAnim }, TileType::GRASS, 50, 0, 1);

	Tile grass = Tile(this->tileSize, 1, texmgr.getRef("grass"), { staticAnim }, TileType::GRASS, 50, 0, 1);
	this->tileAtlas["grass"] = grass;

	Tile forest = Tile(this->tileSize, 1, texmgr.getRef("forest"),
		{ staticAnim },
		TileType::FOREST, 100, 0, 1);
	this->tileAtlas["forest"] = forest;

	Tile water = Tile(this->tileSize, 1, texmgr.getRef("water"),
		{ Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f) },
		TileType::WATER, 0, 0, 1);
	this->tileAtlas["water"] = water;

	Tile deepwater = Tile(this->tileSize, 1, texmgr.getRef("deepwater"),
		{ Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f),
		Animation(0, 3, 0.5f) },
		TileType::DEEPWATER, 0, 0, 1);
	this->tileAtlas["deepwater"] = deepwater;

	Tile road =
		Tile(this->tileSize, 1, texmgr.getRef("road"),
			{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::ROAD, 100, 0, 1);
	this->tileAtlas["road"] = road;

	Tile rail =
		Tile(this->tileSize, 1, texmgr.getRef("rail"),
			{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::ROAD, 100, 0, 1);
	this->tileAtlas["rail"] = rail;

	Tile woodfence =
		Tile(this->tileSize, 1, texmgr.getRef("woodfence"),
			{ staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim, staticAnim,
			staticAnim, staticAnim },
			TileType::ROAD, 100, 0, 1);
	this->tileAtlas["woodfence"] = woodfence;

	Tile wheat = Tile(this->tileSize, 1, texmgr.getRef("wheat"), { staticAnim }, TileType::WHEAT, 50, 0, 1);
	this->tileAtlas["wheat"] = wheat;

	Tile station = Tile(this->tileSize, 2, texmgr.getRef("station"), { staticAnim }, TileType::WHEAT, 50, 0, 1);
	this->tileAtlas["station"] = station;

	this->tileAtlas["tree"] = Tile(this->tileSize, 2, texmgr.getRef("tree"), { staticAnim }, TileType::TREE, 50, 0, 1);
	this->tileAtlas["dirt"] = Tile(this->tileSize, 1, texmgr.getRef("dirt"), { staticAnim }, TileType::DIRT, 50, 0, 1);
	this->tileAtlas["sand"] = Tile(this->tileSize, 1, texmgr.getRef("sand"), { staticAnim }, TileType::SAND, 50, 0, 1);
	*/
	//Various Commercial Densities
	//this->tileAtlas["commercial_high_density"] =  Tile(this->tileSize, 2, texmgr.getRef("commercial_high_density"), { staticAnim }, TileType::COMMERCIAL_HIGH_DENSITY, 50, 0, 1);
	//this->tileAtlas["commercial_medium_density"] = Tile(this->tileSize, 2, texmgr.getRef("commercial_medium_density"), { staticAnim }, TileType::COMMERCIAL_MEDIUM_DENSITY, 50, 0, 1);
	//this->tileAtlas["commercial_low_density"] = Tile(this->tileSize, 2, texmgr.getRef("commercial_low_density"), { staticAnim }, TileType::COMMERCIAL_LOW_DENSITY, 50, 0, 1);

	//Various Industrial Densities
	//this->tileAtlas["industrial_high_density"] = Tile(this->tileSize, 2, texmgr.getRef("industrial_high_density"), { staticAnim }, TileType::INDUSTRIAL_HIGH_DENSITY, 50, 0, 1);
	//this->tileAtlas["industrial_medium_density"] = Tile(this->tileSize, 2, texmgr.getRef("industrial_medium_density"), { staticAnim }, TileType::INDUSTRIAL_MEDIUM_DENSITY, 50, 0, 1);
	//this->tileAtlas["industrial_low_density"] = Tile(this->tileSize, 2, texmgr.getRef("industrial_low_density"), { staticAnim }, TileType::INDUSTRIAL_LOW_DENSITY, 50, 0, 1);

	//Various Residential Densities
	//this->tileAtlas["residential_low_density"] = Tile(this->tileSize, 2, texmgr.getRef("residential_low_density"), { staticAnim }, TileType::RESIDENTIAL_LOW_DENSITY, 50, 0, 1);
	//this->tileAtlas["residential_medium_density"] = Tile(this->tileSize, 2, texmgr.getRef("residential_medium_density"), { staticAnim }, TileType::RESIDENTIAL_MEDIUM_DENSITY, 50, 0, 1);
	//this->tileAtlas["residential_high_density"] = Tile(this->tileSize, 2, texmgr.getRef("residential_high_density"), { staticAnim }, TileType::RESIDENTIAL_HIGH_DENSITY, 50, 0, 1);
	//this->tileAtlas["residential_towers"] = Tile(this->tileSize, 2, texmgr.getRef("residential_towers"), { staticAnim }, TileType::RESIDENTIAL_TOWERS, 50, 0, 1);
	//this->tileAtlas["residential_skyscrapers"] = Tile(this->tileSize, 2, texmgr.getRef("residential_skyscrapers"), { staticAnim }, TileType::RESIDENTIAL_SKYSCRAPERS, 50, 0, 1);
	//this->tileAtlas["residential_skyscrapers_huge"] = Tile(this->tileSize, 4, texmgr.getRef("residential_skyscrapers_huge"), { staticAnim }, TileType::RESIDENTIAL_SKYSCRAPERS_HUGE, 50, 0, 1);

	//this->tileAtlas["ahhh"] = Tile(this->tileSize, 1, texmgr.getRef("ahhh"), { staticAnim }, TileType::WHEAT, 50, 0, 1);
}

void Game::loadFonts()
{
	sf::Font font;
	font.loadFromFile("content/fonts/AmongUs-Regular.ttf");
	this->fonts["main_font"] = font;
	return;
}

void Game::loadStyleSheets()
{
	this->stylesheets["button"] = GuiStyle(&this->fonts.at("main_font"), 1, sf::Color(0xc6, 0xc6, 0xc6), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00),
		sf::Color(0x61, 0x61, 0x61), sf::Color(0x94, 0x94, 0x94), sf::Color(0x00, 0x00, 0x00));
	this->stylesheets["text"] = GuiStyle(&this->fonts.at("main_font"), 0,
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0xff, 0xff),
		sf::Color(0x00, 0x00, 0x00, 0x00), sf::Color(0x00, 0x00, 0x00), sf::Color(0xff, 0x00, 0x00));
	return;
}