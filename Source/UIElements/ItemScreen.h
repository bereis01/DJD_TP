#pragma once
#include <string>
#include "../UIElements/UIScreen.h"

class ItemScreen : public UIScreen {
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 20.0f;
    const int CHAR_WIDTH = 10.0f;
    const Vector2 VERTICAL_OFFSET = Vector2(0, 42.0f);
    const Vector2 SCREEN_POS = Vector2(500, 50);
    const Vector2 SCREEN_SIZE = Vector2(241, 280);
    const Vector2 DETAIL_SCREEN_POS = Vector2(300, 50);
    const Vector2 DETAIL_SCREEN_SIZE = Vector2(241, 280);

    const Vector2 WEAPON_POS = Vector2(100, 10) + SCREEN_POS;
    const Vector2 ITEM_POS = Vector2(100, 230) + SCREEN_POS;

    ItemScreen(Game *game, const std::string &fontName);

    ~ItemScreen();

    void HandleKeyPress(int key) override;

    void SetupDisplay(class Unit *unit);

private:
    UIButton *mW1;
    UIButton *mW2;
    UIButton *mW3;
    UIButton *mW4;
    UIButton *mI1;
    UIButton *mI2;
    UIButton *mI3;
    class Unit *mUnit;
    class Weapon *mWeapon;
    int mNumWeapons;
};
