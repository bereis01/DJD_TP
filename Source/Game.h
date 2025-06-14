#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Utils/Math.h"

class Game {
public:
    static const int LEVEL_WIDTH = 100; // In tiles
    static const int LEVEL_HEIGHT = 100; // In tiles
    static const int TILE_SIZE = 32; // In pixels

    Game(int windowWidth, int windowHeight);

    // Game functions
    bool Initialize();

    void RunLoop();

    void Shutdown();

    void Quit() { mIsRunning = false; }

    // Actor functions
    void InitializeActors();

    void UpdateActors(float deltaTime);

    void AddActor(class Actor *actor);

    void RemoveActor(class Actor *actor);

    // Draw functions
    void AddDrawable(class DrawComponent *drawable);

    void RemoveDrawable(class DrawComponent *drawable);

    // Collider functions
    void AddCollider(class AABBColliderComponent *collider);

    void RemoveCollider(class AABBColliderComponent *collider);

    std::vector<class AABBColliderComponent *> &GetColliders() { return mColliders; }

    // Camera functions
    Vector2 &GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2 &position) { mCameraPos = position; };

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    // Level functions
    int **GetLevelData() const { return mLevelData; }

    // Texture functions
    SDL_Texture *LoadTexture(const std::string &texturePath);

private:
    // Game processing functions
    void ProcessInput();

    void UpdateGame();

    void UpdateCamera();

    void GenerateOutput();

    // Load the level from a CSV file as a 2D array
    int **LoadLevel(const std::string &fileName, int width, int height);

    void BuildLevel(int **levelData, int width, int height);

    // All the actors in the game
    // Updated by the actor
    std::vector<class Actor *> mActors;
    std::vector<class Actor *> mPendingActors;

    // All the draw components
    // Updated by the component
    std::vector<class DrawComponent *> mDrawables;

    // All the collision components
    // Updated by the component
    std::vector<class AABBColliderComponent *> mColliders;

    // SDL stuff
    SDL_Window *mWindow;
    SDL_Renderer *mRenderer;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    // Camera properties
    Vector2 mCameraPos;

    // Game-specific

    // Level data
    int **mLevelData;
    SDL_Texture *mBackground;
};
