#include "ParticleSystem.h"
#include "Particle.h"

ParticleSystem::ParticleSystem(Game *game, const std::string &fontName) : mGame(game) {
    // Loads the font to be used by text particles
    mFont = mGame->LoadFont(fontName);
}

void ParticleSystem::CreateTextParticle(int x, int y, const std::string &text) {
    // Creates the text for the particle
    SDL_Texture *textTexture = mFont->RenderText(text);

    // Creates the text particle
    Particle *textParticle = new Particle(mGame, textTexture, Vector2(5.0f * text.size(), 10.0f));
    textParticle->SetPosition(Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE));
}
