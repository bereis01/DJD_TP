#include "Ally.h"

Ally::Ally(Game *game, const std::string &unitType, Stats stats)
    : Unit(game, stats, false, unitType) {
    // Components to draw the AOE when moving/attacking
    mMovementAOE = new DrawAOEComponent(this, "Movement", Vector2::Zero, stats.mov, 200);
    mMovementAOE->SetColor(Vector3(0, 0.0f, 255.0f));
    mAttackAOE = new DrawAOEComponent(this, "Attack", Vector2::Zero, 0, 200);
    mAttackAOE->SetColor(Vector3(255.0f, 255.0f, 0.0f));

    // Draws the colored square where the ally is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(0, 255, 0));
    mTypeIndicator->SetAlpha(50);

    // Life bar
    mRedBar = new DrawPolygonComponent(this, Vector2(3, Game::TILE_SIZE), Vector2(Game::TILE_SIZE - 6, 2));
    mRedBar->SetColor(Vector3(255, 0, 0));

    mGreenBar = new DrawPolygonComponent(this, Vector2(3, Game::TILE_SIZE), Vector2(Game::TILE_SIZE - 6, 2), 150);
    mGreenBar->SetColor(Vector3(0, 255, 0));
}

Ally::~Ally() {
    mGame->RemoveAlly(this);
}

void Ally::OnUpdate(float deltaTime) {
    // Calls Unit update
    Unit::OnUpdate(deltaTime);

    // Updates life indicator
    mGreenBar->SetSize(Vector2((float(mStats.currHp) / float(mStats.hp)) * (Game::TILE_SIZE - 6), 2));

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
    // Updates type indicator based on state
    if (mAvailable)
        mTypeIndicator->SetColor(Vector3(0, 255, 0));
    else
        mTypeIndicator->SetColor(Vector3(0, 0, 0));
}
