#include "ShopScreen.h"
#include "../Game.h"
#include "../Actors/Ally.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"
#include "../Audio/AudioSystem.h"

ShopScreen::ShopScreen(class Game *game, const std::string &fontName, int level)
    : UIScreen(game, fontName, true) {
    mGold = 8;
    if (level == 2) {
        Weapon *w = new Weapon("Steel sword", 85, 9, 0, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Wo dao", 90, 6, 30, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Falchion", 100, 13, 15, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Steel lance", 80, 10, 0, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Silver lance", 90, 13, 0, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Wishblade", 85, 15, 0, 2);
        mWeapons.emplace_back(w);
        w = new Weapon("Steel axe", 75, 11, 0, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Hand axe", 75, 6, 0, 2);
        mWeapons.emplace_back(w);
        w = new Weapon("Labarunda", 80, 18, 0, 1);
        mWeapons.emplace_back(w);
        w = new Weapon("Thoron", 80, 7, 10, 3, true);
        mWeapons.emplace_back(w);
        w = new Weapon("Meteor", 75, 2, 0, 8, true);
        mWeapons.emplace_back(w);
    }
    SCREEN_SIZE = Vector2(game->GetWindowWidth(), game->GetWindowHeight());
    AddImage("../Assets/UI/ShopBackground.png", SCREEN_POS, SCREEN_SIZE);
    AddText("Buy Weapons", Vector2(SCREEN_SIZE.x / 2 - 125, 30), Vector2(250, 69));
    mCurrentGold = AddText("Current gold: 8", Vector2(400, 130), Vector2(250, 30));
    mWriteUnit = AddText("Unit: ", UNIT_ICON_POS - Vector2(140, 0), Vector2(130, 30));
    //AddText("Unit: ", UNIT_ICON_POS - Vector2(140, 0), Vector2(130, 30));
    mUnitIcon = AddImage("../Assets/Sprites/Units/TrueBlade.png", UNIT_ICON_POS, UNIT_ICON_SIZE);

    Vector2 pos = FIRST_BUTTON_POS;
    for (int i = 0; i < mWeapons.size(); i++) {
        AddButton(mWeapons[i]->name, pos, Vector2(BUTTON_SIZE, 30), [this, i]() { BuyWeapon(i); },
                  Vector2(mWeapons[i]->name.size() * CHAR_WIDTH, WORD_HEIGHT));
        pos += VERTICAL_OFFSET;
    }
    AddButton("End shopping", pos + Vector2(211, 0), Vector2(BUTTON_SIZE, 30), [this]() { GoToExp(); },
              Vector2(12 * CHAR_WIDTH, WORD_HEIGHT));

    mName = AddText("WeaponName", NAME_POS, Vector2(CHAR_WIDTH * 10, WORD_HEIGHT),
                    POINT_SIZE);

    mInfoMt = AddText("Might:", MT_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mMt = AddText("5", MT_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                  POINT_SIZE);

    mInfoHit = AddText("Hit:", HIT_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mHit = AddText("5", HIT_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    mInfoCrit = AddText("Critical:", CRIT_POS, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);
    mCrit = AddText("5", CRIT_POS + Vector2(CHAR_WIDTH * 9 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                    POINT_SIZE);

    mInfoRng = AddText("Range:", RNG_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mRng = AddText("5", RNG_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    mCost = AddText("Gold cost: 5", COST_POS, Vector2(170, 30), POINT_SIZE);

    mDetais = AddImage("../Assets/UI/PlainBackground.png", DETAIL_SCREEN_POS, DETAIL_SCREEN_SIZE);
}

ShopScreen::~ShopScreen() {
}

void ShopScreen::HandleKeyPress(int key) {
    if (!mInteractive)
        return;
    // Navigation
    if (key == SDLK_w) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = mButtons.size() - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_s) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex++;
        if (mSelectedButtonIndex > mButtons.size() - 1) {
            mSelectedButtonIndex = 0;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    }
    // Selection
    else if (key == SDLK_RETURN) {
        if (mSelectedButtonIndex >= 0 && mSelectedButtonIndex <= static_cast<int>(mButtons.size()) - 1) {
            mButtons[mSelectedButtonIndex]->OnClick();
        }
    }
}

void ShopScreen::Update(float deltaTime) {
    if (mSelectedButtonIndex <= mButtons.size() - 2) {
        SetName(mWeapons[mSelectedButtonIndex]->name);
        SetMight(std::to_string(mWeapons[mSelectedButtonIndex]->might));
        SetHit(std::to_string(mWeapons[mSelectedButtonIndex]->hit));
        SetCrit(std::to_string(mWeapons[mSelectedButtonIndex]->criticalChance));
        SetRange(std::to_string(mWeapons[mSelectedButtonIndex]->range));
        SetIcon(mSelectedButtonIndex);
        SetCost(mSelectedButtonIndex);
        SetInfoVisibility(true);
    } else {
        SetInfoVisibility(false);
    }
}

void ShopScreen::SetIcon(int index) {
    if (index <= 2) {
        mUnitIcon->SetImage(mGame->GetRenderer(), "../Assets/Sprites/Units/TrueBlade.png");
    } else if (index <= 5) {
        mUnitIcon->SetImage(mGame->GetRenderer(), "../Assets/Sprites/Units/Pegasus.png");
    } else if (index <= 8) {
        mUnitIcon->SetImage(mGame->GetRenderer(), "../Assets/Sprites/Units/Knight.png");
    } else if (index <= 10) {
        mUnitIcon->SetImage(mGame->GetRenderer(), "../Assets/Sprites/Units/Wizard.png");
    } else {
        mUnitIcon->SetVisible(false);
    }
}

void ShopScreen::SetCost(int index) {
    switch (index) {
        case 0:
        case 3:
        case 6:
            mCost->SetText("Gold cost: 1");
            break;
        case 1:
        case 4:
        case 7:
            mCost->SetText("Gold cost: 2");
            break;
        case 2:
        case 5:
        case 8:
        case 9:
            mCost->SetText("Gold cost: 3");
            break;
        case 10:
            mCost->SetText("Gold cost: 4");
            break;
        default:
            mCost->SetText(" ");
            break;
    }
}

int ShopScreen::GetCost(int index) {
    switch (index) {
        case 0:
        case 3:
        case 6:
            return 1;
        case 1:
        case 4:
        case 7:
            return 2;
        case 2:
        case 5:
        case 8:
        case 9:
            return 3;
        case 10:
            return 4;
        default:
            return 0;
    }
}

void ShopScreen::BuyWeapon(int index) {
    std::unordered_set<int>::const_iterator got = mBought.find(index);
    if (GetCost(index) > mGold || got != mBought.end()) {
        mGame->GetAudio()->PlaySound("Error.ogg");
        return;
    }
    if (index <= 2) {
        if (mGame->GetTrueblade()->GetAllWeapons().size() >= 4) {
            mGame->GetAudio()->PlaySound("Error.ogg");
            return;
        }
        mGame->GetTrueblade()->AddWeapon(mWeapons[mSelectedButtonIndex]);
    } else if (index <= 5) {
        if (mGame->GetPegasusKnight()->GetAllWeapons().size() >= 4) {
            mGame->GetAudio()->PlaySound("Error.ogg");
            return;
        }
        mGame->GetPegasusKnight()->AddWeapon(mWeapons[mSelectedButtonIndex]);
    } else if (index <= 8) {
        if (mGame->GetWarrior()->GetAllWeapons().size() >= 4) {
            mGame->GetAudio()->PlaySound("Error.ogg");
            return;
        }
        mGame->GetWarrior()->AddWeapon(mWeapons[mSelectedButtonIndex]);
    } else if (index <= 10) {
        if (mGame->GetMage()->GetAllWeapons().size() >= 4) {
            mGame->GetAudio()->PlaySound("Error.ogg");
            return;
        }
        mGame->GetMage()->AddWeapon(mWeapons[mSelectedButtonIndex]);
    }
    mGold -= GetCost(index);
    mCurrentGold->SetText("Current gold: " + std::to_string(mGold));
    mBought.insert(index);
    mGame->GetAudio()->PlaySound("CursorSelect.ogg");
}

void ShopScreen::GoToExp() {
    mGame->GoToExpScreen();
}

void ShopScreen::SetInfoVisibility(bool visible) {
    mWriteUnit->SetVisible(visible);
    mInfoMt->SetVisible(visible);
    mInfoHit->SetVisible(visible);
    mInfoCrit->SetVisible(visible);
    mInfoRng->SetVisible(visible);
    mDetais->SetVisible(visible);
    mUnitIcon->SetVisible(visible);
    mCost->SetVisible(visible);
    if (!visible) {
        mName->SetText(" ");
        mMt->SetText(" ");
        mHit->SetText(" ");
        mCrit->SetText(" ");
        mRng->SetText(" ");
    }
}

void ShopScreen::SetName(const std::string &name) {
    mName->SetText(name);
    int len = name.length() * CHAR_WIDTH;
    mName->SetPosition(Vector2(DETAIL_SCREEN_POS.x + DETAIL_SCREEN_SIZE.x / 2 - len / 2, NAME_POS.y));
    mName->SetSize(Vector2(len, WORD_HEIGHT));
}

void ShopScreen::SetMight(const std::string &mt) {
    mMt->SetText(mt);
    int len = mt.length() * CHAR_WIDTH;
    mMt->SetSize(Vector2(len, WORD_HEIGHT));
}

void ShopScreen::SetHit(const std::string &hit) {
    mHit->SetText(hit);
    int len = hit.length() * CHAR_WIDTH;
    mHit->SetSize(Vector2(len, WORD_HEIGHT));
}

void ShopScreen::SetCrit(const std::string &crit) {
    mCrit->SetText(crit);
    int len = crit.length() * CHAR_WIDTH;
    mCrit->SetSize(Vector2(len, WORD_HEIGHT));
}

void ShopScreen::SetRange(const std::string &rng) {
    mRng->SetText(rng);
    int len = rng.length() * CHAR_WIDTH;
    mRng->SetSize(Vector2(len, WORD_HEIGHT));
}
