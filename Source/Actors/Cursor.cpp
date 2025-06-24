#include "Cursor.h"
#include "../Game.h"
#include "Unit.h"
#include "../StatScreen.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/ColliderComponents/AABBColliderComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

Cursor::Cursor(Game *game, const std::string &texturePath)
    : Actor(game) {
    // Creates all necessary components
    mDrawPolygonComponent = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mDrawPolygonComponent->SetColor(Vector3(255, 0, 0));
    mDrawPolygonComponent->SetAlpha(100);

    new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
}

void Cursor::OnUpdate(float deltaTime) {
    // Sets the cursor indicator to the appropriate color according to tile selected
    if (mState == CursorState::Free) {
        if (mGame->GetLevelData(GetX(), GetY()) == 0)
            mDrawPolygonComponent->SetColor(Vector3(255, 0, 0));
        else
            mDrawPolygonComponent->SetColor(Vector3(0, 255, 0));
    } else if (mState == CursorState::Locked)
        mDrawPolygonComponent->SetColor(Vector3(255, 255, 0));
}


void Cursor::OnHandleKeyPress(const int key, const bool isPressed) {
    if (mState == CursorState::Free) {
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

        if (key == SDLK_RETURN) {
            class Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());
            if (unit == nullptr) {
                return;
            } else {
                mState = CursorState::Locked;
                unit->ShowStats();
            }
        }
    } else if (mState == CursorState::Locked) {
        if (key == SDLK_b) {
            if (!mGame->GetUIStack().empty()) {
                mGame->GetUIStack().pop_back();
                if (mGame->GetUIStack().empty()) {
                    mState = CursorState::Free;
                }
            }
        }
    }
}
