#include "ItemScreen.h"
#include "../Game.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"

ItemScreen::ItemScreen(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName, true) {
    SCREEN_POS = Vector2(game->GetWindowWidth() - 250, 100);
    DETAIL_SCREEN_POS = Vector2(SCREEN_POS.x - SCREEN_SIZE.x * 1.2, SCREEN_POS.y + SCREEN_SIZE.y / 3);
    WEAPON_POS += SCREEN_POS;
    ITEM_POS += SCREEN_POS;
    MT_POS += DETAIL_SCREEN_POS;
    HIT_POS += DETAIL_SCREEN_POS;
    CRIT_POS += DETAIL_SCREEN_POS;
    RNG_POS += DETAIL_SCREEN_POS;
    DESC_POS += DETAIL_SCREEN_POS;
    AddButton("Weapon1", WEAPON_POS, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Weapon2", WEAPON_POS + VERTICAL_OFFSET, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(5 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Weapon3", WEAPON_POS + VERTICAL_OFFSET * 2, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Weapon4", WEAPON_POS + VERTICAL_OFFSET * 3, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));

    AddButton("Item1", ITEM_POS, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Item2", ITEM_POS + VERTICAL_OFFSET, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));
    AddButton("Item3", ITEM_POS + VERTICAL_OFFSET * 2, Vector2(BUTTON_SIZE, 30), nullptr,
              Vector2(4 * CHAR_WIDTH, WORD_HEIGHT));

    AddText("Might:", MT_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mMt = AddText("5", MT_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                  POINT_SIZE);

    AddText("Hit:", HIT_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mHit = AddText("5", HIT_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Critical:", CRIT_POS, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);
    mCrit = AddText("5", CRIT_POS + Vector2(CHAR_WIDTH * 9 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                    POINT_SIZE);

    AddText("Range:", RNG_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mRng = AddText("5", RNG_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    mDescription = AddText("Item description", DESC_POS, Vector2(CHAR_WIDTH * 16, WORD_HEIGHT),
                           POINT_SIZE);

    AddImage("../Assets/UI/WoodBackground.png", SCREEN_POS, SCREEN_SIZE);
    AddImage("../Assets/UI/PlainBackground.png", DETAIL_SCREEN_POS, DETAIL_SCREEN_SIZE);
    mUnit = nullptr;
    mNumWeapons = 0;
    mNumItens = 0;
}

void ItemScreen::HandleKeyPress(int key) {
    if (!mInteractive)
        return;
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = mButtons.size() - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if (mSelectedButtonIndex > mButtons.size() - 1) {
            mSelectedButtonIndex = 0;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);
    } else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex <= static_cast<int>(mButtons.size()) - 1) {
            mButtons[mSelectedButtonIndex]->OnClick();
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
    auto itens = unit->GetAllItens();
    mNumWeapons = weapons.size();
    mNumItens = itens.size();
    mUnit = unit;
    for (int i = 0; i < mNumWeapons; i++) {
        mButtons[i]->SetText(weapons[i]->name);
        mButtons[i]->SetTextSize(Vector2((weapons[i]->name).size() * CHAR_WIDTH, WORD_HEIGHT));
        mButtons[i]->SetFunction([this, i]() { mUnit->EquipWeapon(i); });
    }
    for (int i = mNumWeapons; i < 4; i++) {
        mButtons[i]->SetText(" ");
        mButtons[i]->SetFunction(nullptr);
    }

    for (int i = 4; i < 4 + mNumItens; i++) {
        mButtons[i]->SetText(itens[i - 4]);
        mButtons[i]->SetTextSize(Vector2(itens[i - 4].size() * CHAR_WIDTH, WORD_HEIGHT));
        mButtons[i]->SetFunction([this, i, itens]() { mUnit->UseItem(itens[i - 4]); });
    }
    for (int i = 4 + mNumItens; i < 7; i++) {
        mButtons[i]->SetText(" ");
        mButtons[i]->SetFunction(nullptr);
    }
}

void ItemScreen::SetMight(const std::string &mt) {
    mMt->SetText(mt);
    int len = mt.length() * CHAR_WIDTH;
    mMt->SetSize(Vector2(len, WORD_HEIGHT));
}

void ItemScreen::SetHit(const std::string &hit) {
    mHit->SetText(hit);
    int len = hit.length() * CHAR_WIDTH;
    mHit->SetSize(Vector2(len, WORD_HEIGHT));
}

void ItemScreen::SetCrit(const std::string &crit) {
    mCrit->SetText(crit);
    int len = crit.length() * CHAR_WIDTH;
    mCrit->SetSize(Vector2(len, WORD_HEIGHT));
}

void ItemScreen::SetRange(const std::string &rng) {
    mRng->SetText(rng);
    int len = rng.length() * CHAR_WIDTH;
    mRng->SetSize(Vector2(len, WORD_HEIGHT));
}
