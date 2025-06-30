#include "Cursor.h"

#include "Enemy.h"
#include "../Game.h"
#include "Unit.h"
#include "../UIElements/StatScreen.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawPolygonComponent.h"

Cursor::Cursor(Game *game, const std::string &texturePath)
    : Actor(game) {
    // Draws the colored square where the cursor is
    mDrawPolygonComponent = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mDrawPolygonComponent->SetColor(Vector3(255, 0, 0));
    mDrawPolygonComponent->SetAlpha(50);

    // Draws the cursor sprite
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
    // Roaming the map
    if (mGame->GetGamePlayState() == Game::GamePlayState::Map) {
        // Movement
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

        // Enter selection
        if (key == SDLK_RETURN) {
            // Checks to see if there is a unit on the position
            Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());

            // If there is a unit, selects it (to move it)
            if (unit != nullptr && unit->IsAvailable()) {
                mGame->SetGamePlayState(Game::GamePlayState::MovingUnit);
                mGame->SetSelectedUnit(unit);
                unit->SetOldPosition(unit->GetPosition());
            }
        }

        // Space selection
        if (key == SDLK_SPACE) {
            // Checks to see if there is a unit on the position
            Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());

            // If there is a unit, show its stats
            if (unit != nullptr) {
                unit->ShowStats();
                mGame->SetGamePlayState(Game::GamePlayState::ShowingStats);
            }
        }

        // A unit is selected for movement
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::MovingUnit) {
        // Movement
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

        // Enter selection
        if (key == SDLK_RETURN) {
            // Gets the selected unit
            Unit *unit = mGame->GetSelectedUnit();

            // Calculates how many squares to move
            int movX = abs(unit->GetX() - GetX());
            int movY = abs(unit->GetY() - GetY());

            // Checks if distance is under the permitted and if there is no unit there
            if ((movX + movY <= unit->GetMovement() &&
                 mGame->GetUnitByPosition(GetX(), GetY()) == nullptr) ||
                (movX + movY == 0)) {
                // Just moves it (TODO maybe animate later?)
                unit->SetXY(GetX(), GetY());

                // Moves the game to the action selection state
                mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
                mGame->PushUI(mGame->GetActionScreen());
            }
        }

        // Space selection
        if (key == SDLK_SPACE) {
            // Checks to see if there is a unit on the position
            Unit *unit = mGame->GetUnitByPosition(GetX(), GetY());

            // If there is a unit, show its stats
            if (unit != nullptr) {
                unit->ShowStats();
                mGame->SetGamePlayState(Game::GamePlayState::ShowingStats);
            }
        }

        // Goes back to roaming on B
        if (key == SDLK_b) {
            mGame->SetGamePlayState(Game::GamePlayState::Map);
            mGame->SetSelectedUnit(nullptr);
        }

        // Showing unit's stats
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::ShowingStats) {
        // Deselects on B or Space
        if (key == SDLK_b || key == SDLK_SPACE) {
            mGame->PopUI();
            if (mGame->GetSelectedUnit() != nullptr)
                mGame->SetGamePlayState(Game::GamePlayState::MovingUnit);
            else
                mGame->SetGamePlayState(Game::GamePlayState::Map);
        }

        // Choosing enemy target
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingTarget) {
        if (mGame->GetTargetUnitIndex() != -1) {
            if (key == SDLK_w || key == SDLK_d) {
                int next_index = mGame->GetTargetUnitIndex() + 1;
                if (next_index >= mGame->GetEnemiesInRange().size()) {
                    next_index = 0;
                }
                mGame->SetTargetUnitIndex(next_index);
                mPosition = mGame->GetEnemiesInRange()[next_index]->GetPosition();
            }
            if (key == SDLK_s || key == SDLK_a) {
                int next_index = mGame->GetTargetUnitIndex() - 1;
                if (next_index < 0) {
                    next_index = mGame->GetEnemiesInRange().size() - 1;
                }
                mGame->SetTargetUnitIndex(next_index);
                mPosition = mGame->GetEnemiesInRange()[next_index]->GetPosition();
            }
            if (key == SDLK_RETURN) {
                auto unit = mGame->GetSelectedUnit();
                auto enemy = mGame->GetEnemiesInRange()[mGame->GetTargetUnitIndex()];
                mGame->GetAttackScreen()->SetDisplayStats(unit->GetStats(), enemy->GetStats(),
                    unit->GetEquippedWeapon(), enemy->GetEquippedWeapon());
                mGame->PushUI(mGame->GetAttackScreen());
                mGame->SetGamePlayState(Game::GamePlayState::ConfirmingAttack);
            }
        }
        if (key == SDLK_b) {
            mGame->SetTargetUnitIndex(-1);
            mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
            mGame->PushUI(mGame->GetActionScreen());
        }
    } else if (mGame->GetGamePlayState() == Game::GamePlayState::ConfirmingAttack) {
        if (key == SDLK_RETURN) {
            mGame->PopUI();
            mGame->GetSelectedUnit()->Attack(mGame->GetEnemiesInRange()[mGame->GetTargetUnitIndex()]);
            mGame->SetSelectedUnit(nullptr);
            mGame->SetTargetUnitIndex(-1);
            mGame->SetGamePlayState(Game::GamePlayState::Map);
        }
        if (key == SDLK_b) {
            mGame->PopUI();
            mGame->SetGamePlayState(Game::GamePlayState::ChoosingTarget);
        }
    }
}
