#pragma once
#include <string>
#include <unordered_set>
#include "../UIElements/UIScreen.h"

class ShopScreen : public UIScreen {
public:
    const int POINT_SIZE = 56;
    const int WORD_HEIGHT = 15.0f;
    const int CHAR_WIDTH = 10.0f;
    const int BUTTON_SIZE = 130;
    const int WORD_OFFSET = 5.0f;

    const Vector2 FIRST_BUTTON_POS = Vector2(69, 142);
    const Vector2 VERTICAL_OFFSET = Vector2(0, 41.0f);
    const Vector2 SCREEN_POS = Vector2(0, 0);
    Vector2 SCREEN_SIZE = Vector2(220, 360);
    const Vector2 UNIT_ICON_POS = Vector2(540, 188);
    const Vector2 UNIT_ICON_SIZE = Vector2(88, 88);

    Vector2 DETAIL_SCREEN_POS = Vector2(440, 300);
    const Vector2 DETAIL_SCREEN_SIZE = Vector2(250, 140);

    Vector2 NAME_POS = Vector2(69, 10) + DETAIL_SCREEN_POS;
    Vector2 MT_POS = Vector2(20, 55) + DETAIL_SCREEN_POS;
    Vector2 HIT_POS = Vector2(150, 55) + DETAIL_SCREEN_POS;
    Vector2 CRIT_POS = Vector2(20, 95) + DETAIL_SCREEN_POS;
    Vector2 RNG_POS = Vector2(150, 95) + DETAIL_SCREEN_POS;
    Vector2 COST_POS = Vector2(-40, 150) + DETAIL_SCREEN_POS;

    ShopScreen(Game *game, const std::string &fontName, int level = 2);

    ~ShopScreen();

    void HandleKeyPress(int key) override;

    void Update(float deltaTime) override;

    void SetName(const std::string &name);

    void SetMight(const std::string &mt);

    void SetHit(const std::string &hit);

    void SetCrit(const std::string &crit);

    void SetRange(const std::string &rng);

    void SetIcon(int index);

    void SetCost(int index);

    void SetInfoVisibility(bool visible);

    int GetCost(int index);

    void BuyWeapon(int index);

    void GoToExp();

private:
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
