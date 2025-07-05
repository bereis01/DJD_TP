#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "Utils/CSV.h"
#include "Utils/Random.h"
#include "Game.h"
#include "Actors/Actor.h"
#include "Actors/Tile.h"
#include "Actors/Cursor.h"
#include "Actors/Ally.h"
#include "Actors/Enemy.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "UIElements/UIScreen.h"
#include "UIElements/StatScreen.h"
#include "UIElements/ActionScreen.h"
#include "UIElements/AttackScreen.h"
#include "Effects/ParticleSystem.h"
#include "Audio/AudioSystem.h"

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr)
      , mRenderer(nullptr)
      , mLevelData(nullptr)
      , mTicksCount(0)
      , mIsRunning(true)
      , mUpdatingActors(false)
      , mWindowWidth(windowWidth)
      , mWindowHeight(windowHeight)
      , mCameraPos(Vector2::Zero)
      , mSelectedUnit(nullptr)
      , mTargetUnitIndex(-1) {
}

bool Game::Initialize() {
    // Initializes SDL video stuff
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    mWindow = SDL_CreateWindow("Aymr", 0, 0, mWindowWidth, mWindowHeight, 0);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // Initialize SDL true type font stuff
    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        SDL_Log("Failed to initialize SDL_mixer");
        return false;
    }

    // Initializes random number generator library
    Random::Init();

    // Initializes time counter
    mTicksCount = SDL_GetTicks();

    // Initializes particle system
    mParticleSystem = new ParticleSystem(this, "../Assets/Fonts/DogicaPixel.ttf");

    // Initializes audio system
    mAudio = new AudioSystem();

    // Starts the game
    SetGameScene(GameScene::Level1, TRANSITION_TIME, true);

    return true;
}

void Game::BuildLevel(int **levelData, int width, int height) {
    // Traverses the level data matrix, instantiating actors
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (levelData[i][j]) {
                case 0: {
                    Tile *tile = new Tile(this, "../Assets/Sprites/Blocks/Grass.png");
                    tile->SetPosition(Vector2(j * TILE_SIZE, i * TILE_SIZE));
                    break;
                }
                case 1: {
                    Tile *tile = new Tile(this, "../Assets/Sprites/Blocks/Water.png");
                    tile->SetPosition(Vector2(j * TILE_SIZE, i * TILE_SIZE));
                    break;
                }
                default:
                    break;
            }
        }
    }
}

int **Game::LoadLevel(const std::string &fileName, int width, int height) {
    // Loads background
    // mBackground = LoadTexture("../Assets/Sprites/Background.png");

    // Creates level matrix
    int **level = new int *[height];
    for (int i = 0; i < height; i++)
        level[i] = new int[width];

    // Opens csv file with level info
    std::ifstream file = std::ifstream(fileName);
    if (!file.is_open()) {
        SDL_Log("Failed to load level");
        return nullptr;
    }

    // Traverses each line, attributing it to the matrix
    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        std::vector<int> parsedLine = CSVHelper::Split(line);
        if (parsedLine.size() != width) {
            SDL_Log("Failed to load level");
            return nullptr;
        }

        for (int i = 0; i < width; i++)
            level[lineCount][i] = parsedLine[i];
        lineCount++;
    }

    // Integrity checks
    if (lineCount != height) {
        SDL_Log("Failed to load level");
        return nullptr;
    }

    return level;
}

void Game::RunLoop() {
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    // Processes events from SDL
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                break;
            case SDL_KEYDOWN: {
                int key = event.key.keysym.sym;
                bool isPressed = event.key.repeat == 0;

                // Changes turn if player has pressed E
                if (key == SDLK_e) {
                    if (mGamePlayState == GamePlayState::EnemyTurn)
                        SetGamePlayState(GamePlayState::Map);
                    else
                        SetGamePlayState(GamePlayState::EnemyTurn);
                    mTurnScreen->ChangeTurn();
                }

                // Handle key press for UI screens
                if (!mUIStack.empty()) {
                    if (mUIStack.back()->IsInteractive()) {
                        mUIStack.back()->HandleKeyPress(key);
                        break;
                    }
                }

                // Handles key presses for actors
                for (int i = 0; i < mActors.size(); i++)
                    mActors[i]->HandleKeyPress(key, isPressed);

                break;
            }
            default:
                break;
        }
    }

    // Gets the keyboard state and processes actor input
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    for (auto actor: mActors)
        actor->ProcessInput(state);
}

void Game::UpdateGame() {
    // Locks framerate to 60
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    // Delta time equals time past from the last frame
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // Updates time count
    mTicksCount = SDL_GetTicks();

    // Updates UI elements
    UpdateUI(deltaTime);

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera(deltaTime);

    // Updates the scene
    UpdateSceneManager(deltaTime);

    // Updates the turn
    UpdateTurn(deltaTime);

    // Updates audio
    mAudio->Update(deltaTime);

    // Checks victory/defeat
    CheckVictory();
}

void Game::CheckVictory() {
    if (mEnemies.size() == 0)
        SetGamePlayState(GamePlayState::LevelComplete);
    if (mUnits.size() == 0)
        SetGamePlayState(GamePlayState::LevelFailed);
}

void Game::UpdateTurn(float deltaTime) {
    if (mGamePlayState != GamePlayState::EnemyTurn) {
        // Checks if all units have been used
        for (auto unit: mUnits)
            if (unit->IsAvailable())
                return;

        // If no units are available, change to the enemy turn
        SetGamePlayState(GamePlayState::EnemyTurn);
        mTurnScreen->ChangeTurn();
        mCurrentEnemyIndex = mEnemies.size() - 1;

        // Plays turn changing sound
        mAudio->PlaySound("EnemyTurn.ogg");
    } else {
        // Traverses the enemies, activating each one
        if (mCurrentEnemyIndex >= 0) {
            if (mEnemies[mCurrentEnemyIndex]->GetEnemyState() == Enemy::EnemyState::None)
                mEnemies[mCurrentEnemyIndex]->SetEnemyState(Enemy::EnemyState::Moving);
            else if (mEnemies[mCurrentEnemyIndex]->GetEnemyState() == Enemy::EnemyState::Finished) {
                mEnemies[mCurrentEnemyIndex]->SetEnemyState(Enemy::EnemyState::None);
                mCurrentEnemyIndex--;
            } else if (mEnemies[mCurrentEnemyIndex]->GetEnemyState() == Enemy::EnemyState::Dead) {
                mEnemies[mCurrentEnemyIndex]->SetEnemyState(Enemy::EnemyState::None);
                mEnemies[mCurrentEnemyIndex]->SetState(ActorState::Destroy);
                mCurrentEnemyIndex--;
            }
        }
        // When all have been activated, returns the turn to the player
        else {
            // Resets availability of units
            for (auto unit: mUnits)
                unit->SetAvailable(true);

            SetGamePlayState(GamePlayState::Map);
            mTurnScreen->ChangeTurn();

            // Plays turn changing sound
            mAudio->PlaySound("PlayerTurn.ogg");
        }
    }
}

void Game::UpdateUI(float deltaTime) {
    // Updates UI screens
    for (auto ui: mUIStack) {
        if (ui->GetState() == UIScreen::UIState::Active) {
            ui->Update(deltaTime);
        }
    }

    // Delete any UIElements that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::UIState::Closing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Game::UpdateCamera(float deltaTime) {
    // Makes the camera move as the cursor touches the edges
    if (mCursor) {
        Vector2 pos = mCursor->GetPosition();
        if (pos.x - mCameraPos.x > (mWindowWidth - 2 * TILE_SIZE))
            if (mCameraPos.x + mWindowWidth < LEVEL_WIDTH * TILE_SIZE)
                mCameraPos.x += TILE_SIZE;
        if (pos.x < mCameraPos.x + TILE_SIZE)
            if (mCameraPos.x > 0)
                mCameraPos.x -= TILE_SIZE;
        if (pos.y - mCameraPos.y > (mWindowHeight - 2 * TILE_SIZE))
            if (mCameraPos.y + mWindowHeight < LEVEL_HEIGHT * TILE_SIZE)
                mCameraPos.y += TILE_SIZE;
        if (pos.y < mCameraPos.y + TILE_SIZE)
            if (mCameraPos.y > 0)
                mCameraPos.y -= TILE_SIZE;
    }
}

void Game::UpdateActors(float deltaTime) {
    // Updates all actors
    mUpdatingActors = true;
    for (auto actor: mActors) {
        actor->Update(deltaTime);
    }
    mUpdatingActors = false;

    // Puts newly created actors to the list
    for (auto pending: mPendingActors) {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // Puts destroyed actors to the dead list
    std::vector<Actor *> deadActors;
    for (auto actor: mActors) {
        if (actor->GetState() == ActorState::Destroy) {
            deadActors.emplace_back(actor);
        }
    }

    // Deletes dead actors
    for (auto actor: deadActors) {
        delete actor;
    }
}

void Game::AddActor(Actor *actor) {
    // If in the middle of updating, adds to pending
    // Else, puts directly on actors list
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    } else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor *actor) {
    // Searches in the pending list and removes if there
    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    // Searches in the normal list and removes if there
    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        // Swap to end of vector and pop off (avoid erase copies)
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddDrawable(class DrawComponent *drawable) {
    // Adds drawable to the list and sorts by draw order
    mDrawables.emplace_back(drawable);
    std::sort(mDrawables.begin(), mDrawables.end(), [](DrawComponent *a, DrawComponent *b) {
        return a->GetDrawOrder() < b->GetDrawOrder();
    });
}

void Game::RemoveDrawable(class DrawComponent *drawable) {
    auto iter = std::find(mDrawables.begin(), mDrawables.end(), drawable);
    mDrawables.erase(iter);
}

void Game::AddCollider(class AABBColliderComponent *collider) {
    mColliders.emplace_back(collider);
}

void Game::RemoveCollider(AABBColliderComponent *collider) {
    auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
    mColliders.erase(iter);
}

void Game::GenerateOutput() {
    // Set draw color to blue
    SDL_SetRenderDrawColor(mRenderer, 107, 140, 255, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draws background texture considering camera position
    if (mBackground) {
        SDL_Rect dstRect = {
            static_cast<int>(-mCameraPos.x),
            static_cast<int>(-mCameraPos.y),
            static_cast<int>(LEVEL_WIDTH * TILE_SIZE),
            static_cast<int>(LEVEL_HEIGHT * TILE_SIZE)
        };
        SDL_RenderCopy(mRenderer, mBackground, nullptr, &dstRect);
    }

    // Draws drawable components
    for (auto drawable: mDrawables) {
        if (drawable->IsVisible()) {
            drawable->Draw(mRenderer);
        }
    }

    // Draws all UI screens
    for (auto ui: mUIStack) {
        ui->Draw(mRenderer);
    }

    // Draws gray grid lines if moving a unit
    if (mGamePlayState == GamePlayState::MovingUnit) {
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 50);
        for (int x = 0; x <= mWindowWidth;
             x += TILE_SIZE) {
            SDL_RenderDrawLine(mRenderer, x, 0, x, mWindowHeight);
        }
        for (int y = 0; y <= mWindowHeight;
             y += TILE_SIZE) {
            SDL_RenderDrawLine(mRenderer, 0, y, mWindowWidth, y);
        }
    }

    // Draws red grid lines if moving a unit
    if (mGamePlayState == GamePlayState::ChoosingTarget) {
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 50);
        for (int x = 0; x <= mWindowWidth;
             x += TILE_SIZE) {
            SDL_RenderDrawLine(mRenderer, x, 0, x, mWindowHeight);
        }
        for (int y = 0; y <= mWindowHeight;
             y += TILE_SIZE) {
            SDL_RenderDrawLine(mRenderer, 0, y, mWindowWidth, y);
        }
    }

    // Cross-fade between scenes
    if (mSceneManagerState == SceneManagerState::Exiting && mSceneManagerTimer <= TRANSITION_TIME) {
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, (TRANSITION_TIME - mSceneManagerTimer) * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);
    } else if (mSceneManagerState == SceneManagerState::Entering && mSceneManagerTimer <= TRANSITION_TIME) {
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, mSceneManagerTimer * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);
    }

    // Victory screen
    if (mGamePlayState == GamePlayState::LevelComplete) {
        // Victory text
        auto VictoryText = new UIScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        VictoryText->AddText("VICTORY", Vector2(225, 270), Vector2(350, 100));
        mUIStack.emplace_back(VictoryText);

        // Blue background
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 0.5 * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);
    }

    // Fail screen
    if (mGamePlayState == GamePlayState::LevelFailed) {
        // Defeat text
        auto DefeatText = new UIScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        DefeatText->AddText("DEFEAT", Vector2(250, 270), Vector2(300, 100));
        mUIStack.emplace_back(DefeatText);

        // Red background
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 0.5 * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);
    }

    // Swap front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}

SDL_Texture *Game::LoadTexture(const std::string &texturePath) {
    // Loads surface from file
    SDL_Surface *surface = IMG_Load(texturePath.c_str());
    if (surface == nullptr) {
        SDL_Log("Failed to load surface: %s", texturePath.c_str());
        return nullptr;
    }

    // Loads texture from surface
    SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr) {
        SDL_Log("Failed to load texture");
        return nullptr;
    }

    return texture;
}

UIFont *Game::LoadFont(const std::string &fileName) {
    // Checks if font is already loaded
    // Just returns it if so
    auto it = mFonts.find(fileName);
    if (it != mFonts.end())
        return it->second;

    // Loads the font
    UIFont *font = new UIFont(mRenderer);
    bool result = font->Load(fileName);
    if (!result) {
        font->Unload();
        delete font;
        return nullptr;
    }

    // Stores and returns it
    mFonts.emplace(fileName, font);
    return font;
}

void Game::Shutdown() {
    // Unload current Scene
    UnloadScene();

    // Deletes particle system
    delete mParticleSystem;

    // Deletes audio system
    delete mAudio;
    Mix_CloseAudio();

    // Deletes loaded fonts
    for (auto font: mFonts) {
        font.second->Unload();
        delete font.second;
    }
    mFonts.clear();

    // Closes SDL stuff
    TTF_Quit();
    IMG_Quit();

    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::UpdateSceneManager(float deltaTime) {
    // Timer for exiting the scene
    if (mSceneManagerState == SceneManagerState::Exiting) {
        mSceneManagerTimer -= deltaTime;
        if (mSceneManagerTimer <= 0) {
            mSceneManagerTimer = TRANSITION_TIME;
            mSceneManagerState = SceneManagerState::Change;
        }
    }

    // Effectively changes the scene
    if (mSceneManagerState == SceneManagerState::Change) {
        mSceneManagerState = SceneManagerState::Entering;
        ChangeScene();
    }

    // Timer for entering a new scene
    if (mSceneManagerState == SceneManagerState::Entering) {
        mSceneManagerTimer -= deltaTime;
        if (mSceneManagerTimer <= 0) {
            mSceneManagerState = SceneManagerState::Active;
        }
    }
}

void Game::ChangeScene() {
    // Unload current Scene
    UnloadScene();

    // Reset camera position
    mCameraPos.Set(0.0f, 0.0f);

    // Scene Manager FSM: using if/else instead of switch
    if (mNextScene == GameScene::Level1) {
        // Loads first level
        mLevelData = LoadLevel("../Assets/Levels/Level1_Base.csv", LEVEL_WIDTH, LEVEL_HEIGHT);

        // Checks if loading was successful and builds the level
        if (mLevelData == nullptr) {
            SDL_Log("Failed initializing level");
            return;
        }

        // Loads cursor
        mCursor = new Cursor(this, "../Assets/Sprites/Cursor.png");
        mCursor->SetXY(20, 8);

        // Loads units (with stats and weapons)
        Stats s = Stats("Mia", 30, 30, 9, 4, 12, 20, 5, 5, 6);
        Weapon *w1 = new Weapon("Wo dao", 90, 7, 20, 1);
        Weapon *w2 = new Weapon("Steel sword", 85, 9, 0, 1);
        Weapon *w3 = new Weapon("Silver sword", 90, 13, 0, 1);
        mTrueblade = new Ally(this, "TrueBlade", s);
        mTrueblade->SetXY(20, 8);
        mTrueblade->SetStats(s);
        mTrueblade->AddWeapon(w1);
        mTrueblade->AddWeapon(w2);
        mTrueblade->AddWeapon(w3);
        mTrueblade->AddItem("Healing potion");
        //mTrueblade->SetEquippedWeapon(w1);
        mUnits.emplace_back(mTrueblade);

        s = Stats("Ilyana", 20, 20, 3, 10, 10, 8, 3, 9, 5);
        w1 = new Weapon("Thunder", 80, 5, 10, 2, true);
        w2 = new Weapon("Fire", 90, 6, 0, 2, true);
        mSorceress = new Ally(this, "Wizard", s);
        mSorceress->SetXY(21, 8);
        mSorceress->SetStats(s);
        mSorceress->AddWeapon(w1);
        mSorceress->AddWeapon(w2);
        //mSorceress->SetEquippedWeapon(w1);
        mUnits.emplace_back(mSorceress);

        // Loads enemies
        Stats ss = Stats("Enemy1", 25, 25, 8, 4, 6, 6, 3, 0, 4);
        Weapon *w = new Weapon("Iron Sword", 90, 6, 0, 1);
        Enemy *enemy = new Enemy(this, "Orc", ss);
        enemy->SetXY(19, 8);
        enemy->SetStats(ss);
        enemy->AddWeapon(w);
        //enemy->SetEquippedWeapon(w);
        mEnemies.emplace_back(enemy);

        ss = Stats("Enemy2", 25, 25, 8, 4, 6, 6, 3, 0, 4);
        w = new Weapon("Iron Sword", 90, 6, 0, 1);
        Enemy *enemy2 = new Enemy(this, "Orc", ss);
        enemy2->SetXY(15, 13);
        enemy2->SetStats(ss);
        enemy2->AddWeapon(w);
        //enemy2->SetEquippedWeapon(w);
        mEnemies.emplace_back(enemy2);

        ss = Stats("Enemy3", 25, 25, 8, 4, 6, 6, 3, 0, 4);
        w = new Weapon("Iron Sword", 90, 6, 0, 1);
        Enemy *enemy3 = new Enemy(this, "Orc", ss);
        enemy3->SetXY(13, 15);
        enemy3->SetStats(ss);
        enemy3->AddWeapon(w);
        //enemy3->SetEquippedWeapon(w);
        mEnemies.emplace_back(enemy3);

        ss = Stats("Enemy4", 25, 25, 8, 4, 6, 6, 3, 0, 4);
        w = new Weapon("Iron Sword", 90, 6, 0, 1);
        Enemy *enemy4 = new Enemy(this, "Orc", ss);
        enemy4->SetXY(13, 13);
        enemy4->SetStats(ss);
        enemy4->AddWeapon(w);
        //enemy4->SetEquippedWeapon(w);
        mEnemies.emplace_back(enemy4);

        // Loads background image
        mBackground = LoadTexture("../Assets/Levels/Level1.png");

        // Sets game state
        SetGamePlayState(GamePlayState::Map);

        // Loads HUD
        mStatScreen = new StatScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mActionScreen = new ActionScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mTurnScreen = new TurnScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mAttackScreen = new AttackScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mItemScreen = new ItemScreen(this, "../Assets/Fonts/SuperVCR.ttf");

        // Plays music
        mAudio->PlaySound("Level1.ogg", true);
    }

    // Set new scene
    mGameScene = mNextScene;
}

void Game::SetGameScene(GameScene scene, float transitionTime, bool fastStart) {
    // Sanity checks
    if (scene != GameScene::MainMenu && scene != GameScene::Level1 && scene != GameScene::Level2 && scene !=
        GameScene::Level3 && scene != GameScene::Shop) {
        SDL_Log("Failed to set game scene!");
        return;
    }

    // Sets the next scene
    mNextScene = scene;
    mSceneManagerState = fastStart ? SceneManagerState::Change : SceneManagerState::Exiting;
    mSceneManagerTimer = transitionTime;
}

void Game::ResetGameScene(float transitionTime) {
    // Sets the game to the current scene
    SetGameScene(mGameScene, transitionTime);
}

void Game::UnloadScene() {
    // Delete actors
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Delete UI screens
    //for (auto ui: mUIStack)
    //delete ui;
    delete mStatScreen;
    delete mAttackScreen;
    delete mActionScreen;
    delete mItemScreen;
    delete mTurnScreen;
    mUIStack.clear();

    // Delete background texture
    if (mBackground) {
        SDL_DestroyTexture(mBackground);
        mBackground = nullptr;
    }

    // Delete level data (deallocates matrix)
    if (mLevelData) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            if (mLevelData[i] != nullptr)
                delete[] mLevelData[i];
        }
        delete[] mLevelData;
        mLevelData = nullptr;
    }
}

Unit *Game::GetUnitByPosition(int x, int y) {
    // Searches the ally vector
    for (auto un: mUnits) {
        if (un->GetX() == x && un->GetY() == y) {
            return un;
        }
    }

    // Searches the enemy vector
    for (auto un: mEnemies) {
        if (un->GetX() == x && un->GetY() == y) {
            return un;
        }
    }

    return nullptr;
}

Ally *Game::GetAllyByPosition(int x, int y) {
    // Searches the ally vector
    for (auto un: mUnits) {
        if (un->GetX() == x && un->GetY() == y) {
            return un;
        }
    }

    return nullptr;
}

Enemy *Game::GetEnemyByPosition(int x, int y) {
    // Searches the enemy vector
    for (auto en: mEnemies) {
        if (en->GetX() == x && en->GetY() == y) {
            return en;
        }
    }

    return nullptr;
}

void Game::RemoveAlly(Ally *ally) {
    // Searches the enemy vector for enemies
    auto iter = std::find(mUnits.begin(), mUnits.end(), ally);
    mUnits.erase(iter);
}

void Game::RemoveEnemy(Enemy *enemy) {
    // Searches the enemy vector for enemies
    auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
    mEnemies.erase(iter);
}

void Game::SetEnemiesInRange() {
    mEnemiesInRange.clear();
    int unitX = mSelectedUnit->GetX();
    int unitY = mSelectedUnit->GetY();
    int range = mSelectedUnit->GetEquippedWeapon()->range;
    Enemy *target;
    for (int radius = 1; radius <= range; radius++) {
        for (int offset = 0; offset < radius; offset++) {
            int invOffset = radius - offset;

            int x = unitX + offset;
            int y = unitY + invOffset;
            target = GetEnemyByPosition(x, y);
            if (target != nullptr) {
                mEnemiesInRange.push_back(target);
            }

            x = unitX + invOffset;
            y = unitY - offset;
            target = GetEnemyByPosition(x, y);
            if (target != nullptr) {
                mEnemiesInRange.push_back(target);
            }

            x = unitX - offset;
            y = unitY - invOffset;
            target = GetEnemyByPosition(x, y);
            if (target != nullptr) {
                mEnemiesInRange.push_back(target);
            }

            x = unitX - invOffset;
            y = unitY + offset;
            target = GetEnemyByPosition(x, y);
            if (target != nullptr) {
                mEnemiesInRange.push_back(target);
            }
        }
    }
}

void Game::SetupAttack() {
    SetEnemiesInRange();
    if (!mEnemiesInRange.empty()) {
        mTargetUnitIndex = 0;
        mCursor->SetPosition(mEnemiesInRange[0]->GetPosition());
    }
    SetGamePlayState(GamePlayState::ChoosingTarget);
    mUIStack.pop_back();
}

void Game::ShowItems() {
    if (mGamePlayState == GamePlayState::ChoosingAction) {
        PopUI();
        PushUI(mItemScreen);
        SetGamePlayState(GamePlayState::OnInventory);
        mItemScreen->SetupDisplay(mSelectedUnit);
    } else {
        mItemScreen->SetupDisplay(mSelectedUnit);
    }
}
