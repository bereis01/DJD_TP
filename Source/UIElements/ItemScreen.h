#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class ItemScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 15.0f;
    const int CHAR_WIDTH = 10.0f;
    const int BUTTON_SIZE = 169;
    const int WORD_OFFSET = 5.0f;
    const Vector2 VERTICAL_OFFSET = Vector2(0, 42.0f);
    Vector2 SCREEN_POS = Vector2(250, 100);
    const Vector2 SCREEN_SIZE = Vector2(220, 360);
    Vector2 DETAIL_SCREEN_POS = Vector2(300, 50);
    const Vector2 DETAIL_SCREEN_SIZE = Vector2(240, 88);

    Vector2 WEAPON_POS = Vector2(30, 10);
    Vector2 ITEM_POS = Vector2(30, 230);
    Vector2 MT_POS = Vector2(20, 15);
    Vector2 HIT_POS = Vector2(150, 15);
    Vector2 CRIT_POS = Vector2(20, 55);
    Vector2 RNG_POS = Vector2(150, 55);
    Vector2 DESC_POS = Vector2(20, 35);

    ItemScreen(Game *game, const std::string &fontName);

    ~ItemScreen();

    void HandleKeyPress(int key) override;

    void Update(float deltaTime) override;

    void SetupDisplay(class Unit *unit);

    void SetMight(const std::string &mt);

    void SetHit(const std::string &hit);

    void SetCrit(const std::string &crit);

    void SetRange(const std::string &rng);

    void SetDescription(const std::string &desc);

    void SetInfoVisibility(bool visible);

private:
    UIText *mInfoMt;
    UIText *mInfoHit;
    UIText *mInfoCrit;
    UIText *mInfoRng;
    UIText *mMt;
    UIText *mHit;
    UIText *mCrit;
    UIText *mRng;
    UIText *mDescription;
    UIImage *mDetais;
    class Unit *mUnit;
    int mNumWeapons;
    int mNumItens;
};
