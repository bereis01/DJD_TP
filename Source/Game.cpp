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
#include "UIElements/MenuScreen.h"
#include "UIElements/ShopScreen.h"

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
    SetGameScene(GameScene::MainMenu, TRANSITION_TIME, true);
    //SetGameScene(GameScene::Level1, TRANSITION_TIME, true);

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
                    if (mGamePlayState == GamePlayState::Map) {
                        for (auto unit: mUnits) {
                            unit->SetAvailable(false);
                            mTurnScreen->ChangeTurn();
                        }
                    }
                }
                if (key == SDLK_RETURN) {
                    if (mGamePlayState == GamePlayState::LevelComplete) {
                        mShopScreen = new ShopScreen(this, "../Assets/Fonts/SuperVCR.ttf", 2);
                        mGamePlayState = GamePlayState::Shopping;
                        PopUI();
                        PushUI(mShopScreen);

                        // Plays audio
                        mAudio->PlaySound("Store.ogg");

                        break;
                    }
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

    // Updates the scene
    UpdateSceneManager(deltaTime);

    // Updates audio
    mAudio->Update(deltaTime);

    if (mGameScene != GameScene::MainMenu) {
        // Update camera position
        UpdateCamera(deltaTime);

        // Updates the turn
        UpdateTurn(deltaTime);

        // Checks victory/defeat
        if (mGamePlayState != GamePlayState::LevelComplete && mGamePlayState != GamePlayState::LevelFailed &&
            mGamePlayState != GamePlayState::Shopping)
            CheckVictory();
    }
}

void Game::CheckVictory() {
    if (mEnemies.size() == 0) {
        SetGamePlayState(GamePlayState::LevelComplete);

        // Plays audio
        mAudio->PlaySound("Victory.ogg");
    }

    if (mUnits.size() == 0) {
        SetGamePlayState(GamePlayState::LevelFailed);

        // Plays audio
        mAudio->PlaySound("Defeat.ogg");
    }
}

void Game::UpdateTurn(float deltaTime) {
    if (mGamePlayState == GamePlayState::Map) {
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
    } else if (mGamePlayState == GamePlayState::EnemyTurn) {
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
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draws background texture considering camera position
    if (mBackground) {
        SDL_Rect dstRect = {0, 0, 0, 0};
        if (mGameScene == GameScene::MainMenu) {
            dstRect = {0, 0, mWindowWidth, mWindowHeight};
            SDL_SetTextureBlendMode(mBackground, SDL_BLENDMODE_BLEND);
            SDL_SetTextureAlphaMod(mBackground, 150);
        } else {
            dstRect = {
                static_cast<int>(-mCameraPos.x),
                static_cast<int>(-mCameraPos.y),
                LEVEL_WIDTH * TILE_SIZE,
                LEVEL_HEIGHT * TILE_SIZE
            };
        }
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
        // Fade background
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 0.25 * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);

        // Victory text
        mLevelFinishedScreen = new UIScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mLevelFinishedScreen->AddImage("../Assets/UI/Victory.png",
                                       Vector2(((GetWindowWidth() - 640) / 2),
                                               ((GetWindowHeight() - 360) / 2)),
                                       Vector2(640, 360));
        std::string instructions = "Press [ENTER] to continue";
        mLevelFinishedScreen->AddText(instructions,
                                      Vector2(((GetWindowWidth() - (10.0f * instructions.size())) / 2),
                                              ((GetWindowHeight() - 15.0f) / 2) + 100),
                                      Vector2(10.0f * instructions.size(), 15.0f));
        mUIStack.emplace_back(mLevelFinishedScreen);
    }

    // Fail screen
    if (mGamePlayState == GamePlayState::LevelFailed) {
        // Fade background
        SDL_Rect drawRect = {0, 0, GetWindowWidth(), GetWindowHeight()};
        SDL_SetRenderDrawColor(mRenderer, 255, 0, 0, 0.25 * 255);
        SDL_SetRenderDrawBlendMode(mRenderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(mRenderer, &drawRect);

        // Defeat text
        mLevelFinishedScreen = new UIScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mLevelFinishedScreen->AddImage("../Assets/UI/Defeat.png",
                                       Vector2(((GetWindowWidth() - 640) / 2),
                                               ((GetWindowHeight() - 360) / 2)),
                                       Vector2(640, 360));
        std::string instructions = "Press [ENTER] to restart";
        mLevelFinishedScreen->AddText(instructions,
                                      Vector2(((GetWindowWidth() - (10.0f * instructions.size())) / 2),
                                              ((GetWindowHeight() - 15.0f) / 2) + 100),
                                      Vector2(10.0f * instructions.size(), 15.0f));
        mUIStack.emplace_back(mLevelFinishedScreen);
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

    // Deletes actors
    while (!mActors.empty()) {
        delete mActors.back();
    }

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
    // Main Menu
    if (mNextScene == GameScene::MainMenu) {
        // Loads background image
        mBackground = LoadTexture("../Assets/UI/MenuBackground.png");

        // Shows title
        mParticleSystem->CreateTitleParticle("Title", 10.0f, 5.0f, -1.0f, false);

        // Plays music
        mMusic = mAudio->PlaySound("Menu.ogg", true);

        // Shows menu buttons
        mMenuScreen = new MenuScreen(this, "../Assets/Fonts/SuperVCR.ttf");
        mUIStack.emplace_back(mMenuScreen);
    }
    // First phase
    else if (mNextScene == GameScene::Level1) {
        // Loads first level
        mLevelData = LoadLevel("../Assets/Levels/Level1_Base.csv", LEVEL_WIDTH, LEVEL_HEIGHT);

        // Checks if loading was successful and builds the level
        if (mLevelData == nullptr) {
            SDL_Log("Failed initializing level");
            return;
        }

        // Loads cursor
        mCursor = new Cursor(this, "../Assets/Sprites/Cursor.png");
        mCursor->SetXY(20, 16);

        // Adjusts camera position
        mCameraPos = Vector2(64, 128);

        // Loads units (with stats and weapons)
        Stats s = Stats("Mia", 30, 30, 9, 4, 12, 20, 5, 5, 6);
        Weapon *w = new Weapon("Iron sword", 90, 6, 0, 1);
        mTrueblade = new Ally(this, "TrueBlade", s);
        mTrueblade->SetXY(20, 16);
        mTrueblade->SetStats(s);
        mTrueblade->AddWeapon(w);
        mTrueblade->AddItem("Healing potion");
        mUnits.emplace_back(mTrueblade);

        s = Stats("Marcia", 25, 25, 9, 7, 10, 15, 4, 9, 7);
        w = new Weapon("Iron lance", 85, 7, 0, 1);
        mPegasusKnight = new Ally(this, "Pegasus", s);
        mPegasusKnight->SetXY(20, 12);
        mPegasusKnight->SetStats(s);
        mPegasusKnight->AddWeapon(w);
        mPegasusKnight->SetFlyer(true);
        mPegasusKnight->AddItem("Healing gem");
        mUnits.emplace_back(mPegasusKnight);

        s = Stats("Hubert", 20, 20, 3, 10, 10, 8, 3, 11, 5);
        w = new Weapon("Thunder", 80, 5, 10, 2, true);
        mMage = new Ally(this, "Wizard", s);
        mMage->SetXY(21, 12);
        mMage->SetStats(s);
        mMage->AddWeapon(w);
        mMage->AddItem("Healing potion");
        mUnits.emplace_back(mMage);

        s = Stats("Ferdinand", 35, 35, 12, 2, 10, 5, 10, 5, 5);
        w = new Weapon("Iron axe", 80, 8, 0, 1);
        mWarrior = new Ally(this, "Warrior", s);
        mWarrior->SetXY(21, 17);
        mWarrior->SetStats(s);
        mWarrior->AddWeapon(w);
        mWarrior->AddItem("Healing potion");
        mWarrior->AddItem("Healing gem");
        mUnits.emplace_back(mWarrior);

        // Loads enemies
        Stats ss = Stats("Orc", 20, 20, 8, 4, 6, 6, 3, 0, 4);
        for (int i = 0; i < 1; i++) {
            Enemy *enemy = new Enemy(this, "Orc", ss);
            w = new Weapon("Iron Sword", 90, 6, 0, 1);
            enemy->SetStats(ss);
            enemy->AddWeapon(w);
            mEnemies.emplace_back(enemy);
        }
        mEnemies[0]->SetXY(19, 16);
        mEnemies[0]->SetCurrentHp(1);
        /*
        mEnemies[1]->SetXY(6, 13);
        mEnemies[2]->SetXY(13, 13);
        mEnemies[3]->SetXY(13, 10);
        mEnemies[4]->SetXY(12, 9);
        mEnemies[5]->SetXY(14, 9);
        mEnemies[6]->SetXY(21, 8);
        mEnemies[7]->SetXY(21, 7);
        mEnemies[8]->SetXY(21, 14);
        mEnemies[9]->SetXY(17, 16);
        mEnemies[10]->SetXY(17, 17);
        mEnemies[11]->SetXY(15, 20);
        mEnemies[11]->SetXY(16, 21);
        */

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
        mMusic = mAudio->PlaySound("Level1.ogg", true);

        // Shows title
        mParticleSystem->CreateTitleParticle("Level1");
        mParticleSystem->CreateTitleParticle("Instructions", 5, 1, 1, true, Vector2(0, 100));
    } else if (mNextScene == GameScene::Level2) {
        // Loads first level
        mLevelData = LoadLevel("../Assets/Levels/Level2_Base.csv", LEVEL_WIDTH, LEVEL_HEIGHT);

        // Checks if loading was successful and builds the level
        if (mLevelData == nullptr) {
            SDL_Log("Failed initializing level");
            return;
        }

        // Adjusts cursor
        mCursor->SetXY(27, 22);

        // Adjusts camera
        mCameraPos = Vector2(64, 128);

        // Reloads units from last level
        for (auto unit: mUnits) {
            unit->SetAvailable(true);
            unit->SetCurrentHp(unit->GetStats().hp);
        }
        mTrueblade->SetXY(27, 22);
        mPegasusKnight->SetXY(28, 21);
        mWarrior->SetXY(27, 21);
        mMage->SetXY(28, 22);

        // Loads enemies
        Weapon *w = nullptr;
        Stats ss = Stats("Orc", 20, 20, 8, 4, 6, 6, 3, 0, 4);
        for (int i = 0; i < 27; i++) {
            Enemy *enemy = new Enemy(this, "Orc", ss);
            w = new Weapon("Iron Sword", 90, 6, 0, 1);
            enemy->SetStats(ss);
            enemy->AddWeapon(w);
            mEnemies.emplace_back(enemy);
        }
        mEnemies[0]->SetXY(1, 11); // Boss, need to adjust stats
        mEnemies[1]->SetXY(1, 12);
        mEnemies[2]->SetXY(2, 11);
        mEnemies[3]->SetXY(2, 16);
        mEnemies[4]->SetXY(6, 8);
        mEnemies[5]->SetXY(5, 9);
        mEnemies[6]->SetXY(4, 20);
        mEnemies[7]->SetXY(7, 20);
        mEnemies[8]->SetXY(7, 21);
        mEnemies[9]->SetXY(7, 22);
        mEnemies[10]->SetXY(9, 9);
        mEnemies[11]->SetXY(10, 8);
        mEnemies[12]->SetXY(11, 12);
        mEnemies[13]->SetXY(12, 12);
        mEnemies[14]->SetXY(11, 17);
        mEnemies[15]->SetXY(17, 15);
        mEnemies[16]->SetXY(19, 14);
        mEnemies[17]->SetXY(19, 13);
        mEnemies[18]->SetXY(20, 13);
        mEnemies[19]->SetXY(14, 21);
        mEnemies[20]->SetXY(15, 22);
        mEnemies[21]->SetXY(24, 7);
        mEnemies[22]->SetXY(24, 8);
        mEnemies[23]->SetXY(27, 15);
        mEnemies[24]->SetXY(28, 14);
        mEnemies[25]->SetXY(23, 20);
        mEnemies[26]->SetXY(19, 21);

        // Loads background image
        mBackground = LoadTexture("../Assets/Levels/Level2.png");

        // Sets game state
        SetGamePlayState(GamePlayState::Map);

        // Loads HUD
        PushUI(mTurnScreen);

        // Plays music
        mMusic = mAudio->PlaySound("Level2.ogg", true);

        // Shows title
        mParticleSystem->CreateTitleParticle("Level2");
        mParticleSystem->CreateTitleParticle("Instructions", 5, 1, 1, true, Vector2(0, 100));
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

    // Stops music from previous scene
    if (mNextScene != GameScene::MainMenu)
        mAudio->StopSound(mMusic);
}

void Game::ResetGameScene(float transitionTime) {
    // Sets the game to the current scene
    SetGameScene(mGameScene, transitionTime);
}

void Game::UnloadScene() {
    // Delete actors and UI screens
    if (mGameScene == GameScene::MainMenu) {
        while (!mActors.empty()) {
            delete mActors.back();
        }

        delete mStatScreen;
        delete mAttackScreen;
        delete mActionScreen;
        delete mItemScreen;
        delete mTurnScreen;
    }

    // Clears UI screen stack
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

void Game::GoToExpScreen() {
    mLevelupScreen = new LevelupScreen(this, "../Assets/Fonts/SuperVCR.ttf");
    PopUI();
    PushUI(mLevelupScreen);

    // Plays audio
    mAudio->PlaySound("PowerUp.ogg");
}
