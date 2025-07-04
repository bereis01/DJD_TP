#include "Unit.h"
#include "../Game.h"
#include "../UIElements/StatScreen.h"

Stats::Stats(std::string n, int h, int ch, int st, int m, int sk, int sp, int d, int r, int mv) {
    name = n;
    hp = h;
    currHp = ch;
    str = st;
    mag = m;
    skl = sk;
    spd = sp;
    def = d;
    res = r;
    mov = mv;
}

Weapon::Weapon(std::string n, int acc, int mt, int crit, int rng, bool m) {
    name = n;
    hit = acc;
    might = mt;
    criticalChance = crit;
    range = rng;
    magic = m;
}

Unit::Unit(Game *game, const std::string &texturePath, Stats stats, bool isEnemy)
    : Actor(game) {
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
    mDmgTaken = 0;
    mStats = stats;
    mIsEnemy = isEnemy;
    mAvailable = true;
}

void Unit::SetStats(Stats stats) {
    mStats.name = stats.name;
    mStats.hp = stats.hp;
    mStats.currHp = stats.currHp;
    mStats.str = stats.str;
    mStats.mag = stats.mag;
    mStats.skl = stats.skl;
    mStats.spd = stats.spd;
    mStats.def = stats.def;
    mStats.res = stats.res;
    mStats.mov = stats.mov;
}

void Unit::ShowStats() {
    mGame->GetStatScreen()->SetDisplayStats(this);
    mGame->PushUI(mGame->GetStatScreen());
}

void Unit::Attack(class Unit *target, bool isCounter) {
    Stats target_stats = target->GetStats();
    int chance_to_hit = (GetEquippedWeapon()->hit + mStats.skl * 2) - (target_stats.spd * 2);
    int crit_chance = GetEquippedWeapon()->criticalChance + mStats.spd - target_stats.skl;
    crit_chance = std::max(crit_chance, 0);
    int true_hit = (rand() % 101) + (rand() % 101);
    int to_crit = rand() % 101;
    if (chance_to_hit * 2 > true_hit) {
        int damage = 0;
        if (GetEquippedWeapon()->magic) {
            damage = mStats.mag + GetEquippedWeapon()->might - target_stats.res;
        } else {
            damage = mStats.str + GetEquippedWeapon()->might - target_stats.def;
        }
        damage = std::max(damage, 0);
        if (crit_chance > to_crit) {
            damage = damage * 3;
        }
        target->SetCurrentHp(target_stats.currHp - damage);
        if (target_stats.currHp - damage <= 0) {
            if (mGame->GetGamePlayState() == Game::GamePlayState::EnemyTurn && target->IsEnemy()) {
                return;
            }
            target->Die();
            mAvailable = false;
            return;
        }
    }
    if (!isCounter) {
        int unitX = GetX();
        int unitY = GetY();
        int targetX = target->GetX();
        int targetY = target->GetY();
        if (abs(unitX - targetX) + abs(unitY - targetY) <= target->GetEquippedWeapon()->range) {
            target->Attack(this, true);
        }
    }
    mAvailable = false;
}

void Unit::EquipWeapon(int pos) {
    Weapon *target = mWeapons[pos];
    auto it = std::find(mWeapons.begin(), mWeapons.end(), target);

    // Check if found and not already the first element
    if (it != mWeapons.end() && it != mWeapons.begin()) {
        std::swap(*it, mWeapons.front());
    }
    mGame->ShowItems();
    mGame->GetActionScreen()->SetSelectedButtonIndex(0);
}

void Unit::UseItem(const std::string &item) {
    if (mStats.currHp < mStats.hp) {
        if (item == "Healing potion") {
            mStats.currHp = std::min(mStats.currHp + 10, mStats.hp);
        } else if (item == "Healing gem") {
            mStats.currHp = std::min(mStats.currHp + 20, mStats.hp);
        }
        mAvailable = false;
        mGame->GetUIStack().pop_back();
        mGame->SetGamePlayState(Game::GamePlayState::Map);
        mGame->SetSelectedUnit(nullptr);
        mGame->GetActionScreen()->SetSelectedButtonIndex(0);
    }
}

void Unit::Die() {
    SetState(ActorState::Destroy);
}

void Unit::Wait() {
    mOldPosition = mPosition;
    mAvailable = false;
    mGame->GetUIStack().pop_back();
    mGame->SetGamePlayState(Game::GamePlayState::Map);
    mGame->SetSelectedUnit(nullptr);
    mGame->GetActionScreen()->SetSelectedButtonIndex(0);
}
