#include "AttackScreen.h"
#include "../Game.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"

AttackScreen::AttackScreen(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName, false) {
    AddText("HP", HP_POS, Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);
    AddText("Dmg", DMG_POS, Vector2(CHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE);
    AddText("Hit", HIT_POS, Vector2(CHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE);
    AddText("Crit", CRIT_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);

    mName = AddText("Jill", NAME_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mEnName = AddText("Fighter", ENEMY_NAME_POS, Vector2(CHAR_WIDTH * 7, WORD_HEIGHT), POINT_SIZE);
    mWeapon = AddText("Silver axe", WEAPON_POS, Vector2(CHAR_WIDTH * 10, WORD_HEIGHT), POINT_SIZE);
    mEnWeapon = AddText("Steel axe", ENEMY_WEAPON_POS, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);

    mHp = AddText("25", HP_POS + STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);
    mEnHp = AddText("25", HP_POS - STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);
    mDmg = AddText("12", DMG_POS + STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);
    mEnDmg = AddText("6", DMG_POS - STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 1, WORD_HEIGHT), POINT_SIZE);
    mHit = AddText("100", HIT_POS + STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE);
    mEnHit = AddText("90", HIT_POS - STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);
    mCrit = AddText("5", CRIT_POS + STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 1, WORD_HEIGHT), POINT_SIZE);
    mEnCrit = AddText("0", CRIT_POS - STATS_HORIZONTAL_OFFSET, Vector2(CHAR_WIDTH * 1, WORD_HEIGHT), POINT_SIZE);

    AddImage("../Assets/UI/AttackScreenBackground.png", SCREEN_POS, SCREEN_SIZE);

    // Controls
    std::string controlsText = "Press [A, D] to change weapon";
    AddText(controlsText,
            Vector2((SCREEN_POS.x + (SCREEN_SIZE.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (SCREEN_POS.y + SCREEN_SIZE.y + 5)), Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE, 2048);

    controlsText = "Press [ENTER] to confirm";
    AddText(controlsText,
            Vector2((SCREEN_POS.x + (SCREEN_SIZE.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (SCREEN_POS.y + SCREEN_SIZE.y + 5 + 1.5 * WORD_HEIGHT)),
            Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE);

    controlsText = "Press [B] to return";
    AddText(controlsText,
            Vector2((SCREEN_POS.x + (SCREEN_SIZE.x - (controlsText.size() * CHAR_WIDTH)) / 2),
                    (SCREEN_POS.y + SCREEN_SIZE.y + 5 + 3 * WORD_HEIGHT)),
            Vector2(CHAR_WIDTH * controlsText.size(), WORD_HEIGHT),
            POINT_SIZE);
}

void AttackScreen::SetDisplayStats(class Stats unitStats, class Stats enemyStats, class Weapon *unitWeapon,
                                   class Weapon *enemyWeapon, int range) {
    // Calculations for unit
    int damage = 0;
    if (unitWeapon->magic) {
        damage = unitStats.mag + unitWeapon->might - enemyStats.res;
    } else {
        damage = unitStats.str + unitWeapon->might - enemyStats.def;
    }
    damage = std::max(damage, 0);
    int chance_to_hit = (unitWeapon->hit + unitStats.skl * 2) - (enemyStats.spd * 2);
    chance_to_hit = std::max(chance_to_hit, 0);
    chance_to_hit = std::min(chance_to_hit, 100);
    int crit_chance = unitWeapon->criticalChance + unitStats.spd - enemyStats.skl;
    crit_chance = std::max(crit_chance, 0);
    crit_chance = std::min(crit_chance, 100);

    // Set display
    SetName(unitStats.name);
    SetWeapon(unitWeapon->name);
    SetHp(std::to_string(unitStats.currHp));
    SetDmg(std::to_string(damage));
    SetHit(std::to_string(chance_to_hit));
    SetCrit(std::to_string(crit_chance));

    // Calculations for enemy
    damage = 0;
    if (enemyWeapon->magic) {
        damage = enemyStats.mag + enemyWeapon->might - unitStats.res;
    } else {
        damage = enemyStats.str + enemyWeapon->might - unitStats.def;
    }
    damage = std::max(damage, 0);
    chance_to_hit = (enemyWeapon->hit + enemyStats.skl * 2) - (unitStats.spd * 2);
    chance_to_hit = std::max(chance_to_hit, 0);
    chance_to_hit = std::min(chance_to_hit, 100);
    crit_chance = enemyWeapon->criticalChance + enemyStats.spd - unitStats.skl;
    crit_chance = std::max(crit_chance, 0);
    crit_chance = std::min(crit_chance, 100);

    // Set display
    SetEnemyName(enemyStats.name);
    SetEnemyWeapon(enemyWeapon->name);
    SetEnemyHp(std::to_string(enemyStats.currHp));
    if (enemyWeapon->range < range) {
        SetEnemyDmg("--");
        SetEnemyHit("--");
        SetEnemyCrit("--");
    } else {
        SetEnemyDmg(std::to_string(damage));
        SetEnemyHit(std::to_string(chance_to_hit));
        SetEnemyCrit(std::to_string(crit_chance));
    }
}

void AttackScreen::SetName(const std::string &name) {
    mName->SetText(name);
    int len = name.length() * CHAR_WIDTH;
    mName->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, NAME_POS.y));
    mName->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyName(const std::string &name) {
    mEnName->SetText(name);
    int len = name.length() * CHAR_WIDTH;
    mEnName->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, ENEMY_NAME_POS.y));
    mEnName->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetWeapon(const std::string &wpn) {
    mWeapon->SetText(wpn);
    int len = wpn.length() * CHAR_WIDTH;
    mWeapon->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, WEAPON_POS.y));
    mWeapon->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyWeapon(const std::string &wpn) {
    mEnWeapon->SetText(wpn);
    int len = wpn.length() * CHAR_WIDTH;
    mEnWeapon->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, ENEMY_WEAPON_POS.y));
    mEnWeapon->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetHp(const std::string &hp) {
    mHp->SetText(hp);
    int len = hp.length() * CHAR_WIDTH;
    mHp->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyHp(const std::string &hp) {
    mEnHp->SetText(hp);
    int len = hp.length() * CHAR_WIDTH;
    mEnHp->SetSize(Vector2(len, WORD_HEIGHT));
}


void AttackScreen::SetDmg(const std::string &dmg) {
    mDmg->SetText(dmg);
    int len = dmg.length() * CHAR_WIDTH;
    mDmg->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyDmg(const std::string &dmg) {
    mEnDmg->SetText(dmg);
    int len = dmg.length() * CHAR_WIDTH;
    mEnDmg->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetHit(const std::string &hit) {
    mHit->SetText(hit);
    int len = hit.length() * CHAR_WIDTH;
    mHit->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyHit(const std::string &hit) {
    mEnHit->SetText(hit);
    int len = hit.length() * CHAR_WIDTH;
    mEnHit->SetSize(Vector2(len, WORD_HEIGHT));
}


void AttackScreen::SetCrit(const std::string &crit) {
    mCrit->SetText(crit);
    int len = crit.length() * CHAR_WIDTH;
    mCrit->SetSize(Vector2(len, WORD_HEIGHT));
}

void AttackScreen::SetEnemyCrit(const std::string &crit) {
    mEnCrit->SetText(crit);
    int len = crit.length() * CHAR_WIDTH;
    mEnCrit->SetSize(Vector2(len, WORD_HEIGHT));
}
