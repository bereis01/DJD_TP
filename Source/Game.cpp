#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <SDL_image.h>
#include "Utils/CSV.h"
#include "Utils/Random.h"
#include "Game.h"
#include "Actors/Actor.h"
#include "Actors/Tile.h"
#include "Actors/Cursor.h"
#include "Actors/Unit.h"
#include "Components/DrawComponents/DrawComponent.h"
#include "StatScreen.h"
#include "UIElements/UIScreen.h"

Game::Game(int windowWidth, int windowHeight)
    : mWindow(nullptr)
      , mRenderer(nullptr)
      , mLevelData(nullptr)
      , mTicksCount(0)
      , mIsRunning(true)
      , mUpdatingActors(false)
      , mWindowWidth(windowWidth)
      , mWindowHeight(windowHeight)
      , mCameraPos(Vector2::Zero) {
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

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // Initializes random number generator library
    Random::Init();

    // Init all game actors
    InitializeActors();

    // Loads cursor
    mCursor = new Cursor(this, "../Assets/Sprites/Cursor.png");
    mCursor->SetXY(20, 8);

    mKnight = new Unit(this, "../Assets/Sprites/Units/Knight.png");
    mKnight->SetXY(20, 8);
    Stats s = Stats("Edel", 30, 14, 4, 8, 9, 5);
    mKnight->SetStats(s);
    mUnits.emplace_back(mKnight);

    // Loads background image
    mBackground = LoadTexture("../Assets/Levels/Level1.png");

    mStatScreen = new StatScreen(this,"../Assets/Fonts/Arial.ttf");

    // Initializes time counter
    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::InitializeActors() {
    // Loads first level
    mLevelData = LoadLevel("../Assets/Levels/Level1_Base.csv", LEVEL_WIDTH, LEVEL_HEIGHT);

    // Checks if loading was successful and builds the level
    if (mLevelData == nullptr) {
        SDL_Log("Failed initializing level");
        return;
    }
    // BuildLevel(mLevelData, LEVEL_WIDTH, LEVEL_HEIGHT);
}

void Game::BuildLevel(int **levelData, int width, int height) {
    // Traverses the level data matrix, instantiating actors
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (levelData[i][j]) {
                case 0: {
                    //Tile *tile = new Tile(this, "../Assets/Sprites/Blocks/Grass.png");
                    //tile->SetPosition(Vector2(j * TILE_SIZE, i * TILE_SIZE));
                    break;
                }
                case 1: {
                    //Tile *tile = new Tile(this, "../Assets/Sprites/Blocks/Water.png");
                    //tile->SetPosition(Vector2(j * TILE_SIZE, i * TILE_SIZE));
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
            case SDL_KEYDOWN:
                // Handles key presses for actors
                if (!mUIStack.empty()) {
                    mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
                }
                HandleKeyPressActors(event.key.keysym.sym, event.key.repeat == 0);
                break;
        }
    }

    // Gets the keyboard state and processes actor input
    const Uint8 *state = SDL_GetKeyboardState(nullptr);
    for (auto actor: mActors)
        actor->ProcessInput(state);
}

void Game::HandleKeyPressActors(const int key, const bool isPressed)
{
    for (auto actor: mActors)
        actor->HandleKeyPress(key, isPressed);
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

    // Update all actors and pending actors
    UpdateActors(deltaTime);

    // Update camera position
    UpdateCamera();
    // Reinsert UI screens
    for (auto ui : mUIStack) {
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

void Game::UpdateCamera() {
    // Makes the camera move as the cursor touches the edges
    Vector2 pos = mCursor->GetPosition();
    while (pos.x - mCameraPos.x > (mWindowWidth - TILE_SIZE))
        mCameraPos.x += TILE_SIZE;
    while (pos.x < mCameraPos.x)
        mCameraPos.x -= TILE_SIZE;
    while (pos.y - mCameraPos.y > (mWindowHeight - TILE_SIZE))
        mCameraPos.y += TILE_SIZE;
    while (pos.y < mCameraPos.y)
        mCameraPos.y -= TILE_SIZE;
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

    for (auto ui :mUIStack) {
        ui->Draw(mRenderer);
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

UIFont* Game::LoadFont(const std::string& fileName)
{
    if (mFonts.find(fileName) != mFonts.end()) {
        return mFonts[fileName];
    }
    auto *font = new UIFont(mRenderer);
    if (font->Load(fileName)) {
        mFonts[fileName] = font;
        return mFonts[fileName];
    }
    font->Unload();
    delete font;
    return nullptr;
}

class Unit* Game::GetUnitByPosition(int x, int y) {
    for (auto un: mUnits) {
        if (un->GetX() == x && un->GetY() == y) {
            return un;
        }
    }
    return nullptr;
}

void Game::Shutdown() {
    // Delete actors
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Delete level data (deallocates matrix)
    if (mLevelData != nullptr) {
        for (int i = 0; i < LEVEL_HEIGHT; ++i) {
            if (mLevelData[i] != nullptr)
                delete[] mLevelData[i];
        }
    }
    delete[] mLevelData;

    // Closes SDL stuff
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
