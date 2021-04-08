#pragma once
#include "game_state.hpp"

class GameStateLoadGame : public GameState
{
private:
    sf::View optionsView;
public:
    virtual void draw(const float dt);
    virtual void update(const float dt);
    virtual void handleInput();

    void LoadGame(std::string world);

    GameStateLoadGame(Game* game);
};