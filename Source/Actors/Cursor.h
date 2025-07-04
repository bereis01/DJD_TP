#pragma once
#include <string>
#include "Actor.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

class Cursor : public Actor {
public:
    enum class CursorState {
        Free,
        Locked,
    };

    Cursor(Game *game, const std::string &texturePath);

    void OnUpdate(float deltaTime) override;

    void OnHandleKeyPress(int key, bool isPressed) override;

    // Manipulation based on coordinates
    void SetXY(int x, int y) { mPosition = Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE); };
    int GetX() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }

private:
    DrawPolygonComponent *mDrawPolygonComponent;

    CursorState mCursorState = CursorState::Free;

    int mSelectedWeapon;
};
