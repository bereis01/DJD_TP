#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Json.h"
#include <fstream>

DrawAnimatedComponent::DrawAnimatedComponent(class Actor *owner, const std::string &spriteSheetPath,
                                             const std::string &spriteSheetData, int drawOrder)
    : DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder) {
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent() {
    for (const auto &rect: mSpriteSheetData) {
        delete rect;
    }
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string &texturePath, const std::string &dataPath) {
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    // Load sprite sheet data
    std::ifstream spriteSheetFile(dataPath);
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    SDL_Rect *rect = nullptr;
    for (const auto &frame: spriteSheetData["frames"]) {
        int x = frame["frame"]["x"].get<int>();
        int y = frame["frame"]["y"].get<int>();
        int w = frame["frame"]["w"].get<int>();
        int h = frame["frame"]["h"].get<int>();
        rect = new SDL_Rect({x, y, w, h});

        mSpriteSheetData.emplace_back(rect);
    }
}

void DrawAnimatedComponent::Draw(SDL_Renderer *renderer) {
    // Calculates the correct sprite index based on elapsed time and target framerate
    int spriteIdx = mAnimations[mAnimName][static_cast<int>(mAnimTimer)];

    // Renders the correct sprite
    SDL_Rect *srcRect = mSpriteSheetData[spriteIdx];
    SDL_RendererFlip flip = mOwner->GetRotation() == 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
    Vector2 ownerPosition = mOwner->GetPosition();
    Vector2 cameraPosition = mOwner->GetGame()->GetCameraPos();
    SDL_Rect dstRect = {
        static_cast<int>(ownerPosition.x - cameraPosition.x), static_cast<int>(ownerPosition.y - cameraPosition.y),
        srcRect->w, srcRect->h
    };
    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, srcRect, &dstRect, 0, nullptr, flip);
}

void DrawAnimatedComponent::Update(float deltaTime) {
    // Do nothing if animation is paused
    if (mIsPaused)
        return;

    // Updates animation timer based on elapsed time and target framerate
    mAnimTimer += deltaTime * mAnimFPS;

    // Adjusts the timer based on current animation
    while (mAnimTimer >= mAnimations[mAnimName].size())
        mAnimTimer -= mAnimations[mAnimName].size();
}

void DrawAnimatedComponent::SetAnimation(const std::string &name) {
    mAnimName = name;
    Update(0.0f); // Resets animation timer
}

void DrawAnimatedComponent::AddAnimation(const std::string &name, const std::vector<int> &spriteNums) {
    mAnimations.emplace(name, spriteNums);
}
