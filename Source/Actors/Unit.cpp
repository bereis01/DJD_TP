#include "Unit.h"
#include "../Game.h"
#include "../UIElements/StatScreen.h"

Stats::Stats(std::string n, int h, int st, int m, int sk, int sp, int d, int r) {
    name = n;
    hp = h;
    str = st;
    mag = m;
    skl = sk;
    spd = sp;
    def = d;
    res = r;
}

Weapon::Weapon(std::string n, int acc, int mt, int crit, int rng, bool m) {
    name = n;
    hit = acc;
    might = mt;
    criticalChance = crit;
    range = rng;
    magic = m;
}

Unit::Unit(Game *game, const std::string &texturePath, int mov)
    : Actor(game) {
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
    mDmgTaken = 0;
    mMovement = mov;
    mAvailable = true;
    mEquippedWeapon = nullptr;

    // Components to draw the AOE when moving/attacking
    mMovementAOE = new DrawAOEComponent(this, Vector2::Zero, mov, 200);
    mMovementAOE->SetColor(Vector3(0, 0.0f, 255.0f));
    mAttackAOE = new DrawAOEComponent(this, Vector2::Zero, 0, 200);
    mAttackAOE->SetColor(Vector3(255.0f, 0.0f, 0.0f));

    // Draws the colored square where the unit is
    mTypeIndicator = new DrawPolygonComponent(this, Vector2::Zero, Vector2(Game::TILE_SIZE, Game::TILE_SIZE));
    mTypeIndicator->SetColor(Vector3(0, 255, 0));
    mTypeIndicator->SetAlpha(50);
}

void Unit::OnUpdate(float deltaTime) {
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
    if (mEquippedWeapon)
        mAttackAOE->SetRadius(mEquippedWeapon->range);
}

void Unit::SetStats(Stats stats) {
    mStats.name = stats.name;
    mStats.hp = stats.hp;
    mStats.str = stats.str;
    mStats.mag = stats.mag;
    mStats.skl = stats.skl;
    mStats.spd = stats.spd;
    mStats.def = stats.def;
    mStats.res = stats.res;
}

void Unit::ShowStats() {
    mGame->GetStatScreen()->SetDisplayStats(mStats, mDmgTaken);
    mGame->PushUI(reinterpret_cast<struct UIScreen *>(mGame->GetStatScreen()));
}

void Unit::Attack(class Unit *target) {
    Stats target_stats = target->GetStats();
    int chance_to_hit = (mEquippedWeapon->hit + mStats.skl * 2) - (target_stats.spd * 2);
    int true_hit = (rand() % 101) + (rand() % 101);
    if (chance_to_hit * 2 > true_hit) {
        int damage = 0;
        if (mEquippedWeapon->magic) {
            damage = mStats.mag + mEquippedWeapon->might - target_stats.res;
        } else {
            damage = mStats.str + mEquippedWeapon->might - target_stats.def;
        }
        target->TakeDamage(std::max(damage, 0));
    }
}

void Unit::TakeDamage(int damage) {
    mDmgTaken += damage;
    if (mDmgTaken >= mStats.hp) {
        Die();
    }
}

void Unit::Die() {
    //SetState(ActorState::Destroy);
}

void Unit::Wait() {
    mOldPosition = mPosition;
    mAvailable = false;
    mGame->GetUIStack().pop_back();
    mGame->SetGamePlayState(Game::GamePlayState::Map);
    mGame->SetSelectedUnit(nullptr);
    mGame->GetActionScreen()->SetSelectedButtonIndex(0);
}
