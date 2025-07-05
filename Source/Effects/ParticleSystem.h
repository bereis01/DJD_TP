#pragma once
#include "../Game.h"

class ParticleSystem {
public:
    ParticleSystem(Game *game, const std::string &fontName);

    // Text particle
    void CreateTextParticle(int x, int y, const std::string &text);

    // Animated particle
    void CreateAnimatedParticle(int x, int y, const std::string &type);

private:
    Game *mGame = nullptr;

    // Text particle
    UIFont *mFont = nullptr;
};
