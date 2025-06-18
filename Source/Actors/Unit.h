#pragma once
#include "Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

class Unit : public Actor {
public:
    Unit(Game *game, const std::string &texturePath);

    // Manipulation based on coordinates
    void SetXY(int x, int y) { mPosition = Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE); };
    int GetX() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }

    void OnUpdate(float deltaTime) override;

private:
    DrawSpriteComponent *mDrawComponent;
};
