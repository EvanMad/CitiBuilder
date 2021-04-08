#pragma once
#include "game_state_options.hpp"
#include "game_state_editor.hpp"
#include "game_state_start.hpp"
#include "game_state_newgame.hpp"
#include "game_state_loadgame.hpp"
#include <iostream>
#include <filesystem>
#include "util.hpp"


void GameStateLoadGame::draw(const float dt)
{
	this->game->window.setView(this->optionsView);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this->game->gui.draw();
	return;
}

void GameStateLoadGame::update(const float dt)
{

}

void GameStateLoadGame::handleInput()
{
	sf::Event event;

	sf::Vector2f mousePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->optionsView);

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

			optionsView.setSize(event.size.width, event.size.height);
			optionsView.setSize(event.size.width, event.size.height);
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->optionsView));
			this->game->background.setScale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) this->game->pushState(new GameStateStart(this->game));
		}
		//Menu Highlight
		case sf::Event::MouseMoved:
		{
			break;
		}
		//Menu Click
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{

			}
			break;
		}
		default: break;
		}
	}
	return;
}

void GameStateLoadGame::LoadGame(std::string world)
{
	GameStateEditor* gse = new GameStateEditor(this->game);
	//gse->city.map = Map(world , 32, 32, game->tileAtlas);
	//gse->city.shuffleTiles();
	gse->city.load(world, game->tileAtlas);
	gse->city.shuffleTiles();
	gse->city.name = remove_extension(base_name(world));
	this->game->pushState(gse);
}

GameStateLoadGame::GameStateLoadGame(Game* game)
{
	std::cout << "hello!" << std::endl;
	this->game = game;
	this->game->gui.removeAllWidgets();
	sf::Vector2f windowSize = sf::Vector2f(this->game->window.getSize());
	this->optionsView.setSize(windowSize);
	sf::Vector2f centrePos = windowSize * 0.5f;
	this->optionsView.setCenter(centrePos);
	sf::Vector2f pos = centrePos;

	//tgui::Button::Ptr btn = tgui::Button::create();
	//btn->setSize(192, 32);
	//btn->setPosition({ "50%" "50%" });
	//btn->setOrigin(0.5, 0.5);

	auto grid = tgui::Grid::create();
	grid->setSize({ "50%", "100%" });
	//grid->setAutoSize(true);
	//auto layout = tgui::VerticalLayout::create();
	//layout->setSize("100%", "100%");

	tgui::ScrollablePanel::Ptr scrollZone = tgui::ScrollablePanel::create();
	scrollZone->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	scrollZone->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
	scrollZone->setOrigin(0.5, 0.5);
	scrollZone->setPosition({ "50%", "50%" });
	scrollZone->setSize({ "50%", "50%" });
	scrollZone->add(grid);
	tgui::Button::Ptr btn;
	int i = 0;
	for (const auto& entry : std::filesystem::directory_iterator("saves"))
	{
		//std::string path_string{ entry.path().string() };
		//std::transform(path_string.begin(), path_string.end(), path_string.begin(), ::toupper);
		//path_string = remove_extension(base_name(path_string));
		btn = tgui::Button::create(tgui::String(entry.path()));
		//btn->setSize(768, 64);
		//btn-> setPosition({ "100%", "50%" });
		//btn->setSize({ "200%", "20" });
		btn->setSize("parent.width", 67);
		btn->setPosition(0, i * 67);
		//btn->setSize("(parent.size.x)*2, 67");
		//btn->setSize(scrollZone->getContentSize().x, 64);
		//btn->setPosition({ "50%", "50%" });
		btn->onClick(&GameStateLoadGame::LoadGame, this, std::string(btn->getText()));
		//btn->setInheritedFont(tgui::Font("content/fonts/AmongUs-Regular.ttf"));
		btn->setTextSize(35);
		scrollZone->add(btn);
		//grid->addWidget(btn, i, 0);
		i++;
	}

	//grid->setHeight(67 * i);

	//scrollZone->add(layout);
	this->game->gui.add(scrollZone);
	//game->gui.add(world_name_label, "World Name Label");
}