#include "ParticleSystem.h"
#include "Particle.h"
#include "AnimatedParticle.h"
#include "TitleParticle.h"

ParticleSystem::ParticleSystem(Game *game, const std::string &fontName) : mGame(game) {
    // Loads the font to be used by text particles
    mFont = mGame->LoadFont(fontName);
}

void ParticleSystem::CreateTextParticle(int x, int y, const std::string &text) {
    // Creates the text for the particle
    SDL_Texture *textTexture = mFont->RenderText(text);

    // Creates the text particle
    Particle *textParticle = new Particle(mGame, textTexture, Vector2(7.5f * text.size(), 7.5f));
    textParticle->SetPosition(Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE));
}

void ParticleSystem::CreateAnimatedParticle(int x, int y, const std::string &type) {
    // Creates the animated particle
    AnimatedParticle *animatedParticle = new AnimatedParticle(mGame, type, Vector2(100, 100));
    animatedParticle->SetPosition(Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE));
}

void ParticleSystem::CreateTitleParticle(const std::string &text) {
    int WIDTH = 640;
    int HEIGHT = 360;

    // Creates the text for the particle
    SDL_Texture *titleTexture = nullptr;
    if (text == "Level1")
        titleTexture = mGame->LoadTexture("../Assets/UI/Level1Title.png");

    // Creates the text particle
    TitleParticle *titleParticle = new
            TitleParticle(mGame, titleTexture, Vector2(WIDTH, HEIGHT));
    titleParticle->SetPosition(Vector2((mGame->GetWindowWidth() - WIDTH) / 2 + mGame->GetCameraPos().x,
                                       (mGame->GetWindowHeight() - HEIGHT) / 2 + mGame->GetCameraPos().y));
}
