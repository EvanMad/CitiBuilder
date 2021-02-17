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

	this->game->window.setView(this->guiView);
	this->game->window.draw(this->game->background);
	gameView.setSize(std::floor(gameView.getSize().x) + 0.375f, std::floor(gameView.getSize().y) + 0.375f);
	this->game->window.setView(this->gameView);
	city.map.draw(this->game->window, dt);

	this->game->window.setView(this->guiView);
	for (auto gui : this->guiSystem) this->game->window.draw(gui.second);

	return;
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

	this->city.update(dt);
	this->guiSystem.at("infoBar").setEntryText(0, "Day: " + std::to_string(this->city.day));
	this->guiSystem.at("infoBar").setEntryText(0, "$" + std::to_string(long(this->city.funds)));
	this->guiSystem.at("infoBar").setEntryText(2, std::to_string(long(this->city.population)) + " (" + std::to_string(long(this->city.getHomeless())) + ")");
	this->guiSystem.at("infoBar").setEntryText(3, std::to_string(long(this->city.employable)) + " (" + std::to_string(long(this->city.getUnemployed())) + ")");
	this->guiSystem.at("infoBar").setEntryText(4, tileTypeToStr(this->currentTile->tileType));

	this->guiSystem.at("rightClickMenu").highlight(this->guiSystem.at("rightClickMenu").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	
	for (auto menu : this->submenus)
	{
		//menu.second.highlight(menu.second.getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
		this->guiSystem.at(menu.first).highlight(this->guiSystem.at(menu.first).getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	}

	//this->guiSystem.at("rightClickSubMenu_residential").highlight(this->guiSystem.at("rightClickSubMenu_residential").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	//this->guiSystem.at("rightClickSubMenu_commercial").highlight(this->guiSystem.at("rightClickSubMenu_commercial").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	//this->guiSystem.at("rightClickSubMenu_industrial").highlight(this->guiSystem.at("rightClickSubMenu_industrial").getEntry(this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView)));
	return;
}

void GameStateEditor::handleInput()
{
	sf::Event event;

	sf::Vector2f guiPos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->guiView);
	sf::Vector2f gamePos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
	while (this->game->window.pollEvent(event))
	{
		//std::cout << ((char)this->actionState) << std::endl;
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			game->window.close();
		}
		case sf::Event::Resized:
		{
			//gameView.setSize(event.size.width, event.size.height);
			gameView.setSize(std::floor(event.size.width)+0.375f, std::floor(event.size.height)+0.375f);
			gameView.zoom(zoomLevel);
			guiView.setSize(event.size.width, event.size.height);
			this->guiSystem.at("infoBar").setDimensions(sf::Vector2f(event.size.width / this->guiSystem.at("infoBar").entries.size(), 16));
			this->guiSystem.at("infoBar").setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, event.size.height - 16), this->guiView));
			this->guiSystem.at("infoBar").show();
			this->game->background.setPosition(this->game->window.mapPixelToCoords(sf::Vector2i(0, 0), this->guiView));
			this->game->background.setScale(
				float(event.size.width) / float(this->game->background.getTexture()->getSize().x),
				float(event.size.height) / float(this->game->background.getTexture()->getSize().y));
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (event.key.code == sf::Keyboard::Escape) this->game->pushState(new GameStateStart(this->game)); this->city.save(this->city.name);
			if (event.key.code == sf::Keyboard::H)
			{
				this->city.save(this->city.name);
			}
			if (event.key.code == sf::Keyboard::J)
			{
				this->clickActionState = ClickAction::BRUSH;
				//this->actionState = ActionState::BRUSH;
			}
			if (event.key.code == sf::Keyboard::K)
			{
				//this->actionState = ActionState::SELECTING;
				this->clickActionState = ClickAction::SELECT;
			}
			break;
		}
		case sf::Event::MouseMoved:
		{
			sf::Vector2i SelectionPoint;
			SelectionPoint.x = gamePos.y / (this->city.map.tileSize) + gamePos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
			SelectionPoint.y = gamePos.y / (this->city.map.tileSize) - gamePos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;

			this->city.map.SelectTile(SelectionPoint);
			if (this->clickActionState == ClickAction::BRUSH && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			//if (this->actionState == ActionState::BRUSH)
			{
				this->actionState = ActionState::BRUSH;
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);
				if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) { this->city.map.PlaceSingleTile(SelectionPoint, *this->currentTile); }
			}
			// Camera Pan Stuff
			else if (this->actionState == ActionState::PANNING)
			{
				sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(this->game->window) - this->panningAnchor);
				//gameView.move(-1.0f * (roundVector2f(pos, this->zoomLevel)));
				gameView.move(-1.0f * pos * this->zoomLevel);
				panningAnchor = sf::Mouse::getPosition(this->game->window);
				//gameView.setCenter(roundVector2f(pos, this->zoomLevel));
				//gameView.setCenter(-1.0f * pos * this->zoomLevel);

			}
			
			//Select Tile Stuff
			else if (this->actionState == ActionState::SELECTING)
			{
				sf::Vector2f pos = this->game->window.mapPixelToCoords(sf::Mouse::getPosition(this->game->window), this->gameView);

				//std::cout << "test" << std::endl;
				selectionEnd.x = pos.y / (this->city.map.tileSize) + pos.x / (2 * this->city.map.tileSize) - this->city.map.width * 0.5 - 0.5;
				selectionEnd.y = pos.y / (this->city.map.tileSize) - pos.x / (2 * this->city.map.tileSize) + this->city.map.width * 0.5 + 0.5;

				this->city.map.clearSelected();
				if (this->currentTile->tileType == TileType::GRASS)
				{
					this->city.map.select(selectionStart, selectionEnd, { this->currentTile->tileType, TileType::WATER });
				}
				else
				{
					this->city.map.select(selectionStart, selectionEnd,
						{
						this->currentTile->tileType,
						TileType::FOREST,
						TileType::WATER,
						TileType::RESIDENTIAL,
						TileType::ROAD,
						TileType::COMMERCIAL,
						TileType::INDUSTRIAL
						});
				}

				this->guiSystem.at("selectionCostText").setEntryText(0, "$" + std::to_string(this->currentTile->cost * this->city.map.numSelected));
				if (this->city.funds <= double(this->city.map.numSelected) * this->currentTile->cost)
					this->guiSystem.at("selectionCostText").highlight(0);
				else
					this->guiSystem.at("selectionCostText").highlight(-1);
				this->guiSystem.at("selectionCostText").setPosition(guiPos + sf::Vector2f(16, -16));
				this->guiSystem.at("selectionCostText").show();
			}
			this->guiSystem.at("rightClickMenu").highlight(this->guiSystem.at("rightClickMenu").getEntry(guiPos));
			break;
		}

		case sf::Event::MouseButtonPressed:
		{

			//Pan Stuff
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				this->guiSystem.at("rightClickMenu").hide();
				this->guiSystem.at("selectionCostText").hide();
				closeAllSubMenus();

				if (this->actionState != ActionState::PANNING)
				{
					this->actionState = ActionState::PANNING;
					this->panningAnchor = sf::Mouse::getPosition(this->game->window);
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->guiSystem.at("rightClickMenu").visible == true)
				{
					std::string msg = this->guiSystem.at("rightClickMenu").activate(guiPos);
					if (this->game->tileAtlas.find(msg) != this->game->tileAtlas.end() and (msg != "flatten" && msg != "null")) { this->currentTile = &this->game->tileAtlas.at(msg); this->closeAllSubMenus(); };
					//if (msg != "flatten" && msg != "null") { this->currentTile = &this->game->tileAtlas.at(msg); }
					//this->guiSystem.at("rightClickMenu").hide();

					if (this->guiSystem.find("rightClickSubMenu_" + msg) != this->guiSystem.end())
					{
						//std::cout << "rightClickSubMenu_" + msg << std::endl;
						this->closeAllSubMenus();
						this->guiSystem.at("rightClickSubMenu_" + msg).setPosition(this->guiSystem.at("rightClickMenu").getPosition() + sf::Vector2f(this->guiSystem.at("rightClickMenu").getSize().x, 0));
						this->guiSystem.at("rightClickSubMenu_" + msg).show();
					}
					else
					{
						this->guiSystem.at("rightClickMenu").hide();
					}
					this->city.tileChanged();

					for (auto menu : submenus)
					{
						//std::cout << this->guiSystem.at(menu.first).visible << std::endl;
						if (this->guiSystem.at(menu.first).visible)
						{
							std::string submsg = this->guiSystem.at(menu.first).activate(guiPos);
							//std::cout << submsg << std::endl;
							if (submsg != "null") {
								this->guiSystem.at(menu.first).hide();
								this->currentTile = &this->game->tileAtlas.at(submsg);
							}
						}
					}
				}
				else
				{
					std::cout << static_cast<std::underlying_type<ActionState>::type>(this->clickActionState) << std::endl;
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
						//this->city.map.SelectTile(SelectionPoint, *this->currentTile);
						this->city.map.PlaceSingleTile(SelectionPoint, *this->currentTile);
					}
					
				}
			}
			else if (event.mouseButton.button == sf::Mouse::Right)
			{
				if (this->actionState == ActionState::SELECTING)
				{
					this->actionState = ActionState::NONE;
					this->guiSystem.at("selectionCostText").hide();
					this->city.map.clearSelected();
				}
				else
				{
					sf::Vector2f pos = guiPos;
					if (pos.x > this->game->window.getSize().x - this->guiSystem.at("rightClickMenu").getSize().x)
					{
						pos -= sf::Vector2f(this->guiSystem.at("rightClickMenu").getSize().x, 0);
					}
					if (pos.y > this->game->window.getSize().y - this->guiSystem.at("rightClickMenu").getSize().y)
					{
						pos -= sf::Vector2f(0, this->guiSystem.at("rightClickMenu").getSize().y);
					}
					this->guiSystem.at("rightClickMenu").setPosition(pos);
					this->guiSystem.at("rightClickMenu").show();
					closeAllSubMenus();
				}
			}
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			/* Stop panning */
			if (event.mouseButton.button == sf::Mouse::Middle)
			{
				this->actionState = ActionState::NONE;
			}
			/* Stop selecting */
			else if (event.mouseButton.button == sf::Mouse::Left)
			{
				if (this->actionState == ActionState::SELECTING)
				{
					/* Replace tiles if enough funds and a tile is selected */
					if (this->currentTile != nullptr)
					{
						unsigned int cost = this->currentTile->cost * this->city.map.numSelected;
						if (this->city.funds >= cost)
						{
							this->city.bulldoze(*this->currentTile);
							//this->city.funds -= this->currentTile->cost * this->city.map.numSelected;
							this->city.tileChanged();
						}
					}
					this->guiSystem.at("selectionCostText").hide();
					this->actionState = ActionState::NONE;
					this->city.map.clearSelected();
				}
			}
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
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

void GameStateEditor::closeAllSubMenus()
{
	for (auto menu : this->submenus)
	{
		this->guiSystem.at(menu.first).hide();
	}
}

GameStateEditor::GameStateEditor(Game* game)
{
	this->game = game;
	this->game->gui.removeAllWidgets();
	sf::Vector2f pos = sf::Vector2f(this->game->window.getSize());
	this->guiView.setSize(pos);
	this->gameView.setSize(pos);
	pos *= 0.5f;
	sf::Vector2f centrepos = sf::Vector2f(int(pos.x), int(pos.y));
	this->guiView.setCenter(centrepos);
	this->gameView.setCenter(centrepos);

	//map = map("saves/test.dat", 96, 96, game->tileAtlas);
	//city = City("citi_test", this->game->tileSize, this->game->tileAtlas);
	this->city.funds = 9999999;
	//this->city.population = 10000;
	//this->city.map = Map("saves/test.dat", 32, 32, game->tileAtlas);
	this->city.shuffleTiles();

	this->zoomLevel = 1.0f;

	sf::Vector2f centre(this->city.map.width * 0.5, this->city.map.height * 0.5);
	centre *= float(this->city.map.tileSize);
	gameView.setCenter(centre);

	this->selectionStart = sf::Vector2i(0, 0);
	this->selectionEnd = sf::Vector2i(0, 0);
	this->currentTile = &this->game->tileAtlas.at("grass");
	
	ActionState::NONE;

	// CREATE GUI!!!
	this->guiSystem.emplace("rightClickMenu", Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
		{
			std::make_pair("Flatten $" + this->game->tileAtlas["grass"].getCost(), "grass"),
			std::make_pair("Forest $" + this->game->tileAtlas["forest"].getCost(), "forest"),

			std::make_pair("Residential Zone","residential"),
			std::make_pair("Commercial Zone","commercial"),
			std::make_pair("Industrial Zone", "industrial"),

			std::make_pair("Flood Zone" + this->game->tileAtlas["water"].getCost(), "water"),
			std::make_pair("Deep Flood Zone $" + this->game->tileAtlas["deepwater"].getCost(), "deepwater"),
			std::make_pair("Road $" + this->game->tileAtlas["road"].getCost(), "road"),
			std::make_pair("Rail $" + this->game->tileAtlas["rail"].getCost(), "rail"),
			std::make_pair("Wheat Field $" + this->game->tileAtlas["wheat"].getCost(), "wheat"),
			std::make_pair("Wood Fence $" + this->game->tileAtlas["woodfence"].getCost(), "woodfence"),
			std::make_pair("Station $" + this->game->tileAtlas["station"].getCost(), "station"),
			std::make_pair("Tree $" + this->game->tileAtlas["tree"].getCost(), "tree"),

			std::make_pair("Dirt $" + this->game->tileAtlas["dirt"].getCost(), "dirt"),
			std::make_pair("AHHH $" + this->game->tileAtlas["ahhh2"].getCost(), "ahhh2"),
		std::make_pair("Sand $" + this->game->tileAtlas["dirt"].getCost(), "sand"),
		}
	));

	//std::vector<std::pair<std::string, Gui>> submenus;
	std::string name;

	Gui rightClickSubMenu_residential = Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
		{
			std::make_pair("High Density", "residential_high_density"),
			std::make_pair("Medium Density", "residential_medium_density"),
			std::make_pair("Low Density", "residential_low_density"),
			std::make_pair("Towers", "residential_towers"),
			std::make_pair("SkyScrapers", "residential_skyscrapers"),
			std::make_pair("SkyScrapers 2.0", "residential_skyscrapers_huge"),
		}
	);
	std::string test = std::string("rightClickSubMenu_residential");
	submenus.emplace(std::string("rightClickSubMenu_residential"), rightClickSubMenu_residential);

	Gui rightClickSubMenu_industrial = Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
		{
			std::make_pair("High Density", "industrial_high_density"),
			std::make_pair("Medium Density", "industrial_medium_density"),
			std::make_pair("Low Density", "industrial_low_density"),
		}
	);
	submenus.emplace(std::string("rightClickSubMenu_industrial"), rightClickSubMenu_industrial);

	Gui rightClickSubMenu_commercial = Gui(sf::Vector2f(196, 16), 2, false, this->game->stylesheets.at("button"),
		{
			std::make_pair("High Density", "commercial_high_density"),
			std::make_pair("Medium Density", "commercial_medium_density"),
			std::make_pair("Low Density", "commercial_low_density"),
		}
	);
	submenus.emplace(std::string("rightClickSubMenu_commercial"), rightClickSubMenu_commercial);

	for (auto submenu : submenus)
	{
		std::cout << submenu.first << std::endl;
		this->guiSystem.emplace(submenu.first, submenu.second);
	}

	this->guiSystem.emplace("selectionCostText", Gui(sf::Vector2f(196, 16), 0, false, this->game->stylesheets.at("text"), { std::make_pair("", "") }));
	this->guiSystem.emplace("infoBar", Gui(sf::Vector2f(this->game->window.getSize().x / 5, 16), 2, true, this->game->stylesheets.at("button"),
		{
			std::make_pair("time", "time"),
			std::make_pair("funds", "funds"),
			std::make_pair("population", "population"),
			std::make_pair("employment", "employment"),
			std::make_pair("current tile", "tile"),
		}));
	this->guiSystem.at("infoBar").setPosition(sf::Vector2f(0, this->game->window.getSize().y - 16));
	this->guiSystem.at("infoBar").show();
}