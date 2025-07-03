#include "ItemScreen.h"
#include "../Game.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"

ItemScreen::ItemScreen(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName, true) {

    mW1 = AddButton("Weapon1", WEAPON_POS, Vector2(100, 30), nullptr,
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    mW2 = AddButton("Weapon2", WEAPON_POS + VERTICAL_OFFSET, Vector2(150, 30), nullptr,
              Vector2(5 * CHAR_WIDTH, WORD_HEIGHT));
    mW3 = AddButton("Weapon3", WEAPON_POS + VERTICAL_OFFSET * 2, Vector2(150, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    mW2 = AddButton("Weapon4", WEAPON_POS + VERTICAL_OFFSET * 3, Vector2(150, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));

    mI1 = AddButton("Item1", ITEM_POS, Vector2(100, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    mI2 =  AddButton("Item2", ITEM_POS + VERTICAL_OFFSET, Vector2(100, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    mI3= AddButton("Item3", ITEM_POS + VERTICAL_OFFSET * 2, Vector2(100, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));

    AddImage("../Assets/UI/WoodBackground.png", SCREEN_POS, SCREEN_SIZE);
    mUnit = nullptr;
    mNumWeapons = 0;
}

ItemScreen::~ItemScreen() {
}

void ItemScreen::HandleKeyPress(int key) {
    if (!mInteractive)
        return;
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = std::max(mNumWeapons - 1, 0);
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if (mSelectedButtonIndex > std::max(mNumWeapons - 1, 0)) {
            mSelectedButtonIndex = 0;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex <= static_cast<int>(mButtons.size()) - 1) {
            mButtons[mSelectedButtonIndex]->OnClick();
            mGame->PopUI();
            mGame->PushUI(mGame->GetActionScreen());
            SetSelectedButtonIndex(0);
            mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
        }
    } else if (key == SDLK_b) {
        mGame->PopUI();
        if (mGame->GetGamePlayState() == Game::GamePlayState::OnInventory) {
            mGame->SetGamePlayState(Game::GamePlayState::ChoosingAction);
            SetSelectedButtonIndex(0);
            mGame->PushUI(mGame->GetActionScreen());
        }
    }
}

void ItemScreen::SetupDisplay(class Unit *unit) {
    auto weapons = unit->GetAllWeapons();
    mNumWeapons = weapons.size();
    mUnit = unit;
    for (int i = 0; i < mNumWeapons; i++) {
        mButtons[i]->SetText(weapons[i]->name);
        mWeapon = weapons[i];
        mButtons[i]->SetFunction([this]() { mUnit->EquipWeapon(mWeapon); });
    }
    for (int i = mNumWeapons; i < 4; i++) {
        mButtons[i]->SetText("");
    }
}