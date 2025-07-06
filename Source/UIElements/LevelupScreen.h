#pragma once
#include <string>
#include <unordered_set>
#include "../UIElements/UIScreen.h"

class LevelupScreen : public UIScreen {
public:
    const int POINT_SIZE = 56;
    const int WORD_HEIGHT = 20.0f;
    const int CHAR_WIDTH = 10.0f;
    const int WORD_OFFSET = 5.0f;

    const Vector2 FIRST_BUTTON_POS = Vector2(88, 188);
    Vector2 HORIZONTAL_OFFSET = Vector2(42, 0);
    Vector2 VERTICAL_OFFSET = Vector2(0, 42);
    const Vector2 SCREEN_POS = Vector2(0, 0);
    Vector2 SCREEN_SIZE = Vector2(220, 360);
    Vector2 BUTTON_SIZE = Vector2(100, 40);

    const Vector2 FIRST_UNIT_ICON_POS = FIRST_BUTTON_POS + Vector2(0, 50);
    const Vector2 UNIT_ICON_SIZE = Vector2(100, 100);

    Vector2 DETAIL_SCREEN_POS = Vector2(400, 300);
    const Vector2 DETAIL_SCREEN_SIZE = Vector2(250, 140);

    const Vector2 MIA_POS = Vector2(0, 0);
    const Vector2 MARCIA_POS = Vector2(0, 0);
    const Vector2 HUBERT_POS = Vector2(0, 0);
    const Vector2 FERDINAND_POS = Vector2(0, 0);

    const Vector2 STR_POS = FIRST_BUTTON_POS + Vector2(0, 70 + UNIT_ICON_SIZE.y);
    const Vector2 MAG_POS = STR_POS + VERTICAL_OFFSET;
    const Vector2 SKL_POS = STR_POS + VERTICAL_OFFSET * 2;
    const Vector2 SPD_POS = STR_POS + VERTICAL_OFFSET * 3;
    const Vector2 DEF_POS = STR_POS + VERTICAL_OFFSET * 4;
    const Vector2 RES_POS = STR_POS + VERTICAL_OFFSET * 5;

    Vector2 NAME_POS = Vector2(69, 10) + DETAIL_SCREEN_POS;
    Vector2 MT_POS = Vector2(20, 55) + DETAIL_SCREEN_POS;
    Vector2 HIT_POS = Vector2(150, 55) + DETAIL_SCREEN_POS;
    Vector2 CRIT_POS = Vector2(20, 95) + DETAIL_SCREEN_POS;
    Vector2 RNG_POS = Vector2(150, 95) + DETAIL_SCREEN_POS;
    Vector2 COST_POS = Vector2(42, 150) + DETAIL_SCREEN_POS;

    LevelupScreen(Game *game, const std::string &fontName, int level = 2);

    ~LevelupScreen();

    void HandleKeyPress(int key) override;

    void Update(float deltaTime) override;

    void SetStr(const std::string &str, int index);
    void SetMag(const std::string &mag, int index);
    void SetSkl(const std::string &skl, int index);
    void SetSpd(const std::string &spd, int index);
    void SetDef(const std::string &def, int index);
    void SetRes(const std::string &res, int index);

    void Upgrade();

private:
    UIText *mInfoText;
    std::vector<UIText *> mStr;
    std::vector<UIText *> mMag;
    std::vector<UIText *> mSkl;
    std::vector<UIText *> mSpd;
    std::vector<UIText *> mDef;
    std::vector<UIText *> mRes;
    int mCurrentUpgrade;
    bool mSecondChoice;



    UIText *mCurrentGold;
    UIText *mWriteUnit;
    UIText *mInfoMt;
    UIText *mInfoHit;
    UIText *mInfoCrit;
    UIText *mInfoRng;
    UIText *mName;
    UIText *mMt;
    UIText *mHit;
    UIText *mCrit;
    UIText *mRng;
    UIText *mCost;
    UIImage *mDetais;
    UIImage *mUnitIcon;
    std::vector<class Weapon *> mWeapons;
    std::unordered_set<int> mBought;
    int mGold;
};
