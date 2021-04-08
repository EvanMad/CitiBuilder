#pragma once
#include "game_state.hpp"

class GameStateOptions : public GameState
{
private:
    sf::View optionsView;
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    GameStateOptions(Game* game);
};