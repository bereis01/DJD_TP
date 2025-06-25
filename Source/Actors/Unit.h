#pragma once
#include "Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

class Stats {
public:
    Stats(std::string name = "x", int hp = 0, int str = 0, int mag = 0, int spd = 0, int def = 0, int res = 0);

    std::string name;
    int hp;
    int str;
    int mag;
    int spd;
    int def;
    int res;
};

class Unit : public Actor {
public:
    Unit(Game *game, const std::string &texturePath);

    // Manipulation based on coordinates
    void SetXY(int x, int y) { mPosition = Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE); };
    int GetX() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }

    // Updating functions
    void OnUpdate(float deltaTime) override;

    // Stats functions
    void SetStats(class Stats stats);

    void ShowStats();

private:
    DrawSpriteComponent *mDrawComponent;
    Stats mStats;
};
