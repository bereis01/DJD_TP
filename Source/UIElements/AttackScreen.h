#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class AttackScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int CHAR_WIDTH = 10.0f;
    const Vector2 STATS_VERTICAL_OFFSET = Vector2(0, 38.0f);
    const Vector2 STATS_HORIZONTAL_OFFSET = Vector2(88.0f, 0);
    const Vector2 SCREEN_POS = Vector2(500, 50);
    const Vector2 SCREEN_SIZE = Vector2(241, 280);

    const Vector2 NAME_POS = Vector2(100, 10) + SCREEN_POS;
    const Vector2 WEAPON_POS = Vector2(100, 42) + SCREEN_POS;
    const Vector2 ENEMY_NAME_POS= Vector2(100, 220) + SCREEN_POS;
    const Vector2 ENEMY_WEAPON_POS = Vector2(100, 252) + SCREEN_POS;

    const Vector2 HP_POS = Vector2(110, 75) + SCREEN_POS;
    const Vector2 DMG_POS = Vector2(110, 75) + SCREEN_POS + STATS_VERTICAL_OFFSET;
    const Vector2 HIT_POS = Vector2(110, 75) + SCREEN_POS + STATS_VERTICAL_OFFSET * 2;
    const Vector2 CRIT_POS = Vector2(110, 75) + SCREEN_POS + STATS_VERTICAL_OFFSET * 3;

    AttackScreen(Game *game, const std::string &fontName);

    ~AttackScreen();

    void SetDisplayStats(class Stats unitStats, class Stats enemyStats, class Weapon *unitWeapon, class Weapon *enemyWeapon, int range);

    void SetName(const std::string &name);
    void SetEnemyName(const std::string &name);
    void SetWeapon(const std::string &wpn);
    void SetEnemyWeapon(const std::string &wpn);

    void SetHp(const std::string &hp);
    void SetEnemyHp(const std::string &hp);

    void SetDmg(const std::string &dmg);
    void SetEnemyDmg(const std::string &dmg);

    void SetHit(const std::string &hit);
    void SetEnemyHit(const std::string &hit);

    void SetCrit(const std::string &crit);
    void SetEnemyCrit(const std::string &crit);

private:
    UIText *mName;
    UIText *mEnName;
    UIText *mWeapon;
    UIText *mEnWeapon;
    UIText *mHp;
    UIText *mEnHp;
    UIText *mDmg;
    UIText *mEnDmg;
    UIText *mHit;
    UIText *mEnHit;
    UIText *mCrit;
    UIText *mEnCrit;
};
