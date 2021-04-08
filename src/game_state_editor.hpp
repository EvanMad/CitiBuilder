#ifndef GAME_STATE_EDITOR_HPP
#define GAME_STATE_EDITOR_HPP

#include <SFML/Graphics.hpp>

#include "game_state.hpp"
#include "map.hpp"
#include "city.hpp"
#include <vector>

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
    Tile* currentTile;
    std::vector<Tile> validTiles;
    void SelectTile(int tile);
    void OpenCloseMenu();
public:
    City city;
    tgui::Panel::Ptr panel;
    tgui::ScrollablePanel::Ptr scrollZone;
    //tgui::Panel::Ptr backpanel;
    tgui::ChildWindow::Ptr backpanel;
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();
    GameStateEditor(Game* game);
    std::map< std::string, Gui> submenus;
};

#endif // GAME_STATE_EDITOR_HPP 