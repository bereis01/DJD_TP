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


void Cursor::OnHandleKeyPress(const int key, const bool isPressed)
{
    if (mGame->GetGamePlayState() == Game::GamePlayState::Map) {
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
            if (unit == nullptr)
                return;
            //mState = CursorState::Locked;
            mGame->SetGamePlayState(Game::GamePlayState::UnitSelected);
            mGame->SetSelectedUnit(unit);
            unit->SetOldPosition(unit->GetPosition());
        }

        if (key == SDLK_SPACE) {
            class Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());
            if (unit == nullptr)
                return;
            unit->ShowStats();
            mGame->SetGamePlayState(Game::GamePlayState::ShowingStats);
        }
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::UnitSelected) {
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
            Unit *unit = mGame->GetSelectedUnit();
            int movX = abs(unit->GetX() - GetX());
            int movY = abs(unit->GetY() - GetY());
            if ((movX + movY <= unit->GetMovement() &&
                mGame->GetUnitByPosition(GetX(), GetY()) == nullptr) ||
                (movX + movY == 0)) {
                unit->SetXY(GetX(), GetY());
                mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
                mGame->PushUI(mGame->GetActionScreen());
            }
        }
        if (key == SDLK_SPACE) {
            class Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());
            if (unit == nullptr)
                return;
            unit->ShowStats();
            mGame->SetGamePlayState(Game::GamePlayState::ShowingStats);
        }
        if (key == SDLK_b) {
            mGame->SetGamePlayState(Game::GamePlayState::Map);
            mGame->SetSelectedUnit(nullptr);
        }
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::ShowingStats) {
        if (key == SDLK_b || key == SDLK_SPACE) {
            if (mGame->GetSelectedUnit() != nullptr) {
                mGame->SetGamePlayState(Game::GamePlayState::UnitSelected);
                mGame->GetUIStack().pop_back();
            } else {
                mGame->SetGamePlayState(Game::GamePlayState::Map);
                mGame->GetUIStack().pop_back();
            }
        }
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingTarget) {
        if (mGame->GetTargetUnitIndex() != -1) {
            if (key == SDLK_w || key == SDLK_d) {
                int next_index = mGame->GetTargetUnitIndex() + 1;
                if (next_index <= mGame->GetUnitsInRange().size()) {
                    next_index = 0;
                }
                mGame->SetTargetUnitIndex(next_index);
                mPosition = mGame->GetUnitsInRange()[next_index]->GetPosition();
            }
            if (key == SDLK_s || key == SDLK_a) {
                int next_index = mGame->GetTargetUnitIndex() - 1;
                if (next_index < 0) {
                    next_index = mGame->GetUnitsInRange().size() - 1;
                }
                mGame->SetTargetUnitIndex(next_index);
                mPosition = mGame->GetUnitsInRange()[next_index]->GetPosition();
            }
            if (key == SDLK_RETURN) {
                mGame->GetSelectedUnit()->Attack(mGame->GetUnitsInRange()[mGame->GetTargetUnitIndex()]);
                mGame->SetSelectedUnit(nullptr);
                mGame->SetTargetUnitIndex(-1);
                mGame->GetUnitsInRange().clear();
                mGame->SetGamePlayState(Game::GamePlayState::Map);
            }
        }
        if (key == SDLK_b) {
            mGame->SetTargetUnitIndex(-1);
            mGame->GetUnitsInRange().clear();
            mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
            mGame->PushUI(mGame->GetActionScreen());
        }
    }
}
