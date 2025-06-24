#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "Utils/Math.h"

class Game {
public:
    static const int LEVEL_WIDTH = 30; // In tiles
    static const int LEVEL_HEIGHT = 30; // In tiles
    static const int TILE_SIZE = 32; // In pixels

    enum class GameScene
    {
        MainMenu,
        Level1,
        Level2,
        Level3,
        Shop
    };

    enum class SceneManagerState
    {
        None,
        Entering,
        Active,
        Exiting
    };

    enum class GamePlayState
    {
        Free,
        UnitSelected,
        ChoosingTarget,
        EnemyTurn,
        LevelComplete,
        Shopping
    };

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

    void HandleKeyPressActors(const int key, const bool isPressed);

    // Draw functions
    void AddDrawable(class DrawComponent *drawable);

    void RemoveDrawable(class DrawComponent *drawable);

    SDL_Renderer *GetRenderer() {return mRenderer;}

    // Collider functions
    void AddCollider(class AABBColliderComponent *collider);

    void RemoveCollider(class AABBColliderComponent *collider);

    std::vector<class AABBColliderComponent *> &GetColliders() { return mColliders; }

    // Camera functions
    Vector2 &GetCameraPos() { return mCameraPos; };
    void SetCameraPos(const Vector2 &position) { mCameraPos = position; };

    //UI functions
    void PushUI(class UIScreen* screen) { mUIStack.emplace_back(screen); }
    std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    class StatScreen* GetStatScreen() { return mStatScreen; }

    // Window functions
    int GetWindowWidth() const { return mWindowWidth; }
    int GetWindowHeight() const { return mWindowHeight; }

    // Loading functions
    class UIFont* LoadFont(const std::string& fileName);
    SDL_Texture* LoadTexture(const std::string& texturePath);

    void SetGameScene(GameScene scene, float transitionTime = .0f);
    GameScene GetGameScene() const { return mGameScene; }
    void ResetGameScene(float transitionTime = .0f);
    void UnloadScene();

    void SetBackgroundImage(const std::string& imagePath, const Vector2 &position = Vector2::Zero, const Vector2& size = Vector2::Zero);

    // Level functions
    int GetLevelData(const int x, const int y) const { return mLevelData[x][y]; }
    std::vector<class Unit*> GetUnits() {return mUnits;};
    class Unit *GetUnitByPosition(int x, int y);

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

    // All the UI elements
    std::vector<class UIScreen*> mUIStack;
    std::unordered_map<std::string, class UIFont*> mFonts;

    // Track elapsed time since game start
    Uint32 mTicksCount;

    // Track if we're updating actors right now
    bool mIsRunning;
    bool mUpdatingActors;

    // Camera properties
    Vector2 mCameraPos;

    // Game-specific
    class Cursor *mCursor;
    class Unit *mKnight;
    std::vector<class Unit *> mUnits;
    class StatScreen *mStatScreen;

    // Level data
    int **mLevelData;
    SDL_Texture *mBackground;

    // Track level state
    GameScene mGameScene;
    GameScene mNextScene;
};
