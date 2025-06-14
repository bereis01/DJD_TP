#include "Cursor.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"

Cursor::Cursor(Game *game, const std::string &texturePath)
    : Actor(game) {
    // Creates all necessary components
    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
}


void Cursor::OnHandleKeyPress(const int key, const bool isPressed) {
    if (key == SDLK_w)
        if (mPosition.y > 0)
            mPosition.y -= Game::TILE_SIZE;

    if (key == SDLK_s)
        if (mPosition.y < (Game::LEVEL_HEIGHT - 1) * Game::TILE_SIZE)
            mPosition.y += Game::TILE_SIZE;

    if (key == SDLK_a)
        if (mPosition.x > 0)
            mPosition.x -= Game::TILE_SIZE;

    if (key == SDLK_d)
        if (mPosition.x < (Game::LEVEL_WIDTH - 1) * Game::TILE_SIZE)
            mPosition.x += Game::TILE_SIZE;
}
