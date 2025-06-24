#include "StatScreen.h"
#include "Game.h"
#include "UIElements/UIText.h"
#include "Actors/Unit.h"

StatScreen::StatScreen(class Game* game, const std::string& fontName)
        :UIScreen(game, fontName)
{
    AddText("Unit:", NAME_POS, Vector2(CHAR_WIDTH * 5, WORD_HEIGHT), POINT_SIZE);
    mName = AddText("Edel", NAME_POS + Vector2(CHAR_WIDTH * 5 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);

    AddText("Hp:", HP_POS, Vector2(CHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE);
    mHp = AddText("25", HP_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);

    AddText("Str:", STR_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mStr = AddText("12", STR_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 2, WORD_HEIGHT), POINT_SIZE);

    AddText("Mag:", MAG_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mMag = AddText("4", MAG_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT), POINT_SIZE);

    AddText("Spd:", SPD_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mSpd = AddText("9", SPD_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT), POINT_SIZE);

    AddText("Def:", DEF_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mDef = AddText("9", DEF_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT), POINT_SIZE);

    AddText("Res:", RES_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT), POINT_SIZE);
    mRes = AddText("5", RES_POS + Vector2(CHAR_WIDTH * 4 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT), POINT_SIZE);

    AddImage("../Assets/Other/stat_background.png", SCREEN_POS, SCREEN_SIZE, mGame->GetRenderer());
}

StatScreen::~StatScreen() {
}

void StatScreen::SetStats(class Stats stats) {
    SetName(stats.name);
    SetHp(std::to_string(stats.hp));
    SetStr(std::to_string(stats.str));
    SetMag(std::to_string(stats.mag));
    SetSpd(std::to_string(stats.spd));
    SetDef(std::to_string(stats.def));
    SetRes(std::to_string(stats.res));
}

void StatScreen::SetName(const std::string &name) {
    mName->SetText(name);
}

void StatScreen::SetHp(const std::string &hp) {
    mHp->SetText(hp);
}

void StatScreen::SetStr(const std::string &str) {
    mStr->SetText(str);
}

void StatScreen::SetMag(const std::string &mag) {
    mMag->SetText(mag);
}

void StatScreen::SetSpd(const std::string &spd) {
    mSpd->SetText(spd);
}

void StatScreen::SetDef(const std::string &def) {
    mDef->SetText(def);
}

void StatScreen::SetRes(const std::string &res) {
    mRes->SetText(res);
}