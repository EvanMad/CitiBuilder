#include <SFML/Graphics.hpp>
#include "game_state_start.hpp"
#include "game_state_editor.hpp"
#include "game_state.hpp"
#include "map.hpp"
#include <iostream>
#include "texture_manager.hpp"
#include "gui.hpp"
#include <string>
#include <map>

void GameStateEditor::draw(const float dt)
{
	this->game->window.clear(sf::Color::Black);
	this->game->window.draw(this->game->background);
	this->game->window.setView(this->gameView);
	gameView.setSize(std::floor(gameView.getSize().x) + 0.375f, std::floor(gameView.getSize().y) + 0.375f);
	this->city.map.draw(this->game->window, dt);
	this->game->window.setView(this->guiView);
	this->game->gui.draw();
	return;
}

void GameStateEditor::handleInput()
{
	sf::Event event;

	sf::Vector2f gamePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
	while (this->game->window.pollEvent(event))
	{
		//std::cout << ((char)this->actionState) << std::endl;
		this->game->gui.handleEvent(event);
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			game->window.close();
		}
		case sf::Event::Resized:
		{
			//gameView.setSize(event.size.width, event.size.height);
			//guiView.setSize(std::floor(event.size.width) + 0.375f, std::floor(event.size.height) + 0.375f);
			//gameView.setSize(std::floor(event.size.width) + 0.375f, std::floor(event.size.height) + 0.375f);
			//gameView.zoom(zoomLevel);
			//this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->gameView));
			//this->game->background.setScale(
				//float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				//float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				this->game->pushState(new GameStateStart(this->game));
				this->city.save(this->city.name);
			}
			if (event.key.code == sf::Keyboard::H)
			{
				this->city.save(this->city.name);
			}
			if (event.key.code == sf::Keyboard::J)
			{
				this->clickActionState = ClickAction::BRUSH;
			}
			if (event.key.code == sf::Keyboard::K)
			{
				this->clickActionState = ClickAction::SELECT;
			}
			if (event.key.code == sf::Keyboard::F)
			{
				this->OpenCloseMenu();
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			//if (this->backpanel->isVisible()) { return; }
			sf::Vector2i SelectionPoint;
			SelectionPoint.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
			SelectionPoint.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;

			this->city.map.SelectTile(SelectionPoint);
			if (this->clickActionState == ClickAction::BRUSH && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				this->actionState = ActionState::BRUSH;
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { this->city.map.PlaceSingleTile(SelectionPoint, *this->currentTile); }
			}
			// Camera Pan Stuff
			else if (this->actionState == ActionState::PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
				gameView.move(-1.0f * pos * this->zoomLevel);
				panningAnchor = sf::Mouse::getPosition(this->game->window);

			}

			//Select Tile Stuff
			else if (this->actionState == ActionState::SELECTING)
			{
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);

				selectionEnd.x = pos.y / (this->city.map.tileSize) + pos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
				selectionEnd.y = pos.y / (this->city.map.tileSize) - pos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;

				this->city.map.clearSelected();
				this->city.map.select(selectionStart, selectionEnd, {});
			}
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			//if (this->backpanel->isVisible()) { return; }
			//Pan Stuff
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				if (this->actionState != ActionState::PANNING)
				{
					this->actionState = ActionState::PANNING;
					this->panningAnchor = sf::Mouse::getPosition(this->game->window);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->backpanel->isVisible()) { return; }
				//	std::cout << static_cast<std::underlying_type<ActionState>::type>(this->clickActionState) << std::endl;
				if (this->clickActionState == ClickAction::SELECT)
				{
					this->actionState = ActionState::SELECTING;
					sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
					selectionStart.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
					selectionStart.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;
				}

				if (this->clickActionState == ClickAction::BRUSH)
				{
					sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
					sf::Vector2i SelectionPoint;
					SelectionPoint.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
					SelectionPoint.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;
					this->city.map.PlaceSingleTile(SelectionPoint, *this->currentTile);
					this->city.tileChanged();
				}


			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (this->backpanel->isVisible()) { return; }
				if (this->actionState == ActionState::SELECTING)
				{
					this->actionState = ActionState::NONE;
					this->city.map.clearSelected();
				}
				else
				{
					if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
						this->OpenCloseMenu();
						this->backpanel->setPosition(sf::Mouse::getPosition(this->game->window).x, sf::Mouse::getPosition(this->game->window).y + this->backpanel->getSize().y/2);
					//sf::Vector2f pos = guiPos;
					//if (pos.x > this->game->window.getSize().x - this->guiSystem.at("rightClickMenu").getSize().x)
				//	{
				//		//pos -= sf::Vector2f(this->guiSystem.at("rightClickMenu").getSize().x, 0);
				//	}
				//	if (pos.y > this->game->window.getSize().y - this->guiSystem.at("rightClickMenu").getSize().y)
				//	{
				//		//pos -= sf::Vector2f(0, this->guiSystem.at("rightClickMenu").getSize().y);
				//	}
				}
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			//if (this->backpanel->isVisible()) { return; }
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				this->actionState = ActionState::NONE;
			}
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::SELECTING)
				{
					if (this->currentTile != nullptr)
					{
						unsigned int cost = this->currentTile->cost * this->city.map.numSelected;
						if (this->city.funds >= cost)
						{
							this->city.bulldoze(*this->currentTile);
							this->city.tileChanged();
						}
					}
					this->actionState = ActionState::NONE;
					this->city.map.clearSelected();
				}
			}
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
			//if (this->backpanel->isVisible()) { return; }
			if (event.mouseWheel.delta < 0)
			{
				gameView.zoom(2.0f);
				zoomLevel *= 2.0f;
			}
			else
			{
				gameView.zoom(0.5f);
				zoomLevel *= 0.5f;
			}
			break;
		}
		default: break;
		}
	}
}

void GameStateEditor::update(const float dt)
{
	float x = gameView.getCenter().x;
	float y = gameView.getCenter().y;
	x = std::floor(x);
	y = std::floor(y);
	x += 0.375f;
	y += 0.375f;
	gameView.setCenter(x, y);
}

void toggle(tgui::Panel::Ptr pnl, std::vector<tgui::Panel::Ptr> panels)
{
	bool temp = pnl->isVisible();

	for (auto panel : panels)
	{
		panel->setVisible(false);
		//panel->setEnabled(false);
	}

	if (temp)
	{
		//pnl->setEnabled(false);
		pnl->setVisible(false);
	}
	else
	{
		//pnl->setEnabled(true);
		pnl->setVisible(true);
	}
}

void GameStateEditor::SelectTile(int tile)
{
	//this->currentTile = &this->game->tileAtlas[43];
	//this->currentTile = tile;
	this->currentTile = &this->validTiles[tile];
	this->backpanel->setVisible(false);
}

void GameStateEditor::OpenCloseMenu()
{
	bool inverseMenuOpen = !(this->backpanel->isVisible());
	this->backpanel->setVisible(inverseMenuOpen);
	this->backpanel->setPosition({ "50%", "92.5%" });
	this->backpanel->setSize({ "50%", "50%" });
}

GameStateEditor::GameStateEditor(Game* game)
{
	this->game = game;
	this->game->gui.removeAllWidgets();
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	//this->guiView.setSize(pos);
	this->gameView.setSize(pos);
	this->game->background.setScale(pos);
	pos *= 0.5f;
	sf::Vector2f centrepos = sf::Vector2f(int(pos.x), int(pos.y));
	this->gameView.setCenter(centrepos);
	this->guiView.setCenter(centrepos);

	this->city.funds = 9999999;

	this->city.shuffleTiles();

	this->zoomLevel = 1.0f;

	sf::Vector2f centre(this->city.map.width * 0.5, this->city.map.height * 0.5);
	centre *= float(this->city.map.tileSize);
	this->gameView.setCenter(centre);

	this->selectionStart = sf::Vector2i(0, 0);
	this->selectionEnd = sf::Vector2i(0, 0);
	this->currentTile = &this->game->tileAtlas[12];

	ActionState::NONE;
	std::string name;

	//GUI Stuff

	std::vector<tgui::Panel::Ptr> panels;

	tgui::Layout2d buttonSize = tgui::Layout2d(384, 64);
	this->panel = tgui::Panel::create();
	this->panel->setSize("100%", "7.5%");
	this->panel->setPosition("0%", "92.5%");
	this->panel->getRenderer()->setBackgroundColor(tgui::Color(204, 204, 204));
	this->panel->getRenderer()->setBorderColor(tgui::Color(255, 255, 255, 255));
	this->game->gui.add(this->panel);

	//tgui::Panel::Ptr menu = tgui::Panel::create();
	//menu->setPosition("50%", "50%");
	//menu->setOrigin(0.5, 0.5);
	//menu->setSize("50%", "50%");
	//this->game->gui.add(menu);

	//this->box = tgui::Grid::create();
	//this->box->setPosition("50%", "50%");
	//this->box->setOrigin(0.5, 0.5);
	//this->box->setSize("100%", "100%");
	//this->game->gui.add(box);
	//this->box->setAutoSize(false);
	//panel->add(box);
	//this->game->gui.add(this->box);

	//this->backpanel = tgui::Panel::create();
	this->backpanel = tgui::ChildWindow::create();
	this->backpanel->setTitle("Tile Selection Menu");
	this->backpanel->setTitleTextSize(18);
	this->backpanel->setInheritedFont(tgui::Font("content/fonts/rct2.ttf"));
	this->backpanel->getRenderer()->setTitleBarColor(tgui::Color(115, 115, 115, 255));
	this->backpanel->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Left);
	this->backpanel->setSize({ "50%", "50%" });
	this->backpanel->setResizable(true);
	this->backpanel->setTitleButtons(tgui::ChildWindow::Minimize);
	//this->backpanel->setPosition({ "50%", "50%" });
	this->backpanel->setPosition({ "50%", "92.5%" });
	this->backpanel->setOrigin(0.5, 1);
	//this->backpanel->getRenderer()->setBackgroundColor(tgui::Color(204, 204, 204));
	//this->backpanel->getRenderer()->setBorderColor(tgui::Color(255, 255, 255, 255));
	//this->backpanel->getRenderer()->setTexture("")
	//this->backpanel->getRenderer()->setTextureBackground("content/graphics/bg_test.png");
	this->backpanel->setVisible(false);
	this->game->gui.add(this->backpanel);

	this->scrollZone = tgui::ScrollablePanel::create();
	//this->scrollZone->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	this->scrollZone->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Always);
	this->scrollZone->setOrigin(0.5, 0.5);
	this->scrollZone->setPosition({ "50%", "50%" });
	this->scrollZone->setSize({ "100%", "100%" });
	//this->scrollZone->setVisible(false);
	this->scrollZone->getRenderer()->setBackgroundColor(tgui::Color(204, 204, 204, 255));
	this->scrollZone->getRenderer()->setBorderColor(tgui::Color(255, 255, 255, 255));
	this->backpanel->add(scrollZone);

	//tgui::MenuBar::Ptr menubar = tgui::MenuBar::create();
	//menubar->setSize("100%", 25);
	//menubar->addMenu("Tile Selection Menu");
	//menubar->getRenderer()->setBackgroundColor(tgui::Color(115, 115, 115, 255));
	//this->backpanel->add(menubar);

	//this->game->gui.add(scrollZone);
	//this->scrollZone->add(box);
	int lastline = 0;
	int xpos = -1;
	int amount = 0;
	int buffermultipler_x = 0;
	int buffermultipler_y = 0;
	int buffer_x = 4;
	int buffer_y = 4;
	for (auto tile : this->game->tileAtlas)
	{
		if (tile.tileType != -842150451)
		{
			this->validTiles.push_back(tile);
			amount++;
		}
	}
	int linecount = amount / 8;
	int linewidth = 8; //this->scrollZone->getSize().x / 96;
	for (int x = 0; x < amount; x++)
	{
		tgui::BitmapButton::Ptr btn = tgui::BitmapButton::create();
		btn->setSize(112, 112);
		//btn->setImage("content/tiles/icons/residential_towers.png");
		//btn->setImage(this->game->icons[x]);

		btn->setImage(this->game->icons[x]);
		//btn->setImage(this->validTiles[x].icon);

		//const sf::Texture* tex = validTiles[x].sprite.getTexture();
		//btn->setImage(*tex);
		btn->getRenderer()->setBackgroundColor(tgui::Color(123, 123, 123, 255));
		btn->getRenderer()->setBorderColor(tgui::Color(0, 0, 0, 0));
		btn->onClick(&GameStateEditor::SelectTile, this, x);
		btn->getRenderer()->setBackgroundColorDownHover(tgui::Color(0, 0, 0, 255));
		//btn->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0, 0));
		int y = x / linewidth;
		xpos++;
		if (y != lastline)
		{
			//New Line
			buffer_x = 0;
			buffer_y = 0;
			xpos = 0;
			buffermultipler_x = 0;
		}
		//else { buffer = x*4; }
		buffermultipler_y = y;
		buffermultipler_x++;
		buffermultipler_y++;
		buffer_x = buffermultipler_x * 4;
		buffer_y = buffermultipler_y * 4;
		lastline = y;
		btn->setPosition((xpos * 112) + 10 + buffer_x, (y * 112) + 32 + buffer_y);
		scrollZone->add(btn);
	}

	auto openBtn = tgui::Button::create("TILE MENU");
	openBtn->setOrigin(0.5, 0.5);
	openBtn->setPosition("50%", "50%");
	openBtn->setSize("50%", "100%");
	openBtn->getRenderer()->setBackgroundColor(tgui::Color(180, 180, 180, 255));
	openBtn->onClick(&GameStateEditor::OpenCloseMenu, this);
	//openBtn->setInheritedFont(tgui::Font("content/fonts/rct2.ttf"));
	openBtn->setInheritedFont(tgui::Font("content/fonts/AmongUs-Regular.ttf"));
	openBtn->setTextSize(42);
	this->panel->add(openBtn);

	/*
	for (int i = 1; i < 8; i++)
	{
		tgui::Panel::Ptr panel2 = tgui::Panel::create();
		int t = 10 * i;
		std::string text = std::to_string(t);
		text += "%";
		const char* test = text.c_str();
		panel2->setSize("10%", "80%");
		panel2->setPosition(test, "92.5%");
		panel2->setOrigin(0.5, 1);
		panel2->getRenderer()->setBackgroundColor(tgui::Color(204, 204, 204));
		panel2->moveToBack();
		//panel2->setEnabled(false);
		panel2->setVisible(false);
		this->game->gui.add(panel2);
		panels.push_back(panel2);

		tgui::BitmapButton::Ptr btn = tgui::BitmapButton::create();
		btn->setImage("content/graphics/residential_test.png");
		btn->setImageScaling(1);
		btn->setOrigin(0.5, 0.5);
		btn->getRenderer()->setBackgroundColor(tgui::Color(00, 0, 0, 0));
		btn->getRenderer()->setBorderColor(tgui::Color(00, 0, 0, 0));
		t = 10 * i;
		text = std::to_string(t);
		text += "%";
		test = text.c_str();
		btn->setPosition(test, "50%");
		btn->setSize( "10%", "100%");
		btn->onClick(toggle, panel2, panels);
		btn->moveToFront();
		panel->add(btn);

		for (int y = 0; y < 5; y++)
		{
			t = 100 - (10 * y);
			text = std::to_string(t);
			text += "%";
			test = text.c_str();
			tgui::BitmapButton::Ptr panelbtn = tgui::BitmapButton::create();
			panelbtn->setImage("content/graphics/residential_test.png");
			panelbtn->setImageScaling(1);
			panelbtn->setOrigin(0.5, 1);
			panelbtn->getRenderer()->setBackgroundColor(tgui::Color(00, 0, 0, 0));
			panelbtn->getRenderer()->setBorderColor(tgui::Color(00, 0, 0, 0));
			panelbtn->setPosition("50%", test);
			panelbtn->setSize("100%", "10%");
			panelbtn->moveToFront();
			panel2->add(panelbtn);
		}
	}
	*/
}