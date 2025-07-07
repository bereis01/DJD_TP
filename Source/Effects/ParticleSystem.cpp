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

void ParticleSystem::CreateTitleParticle(const std::string &type, float timer, float fadeInTimer, float fadeOutTimer,
                                         bool setTimer, Vector2 offset) {
    int WIDTH = 640;
    int HEIGHT = 360;

    // Creates the text for the particle
    SDL_Texture *titleTexture = nullptr;
    if (type == "Title")
        titleTexture = mGame->LoadTexture("../Assets/UI/Title.png");
    else if (type == "Level1")
        titleTexture = mGame->LoadTexture("../Assets/UI/Level1.png");
    else if (type == "Level2")
        titleTexture = mGame->LoadTexture("../Assets/UI/Level2.png");
    else if (type == "Level3")
        titleTexture = mGame->LoadTexture("../Assets/UI/Level3.png");
    else if (type == "Instructions") {
        std::string instructions = "Defeat all the enemies!";
        titleTexture = mFont->RenderText(instructions, Color::White, 48);
        WIDTH = 10.0f * instructions.size();
        HEIGHT = 15.0f;
    }

    // Creates the text particle
    TitleParticle *titleParticle = new
            TitleParticle(mGame, titleTexture, Vector2(WIDTH, HEIGHT), timer, fadeInTimer, fadeOutTimer, setTimer,
                          offset);
    titleParticle->SetPosition(Vector2((mGame->GetWindowWidth() - WIDTH) / 2 + mGame->GetCameraPos().x,
                                       (mGame->GetWindowHeight() - HEIGHT) / 2 + mGame->GetCameraPos().y) + offset);
}
