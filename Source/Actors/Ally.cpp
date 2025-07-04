#include "Ally.h"

Ally::Ally(Game *game, const std::string &spriteSheetPath, const std::string &spriteSheetData, Stats stats)
    : Unit(game, spriteSheetPath, spriteSheetData, stats, false) {
    // Components to draw the AOE when moving/attacking
    mMovementAOE = new DrawAOEComponent(this, Vector2::Zero, stats.mov, 200);
    mMovementAOE->SetColor(Vector3(0, 0.0f, 255.0f));
    mAttackAOE = new DrawAOEComponent(this, Vector2::Zero, 0, 200);
    mAttackAOE->SetColor(Vector3(255.0f, 0.0f, 0.0f));

    // Draws the colored square where the ally is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(0, 255, 0));
    mTypeIndicator->SetAlpha(50);
}

Ally::~Ally() {
    mGame->RemoveAlly(this);
}

void Ally::OnUpdate(float deltaTime) {
    // Updates AOE fields
    // Shows Movement AOE if player is moving the unit
    if (mGame->GetGamePlayState() == Game::GamePlayState::MovingUnit && mGame->GetSelectedUnit() == this)
        mMovementAOE->SetEnabled(true);
    else
        mMovementAOE->SetEnabled(false);
    // Shows Attack AOE if player is attacking with the unit
    if (mGame->GetGamePlayState() == Game::GamePlayState::ChoosingTarget && mGame->GetSelectedUnit() == this)
        mAttackAOE->SetEnabled(true);
    else
        mAttackAOE->SetEnabled(false);
    // Updates the attack range with the weapon stats
    if (GetEquippedWeapon())
        mAttackAOE->SetRadius(GetEquippedWeapon()->range);
}
