#pragma once
#include "../Game.h"

class ParticleSystem {
public:
    ParticleSystem(Game *game, const std::string &fontName);

    // Text particle
    void CreateTextParticle(int x, int y, const std::string &text);

    void CreateTitleParticle(const std::string &type, float timer = 5.0f, float fadeInTimer = 1.0f,
                             float fadeOutTimer = 1.0f, bool setTimer = true);

    // Animated particle
    void CreateAnimatedParticle(int x, int y, const std::string &type);

private:
    Game *mGame = nullptr;

    // Text particle
    UIFont *mFont = nullptr;
};
