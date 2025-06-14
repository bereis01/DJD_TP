#pragma once
#include <string>
#include "Actor.h"

// OBS: Mudei tudo isso aqui só para manter a interface padronizada.
// Mas implementei as funções de get das coordenadas baseadas em tiles para você :)

class Cursor : public Actor {
public:
    Cursor(Game *game, const std::string &texturePath);

    void OnHandleKeyPress(const int key, const bool isPressed) override;

    int GetX() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
};
