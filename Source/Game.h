#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <unordered_map>

#include "UIElements/ActionScreen.h"
#include "UIElements/TurnScreen.h"
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

    enum class GamePlayState {
        Map,
        ShowingStats,
        MovingUnit,
        ChoosingAction,
        ChoosingTarget,
        EnemyTurn,
        LevelComplete,
        Shopping
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

    void RemoveActor(Actor *actor);

    // Draw functions
    void AddDrawable(class DrawComponent *drawable);

    void RemoveDrawable(DrawComponent *drawable);

    // Collider functions
    void AddCollider(class AABBColliderComponent *collider);

    void RemoveCollider(AABBColliderComponent *collider);

    std::vector<AABBColliderComponent *> &GetColliders() { return mColliders; }

    // Camera functions
    Vector2 &GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2 &position) { mCameraPos = position; };

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    // Level functions
    int GetLevelData(const int x, const int y) const { return mLevelData[x][y]; }

    // Loading functions
    SDL_Texture *LoadTexture(const std::string &texturePath);

    class UIFont *LoadFont(const std::string &fileName);

    // Scene management functions
    void SetGameScene(GameScene scene, float transitionTime = TRANSITION_TIME, bool fastStart = false);

    GameScene GetGameScene() const { return mGameScene; }

    void ResetGameScene(float transitionTime = TRANSITION_TIME);

    void UnloadScene();

    // UI functions
    void PushUI(class UIScreen *screen) { mUIStack.emplace_back(screen); }
    std::vector<UIScreen *> &GetUIStack() { return mUIStack; }

    ActionScreen *GetActionScreen() { return mActionScreen; }
    class StatScreen *GetStatScreen() { return mStatScreen; }

    void LoadHUDScreens();

    // Renderer functions
    SDL_Renderer *GetRenderer() { return mRenderer; }

    // Game-specific
    std::vector<class Unit *> GetUnits() { return mUnits; };

    Unit *GetUnitByPosition(int x, int y);


    // TODO: Maybe move to cursor?
    void SetSelectedUnit(Unit *unit) { mSelectedUnit = unit; }
    Unit *GetSelectedUnit() const { return mSelectedUnit; }

    void SetTargetUnitIndex(int i) { mTargetUnitIndex = i; }
    int GetTargetUnitIndex() const { return mTargetUnitIndex; }

    void SetUnitsInRange();

    std::vector<class Unit *> GetUnitsInRange() { return mUnitsInRange; }

    void SetupAttack();

    // Game state management
    void SetGamePlayState(GamePlayState state) { mGamePlayState = state; }
    GamePlayState GetGamePlayState() const { return mGamePlayState; }

private:
    // Game processing functions
    void ProcessInput();

    void UpdateGame();

    void GenerateOutput();

    // Camera functions
    void UpdateCamera(float deltaTime);

    // UI functions
    void UpdateUI(float deltaTime);

    // Load the level from a CSV file as a 2D array
    int **LoadLevel(const std::string &fileName, int width, int height);

    void BuildLevel(int **levelData, int width, int height);

    // Scene management functions
    void UpdateSceneManager(float deltaTime);

    void ChangeScene();

    // All the actors in the game
    // Updated by the actor
    std::vector<Actor *> mActors;
    std::vector<class Actor *> mPendingActors;

    // All the draw components
    // Updated by the component
    std::vector<DrawComponent *> mDrawables;

    // All the collision components
    // Updated by the component
    std::vector<AABBColliderComponent *> mColliders;

    // All the UI elements
    std::vector<UIScreen *> mUIStack;
    std::unordered_map<std::string, UIFont *> mFonts;

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

    // Game state
    GamePlayState mGamePlayState;

    // TODO move to constructor
    // Game-specific
    class Cursor *mCursor = nullptr;
    Unit *mTrueblade = nullptr;
    Unit *mKnight = nullptr;
    std::vector<Unit *> mUnits;
    std::vector<Unit *> mUnitsInRange;
    int mTargetUnitIndex;

    // TODO: Maybe move to cursor?
    class Unit *mSelectedUnit;

    // Level data
    int **mLevelData = nullptr;
    SDL_Texture *mBackground = nullptr;

    // Scene management
    GameScene mGameScene = GameScene::MainMenu;
    GameScene mNextScene = GameScene::MainMenu;

    SceneManagerState mSceneManagerState = SceneManagerState::None;
    float mSceneManagerTimer = 0.0f;

    // HUD
    StatScreen *mStatScreen;
    ActionScreen *mActionScreen;
    TurnScreen *mTurnScreen;

};
