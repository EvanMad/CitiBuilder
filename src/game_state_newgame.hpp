#pragma once
#include "game_state.hpp"

class GameStateNewGame : public GameState
{
private:
    sf::View optionsView;
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    void GenerateWorld(tgui::EditBox::Ptr name, tgui::EditBox::Ptr seedbox, tgui::Slider::Ptr freq, tgui::EditBox::Ptr world_size);

    GameStateNewGame(Game* game);
};