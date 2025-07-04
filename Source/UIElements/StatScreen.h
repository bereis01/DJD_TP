#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class StatScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 15.0f;
    const int WORD_OFFSET = 5.0f;
    const int CHAR_WIDTH = 10.0f;
    const Vector2 SCREEN_POS = Vector2(80, 80);
    const Vector2 SCREEN_SIZE = Vector2(320, 320);
    const Vector2 STATS_VERTICAL_OFFSET = Vector2(0, 40);

    const Vector2 NAME_POS = Vector2(88, 10) + SCREEN_POS;
    const Vector2 HP_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET;
    const Vector2 MOV_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET;
    const Vector2 STR_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 2;
    const Vector2 MAG_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 2;
    const Vector2 SKL_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 3;
    const Vector2 SPD_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 3;
    const Vector2 DEF_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 4;
    const Vector2 RES_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 4;

    const Vector2 WEAPON_POS = Vector2(88, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 5;
    const Vector2 MT_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 6;
    const Vector2 HIT_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 6;
    const Vector2 CRIT_POS = Vector2(20, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 7;
    const Vector2 RNG_POS = Vector2(180, 0) + SCREEN_POS + STATS_VERTICAL_OFFSET * 7;

    StatScreen(Game *game, const std::string &fontName);

    void SetDisplayStats(class Unit *unit);

    void SetName(const std::string &name);

    void SetMove(const std::string &mov);

    void SetHp(const std::string &hp);

    void SetStr(const std::string &str);

    void SetMag(const std::string &mag);

    void SetSkl(const std::string &skl);

    void SetSpd(const std::string &spd);

    void SetDef(const std::string &def);

    void SetRes(const std::string &res);

    void SetWeaponName(const std::string &name);

    void SetMight(const std::string &mt);

    void SetHit(const std::string &hit);

    void SetCrit(const std::string &crit);

    void SetRange(const std::string &rng);

private:
    UIText *mName;
    UIText *mMove;
    UIText *mHp;
    UIText *mStr;
    UIText *mMag;
    UIText *mSkl;
    UIText *mSpd;
    UIText *mDef;
    UIText *mRes;
    UIText *mWpn;
    UIText *mMt;
    UIText *mHit;
    UIText *mCrit;
    UIText *mRng;
};
