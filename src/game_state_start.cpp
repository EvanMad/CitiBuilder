#include <SFML/Graphics.hpp>
#include "game_state_start.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include "game_state_options.hpp"
#include "game_state_newgame.hpp"
#include "game_state_loadgame.hpp"
#include <iostream>

void GameStateStart::draw(const float dt)
{
	this->game->window.setView(this->view);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this->game->gui.draw();
	for (auto gui : this->guiSystem) this->game->window.draw(gui.second);
	return;
}

void GameStateStart::update(const float dt)
{

}

void GameStateStart::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->view);

	while (this->game->window.pollEvent(event))
	{
		this->game->gui.handleEvent(event);

		switch (event.type)
		{
			case sf::Event::Closed:
			{
				game->window.close();
			}
			case sf::Event::Resized:
			{
				/*
				view.setSize(event.size.width, event.size.height);
				view.setSize(event.size.width, event.size.height);
				this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->view));
				this->game->background.setScale(
					float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
					float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
				break;
				std::cout << event.size.width << std::endl;
				*/
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Escape) this->game->window.close();
				//if (event.key.code == sf::Keyboard::Space) this->loadgame();
				std::cout << event.key.code << std::endl;
				break;
			}
			//Menu Highlight
			case sf::Event::MouseMoved:
			{
				/*
				this->guiSystem.at("menu").highlight(this->guiSystem.at("menu").getEntry(mousePos));
				this->guiSystem.at("menu2").highlight(this->guiSystem.at("menu2").getEntry(mousePos));
				this->guiSystem.at("menu3").highlight(this->guiSystem.at("menu3").getEntry(mousePos));
				*/
				break;
			}
			//Menu Click
			case sf::Event::MouseButtonPressed:
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					/*
					std::string msg = this->guiSystem.at("menu").activate(mousePos);
					if (msg == "load_game")
					{
						this->loadgame();
					}
					msg = this->guiSystem.at("menu2").activate(mousePos);
					if (msg == "new_game")
					{
						this->newgame();
					}
					msg = this->guiSystem.at("menu3").activate(mousePos);
					if (msg == "options")
					{
						this->options();
					}
					*/
				}
				break;
			}
			default: break;
		}
	}
	return;
}

void GameStateStart::loadgame()
{
	printf("space");
	//GameStateEditor* gse = new GameStateEditor(this->game);
	//this->game->pushState(gse);
	//gse->city.funds = 69;
	//gse->city = City("saves/citi_test", this->game->tileSize, this->game->tileAtlas);
	//gse->city.shuffleTiles();

	GameStateLoadGame* gslg = new GameStateLoadGame(this->game);
	this->game->pushState(gslg);
	return;
}

void GameStateStart::newgame()
{
	//GameStateEditor* gse = new GameStateEditor(this->game);
	GameStateNewGame* gsng = new GameStateNewGame(this->game);
	this->game->pushState(gsng);
}

void GameStateStart::options()
{
	GameStateOptions* gso = new GameStateOptions(this->game);
	this->game->pushState(gso);
	std::cout << "options" << std::endl;
}

void callbackFunc()
{
	std::cout << "Button pressed" << std::endl;
}

GameStateStart::GameStateStart(Game* game)
{
    this->game = game;
	this->game->gui.removeAllWidgets();
    sf::Vector2f windowSize = sf::Vector2f(this->game->window.getSize());
    this->view.setSize(windowSize);
	sf::Vector2f centrePos = windowSize * 0.5f;
	this->view.setCenter(centrePos);
	sf::Vector2f pos = centrePos;

	//tgui::Label::Ptr logoPlaceholder = tgui::Label::create("Insert Logo Here :)");
	//logoPlaceholder->setPosition({});
	//game->gui.add(logoPlaceholder);

	tgui::Layout2d buttonSize = tgui::Layout2d(384, 64);
	pos.y += 64;
	tgui::Button::Ptr newGameButton = tgui::Button::create("NEW GAME");
	newGameButton->setSize(buttonSize);
	newGameButton->setOrigin(0.5,0.5);
	newGameButton->setPosition({"50%", "50%"});
	newGameButton->onPress(&GameStateStart::newgame, this);
	game->gui.add(newGameButton, "Test");

	pos.y -= newGameButton->getSize().y + 20;
	tgui::Button::Ptr loadGameButton = tgui::Button::create("LOAD GAME");
	loadGameButton->setSize(buttonSize);
	loadGameButton->setOrigin(0.5, 0.5);
	loadGameButton->setPosition({ "50%", "43.75%" });
	loadGameButton->onPress(&GameStateStart::loadgame, this);
	game->gui.add(loadGameButton, "Test");

	pos.y -= loadGameButton->getSize().y + 20;
	tgui::Button::Ptr optionsButton = tgui::Button::create("OPTIONS");
	optionsButton->setSize(buttonSize);
	optionsButton->setOrigin(0.5, 0.5);
	optionsButton->setPosition({ "50%", "56.25%" });
	optionsButton->onPress(&GameStateStart::options, this);
	game->gui.add(optionsButton, "Test");
	

	newGameButton->setInheritedFont(tgui::Font("content/fonts/AmongUs-Regular.ttf"));
	loadGameButton->setInheritedFont(tgui::Font("content/fonts/AmongUs-Regular.ttf"));
	optionsButton->setInheritedFont(tgui::Font("content/fonts/AmongUs-Regular.ttf"));  
	loadGameButton->setTextSize(35);
	newGameButton->setTextSize(35);
	optionsButton->setTextSize(35);

	/*
	this->guiSystem.emplace("title", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("text"), { std::make_pair("CitiBuilder", "void") }));
	this->guiSystem.at("title").setPosition(pos);
	this->guiSystem.at("title").setOrigin(96, 64+48);
	this->guiSystem.at("title").show();

	this->guiSystem.emplace("menu", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("button"), { std::make_pair("Load Game", "load_game") }));
	this->guiSystem.emplace("menu2", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("button"), { std::make_pair("New Game", "new_game") }));
	this->guiSystem.emplace("menu3", Gui(sf::Vector2f(192, 32), 4, false, game->stylesheets.at("button"), { std::make_pair("Options", "options") }));

	this->guiSystem.at("menu").setPosition(pos);
	this->guiSystem.at("menu").setOrigin(96, 32 * 1 / 2);
	this->guiSystem.at("menu").show();

	this->guiSystem.at("menu2").setPosition(pos);
	this->guiSystem.at("menu2").setOrigin(96, 64);
	this->guiSystem.at("menu2").show();

	this->guiSystem.at("menu3").setPosition(pos);
	this->guiSystem.at("menu3").setOrigin(96, -32);
	this->guiSystem.at("menu3").show();
	*/
}