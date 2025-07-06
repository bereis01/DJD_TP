#pragma once
#include <random>
#include <unordered_set>

#include "Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"

class Stats {
public:
    Stats(std::string name = "x", int hp = 0, int ch = 0, int str = 0, int mag = 0, int skl = 0, int spd = 0,
          int def = 0,
          int res = 0, int mv = 0);

    std::string name;
    int hp;
    int currHp;
    int str;
    int mag;
    int skl;
    int spd;
    int def;
    int res;
    int mov;
};

class Weapon {
public:
    Weapon(std::string name = "x", int acc = 0, int mt = 0, int crit = 0, int rng = 0, bool m = false);

    std::string name;
    int hit;
    int might;
    int criticalChance;
    int range;
    bool magic;
};

class Unit : public Actor {
public:
    Unit(Game *game, Stats stats, bool isEnemy = true, const std::string &unitType = "Knight");

    virtual ~Unit();

    // Manipulation based on coordinates
    void SetXY(int x, int y) { mPosition = Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE); };
    int GetX() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }

    // Stats manipulation
    void SetStats(Stats stats);

    Stats GetStats() { return mStats; }
    void SetCurrentHp(int hp) { mStats.currHp = hp; }
    void SetMovement(int mov) { mStats.mov = mov; }
    int GetMovement() { return mStats.mov; }
    void SetOldPosition(Vector2 oldPosition) { mOldPosition = oldPosition; }
    Vector2 GetOldPosition() { return mOldPosition; }

    // Weapon manipulation
    void AddWeapon(Weapon *weapon) { mWeapons.push_back(weapon); }

    void EquipWeapon(int pos);

    Weapon *GetEquippedWeapon() { return mWeapons.front(); }
    std::vector<Weapon *> GetAllWeapons() { return mWeapons; }

    // Item manipulation
    void AddItem(const std::string &item) { mItens.push_back(item); }
    std::vector<std::string> GetAllItens() { return mItens; }

    void UseItem(const std::string &item);

    // In-game actions
    void ShowStats();

    void SetMovementRange();

    void ClearMovementRange() {mMovementRange.clear();}

    bool MovementIsInRange(int x, int y);

    void Attack(Unit *target, bool isCounter = false);

    void Wait();

    void Die();

    // Turn management
    bool IsAvailable() { return mAvailable; }
    bool IsEnemy() { return mIsEnemy; }
    void SetAvailable(bool available) { mAvailable = available; }
    void SetFlyer(bool f) {mIsFlyer = f; }

    // Update function
    void OnUpdate(float deltaTime);

    // Animation
    void PlayAnimation(const std::string &animName, float timer);

protected:
    // Sprite
    DrawAnimatedComponent *mDrawComponent;

    // Animation
    float mAnimationTimer = 0.0f;
    bool mIsAnimating = false;

    // Stats
    Stats mStats;
    int mDmgTaken;
    bool mIsFlyer;
    Vector2 mOldPosition;

    // Weapons and itens
    std::vector<Weapon *> mWeapons;
    std::vector<std::string> mItens;

    // Turn management
    std::unordered_set<int> mMovementRange;
    bool mAvailable = true;
    bool mIsEnemy = true;

    std::string mUnitType;
};
