    #pragma once

#include <string>

#include "UIElements/UIScreen.h"

class StatScreen : public UIScreen
{
public:
    const int POINT_SIZE = 48;
    const int WORD_HEIGHT = 36.0f;
    const int WORD_OFFSET = 12.0f;
    const int CHAR_WIDTH = 16.0f;
    const Vector2 SCREEN_POS = Vector2(80,80);
    const Vector2 SCREEN_SIZE = Vector2(450,450);
    const Vector2 NAME_POS = Vector2(100,100);
    const Vector2 HP_POS = Vector2(300,100);
    const Vector2 STR_POS = Vector2(100,200);
    const Vector2 MAG_POS = Vector2(300,200);
    const Vector2 SKL_POS = Vector2(100,300);
    const Vector2 SPD_POS = Vector2(300,300);
    const Vector2 DEF_POS = Vector2(100,400);
    const Vector2 RES_POS = Vector2(300,400);

    StatScreen(class Game* game, const std::string& fontName);
    ~StatScreen();

    void SetDisplayStats(class Stats stats, int dmgTaken);
    void SetName(const std::string &name);
    void SetHp(const std::string &hp);
    void SetStr(const std::string &str);
    void SetMag(const std::string &mag);
    void SetSkl(const std::string &skl);
    void SetSpd(const std::string &spd);
    void SetDef(const std::string &def);
    void SetRes(const std::string &res);

private:
    UIText* mName;
    UIText* mHp;
    UIText* mStr;
    UIText* mMag;
    UIText* mSkl;
    UIText* mSpd;
    UIText* mDef;
    UIText* mRes;
};