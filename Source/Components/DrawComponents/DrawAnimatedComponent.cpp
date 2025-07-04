#include "DrawAnimatedComponent.h"
#include "../../Actors/Actor.h"
#include "../../Game.h"
#include "../../Utils/Json.h"
#include <fstream>

DrawAnimatedComponent::DrawAnimatedComponent(Actor *owner, const std::string &spriteSheetPath,
                                             const std::string &spriteSheetData, int drawOrder)
    : DrawSpriteComponent(owner, spriteSheetPath, 0, 0, drawOrder) {
    // Loads the spritesheet containing all animation sprites
    LoadSpriteSheet(spriteSheetPath, spriteSheetData);
}

DrawAnimatedComponent::~DrawAnimatedComponent() {
    // Cleans spritesheet data (texture)
    for (const auto &rect: mSpriteSheetData) {
        delete rect;
    }
    mSpriteSheetData.clear();
}

void DrawAnimatedComponent::LoadSpriteSheet(const std::string &texturePath, const std::string &dataPath) {
    // Load sprite sheet texture
    mSpriteSheetSurface = mOwner->GetGame()->LoadTexture(texturePath);

    // Load sprite sheet data from JSON
    std::ifstream spriteSheetFile(dataPath);
    nlohmann::json spriteSheetData = nlohmann::json::parse(spriteSheetFile);

    // Checks the data and organizes each sprite in the spritesheet in the sprite vector
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
    // From the sprite name and the animation timer,
    // gets the index of the correct sprite and its texture
    int spriteIdx = mAnimations[mAnimName][static_cast<int>(mAnimTimer)];
    SDL_Rect *srcRect = mSpriteSheetData[spriteIdx];

    // Creates destiny rect based on actor and camera positions
    Vector2 pos = mOwner->GetPosition();
    Vector2 cameraPos = mOwner->GetGame()->GetCameraPos();
    // SDL_Rect dstRect = {
    //     static_cast<int>(pos.x - cameraPos.x),
    //     static_cast<int>(pos.y - cameraPos.y),
    //     srcRect->w,
    //     srcRect->h
    // };
    SDL_Rect dstRect = {
        static_cast<int>(pos.x - cameraPos.x - 59),
        static_cast<int>(pos.y - cameraPos.y - 59),
        150,
        150
    };

    // Applies flip if applicable
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if (mOwner->GetRotation() == Math::Pi) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    // Renders the texture
    // SDL_SetTextureBlendMode(mSpriteSheetSurface, SDL_BLENDMODE_BLEND);
    // SDL_SetTextureColorMod(mSpriteSheetSurface, static_cast<Uint8>(modColor.x), static_cast<Uint8>(modColor.y), static_cast<Uint8>(modColor.z));
    SDL_RenderCopyEx(renderer, mSpriteSheetSurface, srcRect, &dstRect, mOwner->GetRotation(), nullptr, flip);
}

void DrawAnimatedComponent::Update(float deltaTime) {
    // Does nothing if paused
    if (mIsPaused) return;

    // Updates the animation timer based on elapsed time
    mAnimTimer += mAnimFPS * deltaTime;
    if (mAnimTimer >= mAnimations[mAnimName].size()) {
        while (mAnimTimer >= mAnimations[mAnimName].size()) {
            mAnimTimer -= mAnimations[mAnimName].size();
        }
    }
}

void DrawAnimatedComponent::SetAnimation(const std::string &name) {
    // Changes the animation (done by the actor)
    mAnimName = name;
    Update(0.0f);
}

void DrawAnimatedComponent::AddAnimation(const std::string &name, const std::vector<int> &spriteNums) {
    // Adds a new animation (done by the actor)
    mAnimations.emplace(name, spriteNums);
}
