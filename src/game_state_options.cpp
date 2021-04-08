#pragma once
#include "game_state_options.hpp"
#include "game_state_editor.hpp"
#include "game_state_start.hpp"
#include <iostream>

void GameStateOptions::draw(const float dt)
{
	this->game->window.setView(this->optionsView);
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this->game->gui.draw();
	return;
}

void GameStateOptions::update(const float dt)
{

}

void GameStateOptions::handleInput()
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

GameStateOptions::GameStateOptions(Game* game)
{
	this->game = game;
	this->game->gui.removeAllWidgets();
	sf::Vector2f windowSize = sf::Vector2f(this->game->window.getSize());
	this->optionsView.setSize(windowSize);
	sf::Vector2f centrePos = windowSize * 0.5f;
	this->optionsView.setCenter(centrePos);
	sf::Vector2f pos = centrePos;

	//tgui::EditBox::Ptr edit = tgui::EditBox::create();
	tgui::Slider::Ptr edit = tgui::Slider::create();
	tgui::Layout2d buttonSize = tgui::Layout2d(384, 64);
	pos.y += 64;
	//tgui::Button::Ptr test = tgui::Button::create("Based Game");
	edit->setSize(buttonSize);
	edit->setOrigin(0.5, 0.5);
	edit->setPosition({ "50%", "10%" });
	//edit->onPress(&dump);
	game->gui.add(edit, "Test");
}