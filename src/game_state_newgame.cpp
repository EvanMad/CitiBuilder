#pragma once
#include "game_state_options.hpp"
#include "game_state_editor.hpp"
#include "game_state_start.hpp"
#include "game_state_newgame.hpp"
#include <iostream>

void GameStateNewGame::draw(const float dt)
{
	this->game->window.setView(this->optionsView);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this->game->gui.draw();
	return;
}

void GameStateNewGame::update(const float dt)
{

}

void GameStateNewGame::handleInput()
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

void dump()
{
	std::cout << "Based" << std::endl;
}

void GameStateNewGame::GenerateWorld(tgui::EditBox::Ptr box, tgui::EditBox::Ptr seedbox, tgui::Slider::Ptr freq, tgui::EditBox::Ptr world_size)
{
	GameStateEditor* gse = new GameStateEditor(this->game);
	float ahh = freq->getValue();
	unsigned int seed = seedbox->getText().toUInt();
	gse->city.RandomCity(seed, (freq->getValue()), game->tileAtlas, world_size->getText().toUInt());
	//gse->city.map = Map("maps/randomworld.dat", 32, 32, game->tileAtlas);
	std::string worldname = std::string(box->getText());
	gse->city.name = worldname;
	//gse->city.save(worldname);
	gse->city.shuffleTiles();
	this->game->pushState(gse);
}

GameStateNewGame::GameStateNewGame(Game* game)
{
	std::cout << "hello!" << std::endl;
	this->game = game;
	this->game->gui.removeAllWidgets();
	sf::Vector2f windowSize = sf::Vector2f(this->game->window.getSize());
	this->optionsView.setSize(windowSize);
	sf::Vector2f centrePos = windowSize * 0.5f;
	this->optionsView.setCenter(centrePos);
	sf::Vector2f pos = centrePos;

	tgui::Layout2d buttonSize = tgui::Layout2d(384, 64);
	tgui::Slider::Ptr slider = tgui::Slider::create();
	slider->setSize(buttonSize);
	slider->setOrigin(0.5, 0.5);
	slider->setPosition("50%", "25%");
	slider->setMaximum(0.15f);
	slider->setMinimum(0.0f);
	slider->setStep(0.01f);

	tgui::EditBox::Ptr world_name = tgui::EditBox::create();
	//tgui::Slider::Ptr edit = tgui::Slider::create();
	pos.y += 64;
	//tgui::Button::Ptr test = tgui::Button::create("Based Game");
	world_name->setSize(buttonSize);
	world_name->setOrigin(0.5, 0.5);
	world_name->setPosition({ "50%", "10%" });
	world_name->setText("World Name");

	char test = char("a");

	tgui::EditBox::Ptr world_size = tgui::EditBox::create();
	//tgui::Slider::Ptr edit = tgui::Slider::create();
	pos.y += 64;
	//tgui::Button::Ptr test = tgui::Button::create("Based Game");
	world_size->setSize(buttonSize);
	world_size->setOrigin(0.5, 0.5);
	world_size->setPosition({ "50%", "32.5%" });
	world_size->setText("World Size (Number)");

	tgui::Label::Ptr world_name_label = tgui::Label::create("World Name");
	world_name_label->setPosition({ "50%", "50%" });
	world_name_label->setText("World Name");

	tgui::EditBox::Ptr seed_box = tgui::EditBox::create();
	seed_box->setSize(buttonSize);
	seed_box->setOrigin(0.5, 0.5);
	srand(time(NULL));
	int ahhhh = rand();
	seed_box->setText(tgui::String(ahhhh));
	//seed_box->setText("Seed (Number)");
	seed_box->setPosition({ "50%", "17.5%" });

	tgui::Button::Ptr generate_button = tgui::Button::create("Generate World");
	generate_button->setPosition({ "50%", "80%" });
	generate_button->setOrigin(0.5, 0.5);
	generate_button->setSize(buttonSize);
	generate_button->onPress(&GameStateNewGame::GenerateWorld, this, world_name, seed_box, slider, world_size);

	//edit->onPress(&dump);
	this->game->gui.add(world_name, "Test");
	this->game->gui.add(generate_button, "GenBtn");
	this->game->gui.add(seed_box, "SeedBox");
	this->game->gui.add(world_size, "WordSize");
	this->game->gui.add(slider);

	//game->gui.add(world_name_label, "World Name Label");
}