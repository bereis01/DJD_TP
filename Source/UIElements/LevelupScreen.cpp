#include "LevelupScreen.h"
#include "../Game.h"
#include "../Actors/Ally.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"
#include "../Audio/AudioSystem.h"

LevelupScreen::LevelupScreen(class Game *game, const std::string &fontName, int level)
    : UIScreen(game, fontName, true) {
    mCurrentUpgrade = 0;
    mSecondChoice = false;
    SCREEN_SIZE = Vector2(game->GetWindowWidth(), game->GetWindowHeight());
    HORIZONTAL_OFFSET = Vector2(SCREEN_SIZE.x / 4.5, 0);

    AddImage("../Assets/UI/ShopBackground.png", SCREEN_POS, SCREEN_SIZE);
    //AddText("Level Up", Vector2(SCREEN_SIZE.x / 2 - 125, 30), Vector2(250, 69));
    AddImage("../Assets/UI/Upgrade.png", Vector2((mGame->GetWindowWidth() - 384) / 2, -40), Vector2(384, 216));

    Vector2 dim = Vector2(29 * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT);
    mInfoText = AddText("Give +1 strength to two units", Vector2(SCREEN_SIZE.x / 2 - dim.x / 2, 142), dim);

    AddButton("Mia", FIRST_BUTTON_POS, BUTTON_SIZE, [this]() { Upgrade(); },
              Vector2(3 * CHAR_WIDTH, WORD_HEIGHT));
    AddImage("../Assets/Sprites/Units/TrueBlade.png", FIRST_UNIT_ICON_POS, UNIT_ICON_SIZE);

    AddButton("Marcia", FIRST_BUTTON_POS + HORIZONTAL_OFFSET, BUTTON_SIZE, [this]() { Upgrade(); },
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    AddImage("../Assets/Sprites/Units/Pegasus.png", FIRST_UNIT_ICON_POS + HORIZONTAL_OFFSET, UNIT_ICON_SIZE);

    AddButton("Hubert", FIRST_BUTTON_POS + HORIZONTAL_OFFSET * 2, BUTTON_SIZE, [this]() { Upgrade(); },
              Vector2(6 * CHAR_WIDTH, WORD_HEIGHT));
    AddImage("../Assets/Sprites/Units/Wizard.png", FIRST_UNIT_ICON_POS + HORIZONTAL_OFFSET * 2, UNIT_ICON_SIZE);

    AddButton("Ferdinand", FIRST_BUTTON_POS + HORIZONTAL_OFFSET * 3, BUTTON_SIZE, [this]() { Upgrade(); },
              Vector2(9 * CHAR_WIDTH, WORD_HEIGHT));
    AddImage("../Assets/Sprites/Units/Knight.png", FIRST_UNIT_ICON_POS + HORIZONTAL_OFFSET * 3, UNIT_ICON_SIZE);


    for (int i = 0; i < 4; i++) {
        AddText("Strength:", STR_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);
        mStr.emplace_back(AddText("12", STR_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 9 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH * 2, WORD_HEIGHT),
                                  POINT_SIZE));

        AddText("Magic:", MAG_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
        mMag.emplace_back(AddText("4", MAG_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH, WORD_HEIGHT),
                                  POINT_SIZE));

        AddText("Skill:", SKL_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
        mSkl.emplace_back(AddText("9", SKL_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH, WORD_HEIGHT),
                                  POINT_SIZE));

        AddText("Speed:", SPD_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
        mSpd.emplace_back(AddText("9", SPD_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH, WORD_HEIGHT),
                                  POINT_SIZE));

        AddText("Defense:", DEF_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 8, WORD_HEIGHT), POINT_SIZE);
        mDef.emplace_back(AddText("9", DEF_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 8 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH, WORD_HEIGHT),
                                  POINT_SIZE));

        AddText("Resistance:", RES_POS + HORIZONTAL_OFFSET * i, Vector2(CHAR_WIDTH * 11, WORD_HEIGHT), POINT_SIZE);
        mRes.emplace_back(AddText("5", RES_POS + HORIZONTAL_OFFSET * i + Vector2(CHAR_WIDTH * 11 + WORD_OFFSET, 0),
                                  Vector2(CHAR_WIDTH, WORD_HEIGHT),
                                  POINT_SIZE));
    }
}

LevelupScreen::~LevelupScreen() {
}

void LevelupScreen::HandleKeyPress(int key) {
    // Corner cases
    if (mGame->GetSceneManagerState() == Game::SceneManagerState::Exiting)
        return;
    if (!mInteractive)
        return;

    // Navigation
    if (key == SDLK_a) {
        mButtons[mSelectedButtonIndex]->SetHighlighted(false);
        mSelectedButtonIndex--;
        if (mSelectedButtonIndex < 0) {
            mSelectedButtonIndex = mButtons.size() - 1;
        }
        mButtons[mSelectedButtonIndex]->SetHighlighted(true);

        // Plays audio
        mGame->GetAudio()->PlaySound("CursorMove.ogg");
    } else if (key == SDLK_d) {
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
        mGame->GetAudio()->PlaySound("CursorSelect.ogg");
    }
}

void LevelupScreen::Update(float deltaTime) {
    Stats s;
    s = mGame->GetTrueblade()->GetStats();
    SetStr(std::to_string(s.str), 0);
    SetMag(std::to_string(s.mag), 0);
    SetSkl(std::to_string(s.skl), 0);
    SetSpd(std::to_string(s.spd), 0);
    SetDef(std::to_string(s.def), 0);
    SetRes(std::to_string(s.res), 0);

    s = mGame->GetPegasusKnight()->GetStats();
    SetStr(std::to_string(s.str), 1);
    SetMag(std::to_string(s.mag), 1);
    SetSkl(std::to_string(s.skl), 1);
    SetSpd(std::to_string(s.spd), 1);
    SetDef(std::to_string(s.def), 1);
    SetRes(std::to_string(s.res), 1);

    s = mGame->GetMage()->GetStats();
    SetStr(std::to_string(s.str), 2);
    SetMag(std::to_string(s.mag), 2);
    SetSkl(std::to_string(s.skl), 2);
    SetSpd(std::to_string(s.spd), 2);
    SetDef(std::to_string(s.def), 2);
    SetRes(std::to_string(s.res), 2);

    s = mGame->GetWarrior()->GetStats();
    SetStr(std::to_string(s.str), 3);
    SetMag(std::to_string(s.mag), 3);
    SetSkl(std::to_string(s.skl), 3);
    SetSpd(std::to_string(s.spd), 3);
    SetDef(std::to_string(s.def), 3);
    SetRes(std::to_string(s.res), 3);
}

void LevelupScreen::Upgrade() {
    Stats s;
    Ally *target = nullptr;
    if (mSelectedButtonIndex == 0) {
        target = mGame->GetTrueblade();
        s = mGame->GetTrueblade()->GetStats();
    } else if (mSelectedButtonIndex == 1) {
        target = mGame->GetPegasusKnight();
        s = mGame->GetPegasusKnight()->GetStats();
    } else if (mSelectedButtonIndex == 2) {
        target = mGame->GetMage();
        s = mGame->GetMage()->GetStats();
    } else if (mSelectedButtonIndex == 3) {
        target = mGame->GetWarrior();
        s = mGame->GetWarrior()->GetStats();
    }
    bool isOver = false;
    switch (mCurrentUpgrade) {
        case (0):
            s.str++;
            if (mSecondChoice) {
                mCurrentUpgrade = 1;
                mSecondChoice = false;
                std::string t = "Give +1 magic to two units";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
            } else {
                mSecondChoice = true;
            }
            break;
        case (1):
            s.mag++;
            if (mSecondChoice) {
                mCurrentUpgrade = 2;
                mSecondChoice = false;
                std::string t = "Give +1 skill to two units";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
            } else {
                mSecondChoice = true;
            }
            break;
        case (2):
            s.skl++;
            if (mSecondChoice) {
                mCurrentUpgrade = 3;
                mSecondChoice = false;
                std::string t = "Give +1 speed to two units";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
            } else {
                mSecondChoice = true;
            }
            break;
        case (3):
            s.spd++;
            if (mSecondChoice) {
                mCurrentUpgrade = 4;
                mSecondChoice = false;
                std::string t = "Give +1 defense to two units";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
            } else {
                mSecondChoice = true;
            }
            break;
        case (4):
            s.def++;
            if (mSecondChoice) {
                mCurrentUpgrade = 5;
                mSecondChoice = false;
                std::string t = "Give +1 resistance to two units";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
            } else {
                mSecondChoice = true;
            }
            break;
        case (5):
            s.res++;
            if (mSecondChoice) {
                mCurrentUpgrade = 0;
                mSecondChoice = false;
                std::string t = "Done!";
                mInfoText->SetText(t);
                mInfoText->SetSize(Vector2(t.size() * 2 * CHAR_WIDTH, 2 * WORD_HEIGHT));
                mInfoText->SetPosition(Vector2(SCREEN_SIZE.x / 2 - (t.size() * 18) / 2, 142));
                isOver = true;
            } else {
                mSecondChoice = true;
            }
            break;
        default:
            break;
    }
    if (target != nullptr) {
        target->SetStats(s);
    }
    if (isOver) {
        mGame->SetGameScene(Game::GameScene::Level2);

        // Plays audio
        mGame->GetAudio()->PlaySound("Finish.ogg");
    }
}

void LevelupScreen::SetStr(const std::string &str, int index) {
    mStr[index]->SetText(str);
    int len = str.length() * CHAR_WIDTH;
    mStr[index]->SetSize(Vector2(len, WORD_HEIGHT));
}


void LevelupScreen::SetMag(const std::string &mag, int index) {
    mMag[index]->SetText(mag);
    int len = mag.length() * CHAR_WIDTH;
    mMag[index]->SetSize(Vector2(len, WORD_HEIGHT));
}

void LevelupScreen::SetSkl(const std::string &skl, int index) {
    mSkl[index]->SetText(skl);
    int len = skl.length() * CHAR_WIDTH;
    mSkl[index]->SetSize(Vector2(len, WORD_HEIGHT));
}

void LevelupScreen::SetSpd(const std::string &spd, int index) {
    mSpd[index]->SetText(spd);
    int len = spd.length() * CHAR_WIDTH;
    mSpd[index]->SetSize(Vector2(len, WORD_HEIGHT));
}

void LevelupScreen::SetDef(const std::string &def, int index) {
    mDef[index]->SetText(def);
    int len = def.length() * CHAR_WIDTH;
    mDef[index]->SetSize(Vector2(len, WORD_HEIGHT));
}

void LevelupScreen::SetRes(const std::string &res, int index) {
    mRes[index]->SetText(res);
    int len = res.length() * CHAR_WIDTH;
    mRes[index]->SetSize(Vector2(len, WORD_HEIGHT));
}
