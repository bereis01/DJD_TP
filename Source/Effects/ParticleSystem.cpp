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
    int WORD_HEIGHT = 75.0f;
    int CHAR_SIZE = 50.0f;

    // Creates the text for the particle
    SDL_Texture *titleTexture = mFont->RenderText(text, Color::White, 64);

    // Creates the text particle
    TitleParticle *titleParticle = new
            TitleParticle(mGame, titleTexture, Vector2(CHAR_SIZE * text.size(), WORD_HEIGHT));
    titleParticle->SetPosition(Vector2((mGame->GetWindowWidth() - CHAR_SIZE * text.size()) / 2,
                                       mGame->GetWindowHeight() / 2));
}
