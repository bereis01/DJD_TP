#pragma once
#include "DrawSpriteComponent.h"
#include <unordered_map>

class DrawAnimatedComponent : public DrawSpriteComponent {
public:
    // (Lower draw order corresponds with further back)
    DrawAnimatedComponent(Actor *owner, const std::string &spriteSheetPath, const std::string &spriteSheetData,
                          int drawOrder = 100, int width = 124, int height = 124, int widthOffset = 46,
                          int heightOffset = 46);

    ~DrawAnimatedComponent();

    void Draw(SDL_Renderer *renderer) override;

    void Update(float deltaTime) override;

    // Use to change the FPS of the animation
    void SetAnimFPS(float fps) { mAnimFPS = fps; }

    // Set the current active animation
    void SetAnimation(const std::string &name);

    // Use to pause/unpause the animation
    void SetIsPaused(bool pause) { mIsPaused = pause; }

    // Add an animation of the corresponding name to the animation map
    void AddAnimation(const std::string &name, const std::vector<int> &images);

private:
    // Loads animation sprites as spritesheet
    void LoadSpriteSheet(const std::string &texturePath, const std::string &dataPath);

    // Vector of sprites (contains all of them)
    std::vector<SDL_Rect *> mSpriteSheetData;

    // Map of animation name to vector of indexes
    // Indices correspond to the animations sprites in mSpriteSheetData
    std::unordered_map<std::string, std::vector<int> > mAnimations;

    // Name of current animation
    std::string mAnimName;

    // Tracks current elapsed time in animation
    float mAnimTimer = 0.0f;

    // The frames per second the animation should run at
    float mAnimFPS = 10.0f;

    // Whether the animation is paused (defaults to false)
    bool mIsPaused = false;
};
