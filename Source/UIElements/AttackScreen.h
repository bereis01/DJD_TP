#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class AttackScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int CHAR_WIDTH = 10.0f;
    const int STATS_OFFSET = 88.0f;
    const Vector2 SCREEN_POS = Vector2(500, 50);
    const Vector2 SCREEN_SIZE = Vector2(241, 280);

    const Vector2 NAME_POS = Vector2(100, 10);
    const Vector2 WEAPON_POS = Vector2(100, 42);
    const Vector2 ENEMY_NAME_POS= Vector2(100, 230);
    const Vector2 ENEMY_WEAPON_POS = Vector2(100, 252);

    const Vector2 HP_POS = Vector2(110, 80);
    const Vector2 DMG_POS = Vector2(110, 118);
    const Vector2 HIT_POS = Vector2(110, 156);
    const Vector2 CRIT_POS = Vector2(110, 190);

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
