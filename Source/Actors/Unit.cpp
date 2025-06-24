#include "Unit.h"
#include "../Game.h"
#include "../StatScreen.h"

Stats::Stats(std::string n, int h, int st, int m, int sp, int d, int r) {
    name = n;
    hp = h;
    str = st;
    mag = m;
    spd = sp;
    def = d;
    res = r;
}

Unit::Unit(Game *game, const std::string &texturePath)
    : Actor(game) {
    mDrawComponent = new DrawSpriteComponent(this, texturePath, Game::TILE_SIZE, Game::TILE_SIZE, 200);
}

void Unit::OnUpdate(float deltaTime) {
}

void Unit::SetStats(class Stats stats) {
    mStats.name = stats.name;
    mStats.hp = stats.hp;
    mStats.str = stats.str;
    mStats.mag = stats.mag;
    mStats.spd = stats.spd;
    mStats.def = stats.def;
    mStats.res = stats.res;
}

void Unit::ShowStats() {
    mGame->GetStatScreen()->SetStats(mStats);
    mGame->PushUI(reinterpret_cast<struct UIScreen *>(mGame->GetStatScreen()));
}