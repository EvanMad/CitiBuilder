#ifndef GAME_STATE_EDITOR_HPP
#define GAME_STATE_EDITOR_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "map.hpp"
#include "city.hpp"

enum class ActionState{NONE, PANNING, SELECTING, BRUSH};
enum class ClickAction{SELECT, BRUSH};

class GameStateEditor : public GameState
{
private:
    ActionState actionState;
    ClickAction clickActionState;
    sf::View gameView;
    sf::View guiView;
    //Map map;
    sf::Vector2i panningAnchor;
    float zoomLevel;
    std::map<std::string, Gui> guiSystem;

    sf::Vector2i selectionStart;
    sf::Vector2i selectionEnd;
    void closeAllSubMenus();
    Tile* currentTile;
public:
    City city;
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
    GameStateEditor(Game* game);
    std::map< std::string, Gui> submenus;
};

#endif // GAME_STATE_EDITOR_HPP 