#pragma once
#include "Actor.h"
#include "../Components/DrawComponents/DrawSpriteComponent.h"

class Stats {
public:
    Stats(std::string name = "x", int hp = 0, int str = 0, int mag = 0, int skl = 0, int spd = 0, int def = 0,
          int res = 0);

    std::string name;
    int hp;
    int str;
    int mag;
    int skl;
    int spd;
    int def;
    int res;
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
    Unit(Game *game, const std::string &texturePath, int mov = 5);

    // Manipulation based on coordinates
    void SetXY(int x, int y) { mPosition = Vector2(y * Game::TILE_SIZE, x * Game::TILE_SIZE); };
    int GetX() { return static_cast<int>(mPosition.y / Game::TILE_SIZE); }
    int GetY() { return static_cast<int>(mPosition.x / Game::TILE_SIZE); }

    // Stats manipulation
    void SetStats(Stats stats);

    Stats GetStats() { return mStats; }
    void SetMovement(int mov) { mMovement = mov; }
    int GetMovement() { return mMovement; }
    void SetOldPosition(Vector2 oldPosition) { mOldPosition = oldPosition; }
    Vector2 GetOldPosition() { return mOldPosition; }

    // Update function
    void OnUpdate(float deltaTime) override;

    // Weapon manipulation
    void AddWeapon(Weapon *weapon) { mWeapons.push_back(weapon); }
    void SetEquippedWeapon(Weapon *weapon) { mEquippedWeapon = weapon; }
    Weapon *GetEquippedWeapon() { return mEquippedWeapon; }

    // In-game actions
    void ShowStats();

    void Attack(Unit *target);

    void TakeDamage(int dmg);

    void Die();

    void Wait();

    // Turn management
    bool IsAvailable() { return mAvailable; }
    void SetAvailable(bool available) { mAvailable = available; }

protected:
    // Sprite
    DrawSpriteComponent *mDrawComponent;

    // Stats
    Stats mStats;
    int mDmgTaken;
    int mMovement;
    Vector2 mOldPosition;

    // Weapon
    Weapon *mEquippedWeapon;
    std::vector<Weapon *> mWeapons;

    // Turn management
    bool mAvailable = true;
};
