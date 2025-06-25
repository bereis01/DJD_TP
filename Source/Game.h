#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Utils/Math.h"

class Game {
public:
    enum class GameScene {
        MainMenu,
        Level1,
        Level2,
        Level3,
        Shop
    };

    enum class SceneManagerState {
        None,
        Entering,
        Active,
        Exiting,
        Change
    };

    static const int LEVEL_WIDTH = 30; // In tiles
    static const int LEVEL_HEIGHT = 30; // In tiles
    static const int TILE_SIZE = 32; // In pixels

    static const int TRANSITION_TIME = 1; // Scene transition time in seconds

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
    int GetLevelData(const int x, const int y) const { return mLevelData[x][y]; }

    // Texture functions
    SDL_Texture *LoadTexture(const std::string &texturePath);

    // Scene management functions
    void SetGameScene(GameScene scene, float transitionTime = TRANSITION_TIME, bool fastStart = false);

    GameScene GetGameScene() const { return mGameScene; }

    void ResetGameScene(float transitionTime = TRANSITION_TIME);

    void UnloadScene();

private:
    // Game processing functions
    void ProcessInput();

    void UpdateGame();

    void UpdateCamera();

    void GenerateOutput();

    // Load the level from a CSV file as a 2D array
    int **LoadLevel(const std::string &fileName, int width, int height);

    void BuildLevel(int **levelData, int width, int height);

    // Scene management functions
    void UpdateSceneManager(float deltaTime);

    void ChangeScene();

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
    SDL_Window *mWindow = nullptr;
    SDL_Renderer *mRenderer = nullptr;

    // Window properties
    int mWindowWidth;
    int mWindowHeight;

    // Track elapsed time since game start
    Uint32 mTicksCount = 0;

    // Track if we're updating actors right now
    bool mIsRunning = true;
    bool mUpdatingActors = false;

    // Camera properties
    Vector2 mCameraPos = Vector2::Zero;

    // Game-specific
    class Cursor *mCursor = nullptr;
    class Unit *mKnight = nullptr;

    // Level data
    int **mLevelData = nullptr;
    SDL_Texture *mBackground = nullptr;

    // Scene management
    GameScene mGameScene = GameScene::MainMenu;
    GameScene mNextScene = GameScene::MainMenu;

    SceneManagerState mSceneManagerState = SceneManagerState::None;
    float mSceneManagerTimer = 0.0f;
};
