#include "StatScreen.h"
#include "../Game.h"
#include "../UIElements/UIText.h"
#include "../Actors/Unit.h"

StatScreen::StatScreen(class Game *game, const std::string &fontName)
    : UIScreen(game, fontName) {
    // AddText("Unit:", NAME_POS, Vector2(CHAR_WIDTH * 5, WORD_HEIGHT), POINT_SIZE);
    mName = AddText("Edel", NAME_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT),
                    POINT_SIZE);

    AddText("HP:", HP_POS, Vector2(CHAR_WIDTH * 3, WORD_HEIGHT), POINT_SIZE);
    mHp = AddText("25/25", HP_POS + Vector2(CHAR_WIDTH * 3 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 5, WORD_HEIGHT),
                  POINT_SIZE);

    AddText("Movement:", MOV_POS, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);
    mMove = AddText("5", MOV_POS + Vector2(CHAR_WIDTH * 9 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 1, WORD_HEIGHT),
                    POINT_SIZE);

    AddText("Strength:", STR_POS, Vector2(CHAR_WIDTH * 9, WORD_HEIGHT), POINT_SIZE);
    mStr = AddText("12", STR_POS + Vector2(CHAR_WIDTH * 9 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH * 2, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Magic:", MAG_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mMag = AddText("4", MAG_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Skill:", SKL_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mSkl = AddText("9", SKL_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Speed:", SPD_POS, Vector2(CHAR_WIDTH * 6, WORD_HEIGHT), POINT_SIZE);
    mSpd = AddText("9", SPD_POS + Vector2(CHAR_WIDTH * 6 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Defense:", DEF_POS, Vector2(CHAR_WIDTH * 8, WORD_HEIGHT), POINT_SIZE);
    mDef = AddText("9", DEF_POS + Vector2(CHAR_WIDTH * 8 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    AddText("Resistance:", RES_POS, Vector2(CHAR_WIDTH * 11, WORD_HEIGHT), POINT_SIZE);
    mRes = AddText("5", RES_POS + Vector2(CHAR_WIDTH * 11 + WORD_OFFSET, 0), Vector2(CHAR_WIDTH, WORD_HEIGHT),
                   POINT_SIZE);

    mWpn = AddText("Aymr", WEAPON_POS, Vector2(CHAR_WIDTH * 4, WORD_HEIGHT),
                   POINT_SIZE);

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

    AddImage("../Assets/UI/WoodBackground.png", SCREEN_POS, SCREEN_SIZE);
}

void StatScreen::SetDisplayStats(class Unit *unit) {
    Stats stats = unit->GetStats();
    Weapon *weapon = unit->GetEquippedWeapon();
    SetName(stats.name);
    std::string max_hp = std::to_string(stats.hp);
    std::string curr_hp = std::to_string(stats.currHp);
    std::string hp_str = curr_hp + "/" + max_hp;
    SetHp(hp_str);
    SetMove(std::to_string(stats.mov));
    SetStr(std::to_string(stats.str));
    SetMag(std::to_string(stats.mag));
    SetSkl(std::to_string(stats.skl));
    SetSpd(std::to_string(stats.spd));
    SetDef(std::to_string(stats.def));
    SetRes(std::to_string(stats.res));
    SetWeaponName(weapon->name);
    SetMight(std::to_string(weapon->might));
    SetHit(std::to_string(weapon->hit));
    SetCrit(std::to_string(weapon->criticalChance));
    SetRange(std::to_string(weapon->range));
}

void StatScreen::SetName(const std::string &name) {
    mName->SetText(name);
    int len = name.length() * CHAR_WIDTH;
    mName->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, NAME_POS.y));
    mName->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetHp(const std::string &hp) {
    mHp->SetText(hp);
    int len = hp.length() * CHAR_WIDTH;
    mHp->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetMove(const std::string &mov) {
    mMove->SetText(mov);
    int len = mov.length() * CHAR_WIDTH;
    mMove->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetStr(const std::string &str) {
    mStr->SetText(str);
    int len = str.length() * CHAR_WIDTH;
    mStr->SetSize(Vector2(len, WORD_HEIGHT));
}


void StatScreen::SetMag(const std::string &mag) {
    mMag->SetText(mag);
    int len = mag.length() * CHAR_WIDTH;
    mMag->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetSkl(const std::string &skl) {
    mSkl->SetText(skl);
    int len = skl.length() * CHAR_WIDTH;
    mSkl->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetSpd(const std::string &spd) {
    mSpd->SetText(spd);
    int len = spd.length() * CHAR_WIDTH;
    mSpd->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetDef(const std::string &def) {
    mDef->SetText(def);
    int len = def.length() * CHAR_WIDTH;
    mDef->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetRes(const std::string &res) {
    mRes->SetText(res);
    int len = res.length() * CHAR_WIDTH;
    mRes->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetWeaponName(const std::string &name) {
    mWpn->SetText(name);
    int len = name.length() * CHAR_WIDTH;
    mWpn->SetPosition(Vector2(SCREEN_POS.x + SCREEN_SIZE.x / 2 - len / 2, WEAPON_POS.y));
    mWpn->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetMight(const std::string &mt) {
    mMt->SetText(mt);
    int len = mt.length() * CHAR_WIDTH;
    mMt->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetHit(const std::string &hit) {
    mHit->SetText(hit);
    int len = hit.length() * CHAR_WIDTH;
    mHit->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetCrit(const std::string &crit) {
    mCrit->SetText(crit);
    int len = crit.length() * CHAR_WIDTH;
    mCrit->SetSize(Vector2(len, WORD_HEIGHT));
}

void StatScreen::SetRange(const std::string &rng) {
    mRng->SetText(rng);
    int len = rng.length() * CHAR_WIDTH;
    mRng->SetSize(Vector2(len, WORD_HEIGHT));
}
