#include "Unit.h"
#include "../Game.h"

Unit::Unit(Game *game, const std::string &texturePath)
    : Actor(game) {
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
}

void Unit::OnUpdate(float deltaTime) {
}
