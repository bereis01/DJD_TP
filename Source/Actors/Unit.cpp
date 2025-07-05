#include "Unit.h"
#include "../Game.h"
#include "../Components/DrawComponents/DrawAnimatedComponent.h"
#include "../Effects/ParticleSystem.h"
#include "../UIElements/StatScreen.h"

Stats::Stats(std::string n, int h, int ch, int st, int m, int sk, int sp, int d, int r, int mv) {
    name = n;
    hp = h;
    currHp = ch;
    str = st;
    mag = m;
    skl = sk;
    spd = sp;
    def = d;
    res = r;
    mov = mv;
}

Weapon::Weapon(std::string n, int acc, int mt, int crit, int rng, bool m) {
    name = n;
    hit = acc;
    might = mt;
    criticalChance = crit;
    range = rng;
    magic = m;
}

Unit::Unit(Game *game, Stats stats, bool isEnemy, const std::string &unitType) : Actor(game) {
    // Sprite component
    if (unitType == "Knight") {
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Units/Allies/Knight.png",
                                                   "../Assets/Sprites/Units/Allies/Knight.json", 200);
        mDrawComponent->AddAnimation("Idle", {15, 16, 17, 18, 19, 20});
        mDrawComponent->AddAnimation("Attack", {0, 1, 2, 3, 4, 5, 6});
        mDrawComponent->AddAnimation("Hurt", {11, 12, 13, 14});
        mDrawComponent->AddAnimation("Death", {7, 8, 9, 10});
    } else if (unitType == "Wizard") {
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Units/Allies/Wizard.png",
                                                   "../Assets/Sprites/Units/Allies/Wizard.json", 200);
        mDrawComponent->AddAnimation("Idle", {14, 15, 16, 17, 18, 19});
        mDrawComponent->AddAnimation("Attack", {0, 1, 2, 3, 4, 5});
        mDrawComponent->AddAnimation("Hurt", {10, 11, 12, 13});
        mDrawComponent->AddAnimation("Death", {7, 8, 9, 6});
    } else if (unitType == "Orc") {
        mDrawComponent = new DrawAnimatedComponent(this, "../Assets/Sprites/Units/Enemies/Orc.png",
                                                   "../Assets/Sprites/Units/Enemies/Orc.json", 200);
        mDrawComponent->AddAnimation("Idle", {14, 15, 16, 17, 18, 19});
        mDrawComponent->AddAnimation("Attack", {0, 1, 2, 3, 4, 5});
        mDrawComponent->AddAnimation("Hurt", {10, 11, 12, 13});
        mDrawComponent->AddAnimation("Death", {8, 9, 6, 7});
    }
    mDrawComponent->SetAnimation("Idle");
    mDrawComponent->SetAnimFPS(10.0f);

    // Other parameters
    mDmgTaken = 0;
    mStats = stats;
    mIsEnemy = isEnemy;
    mAvailable = true;
}

Unit::~Unit() {
    // Deallocates weapons
    for (auto weapon: mWeapons)
        delete weapon;
}

void Unit::OnUpdate(float deltaTime) {
    // If there is an animation going, decreases the timer and
    // when it reaches 0, reverts back to default state
    if (mIsAnimating) {
        if (mAnimationTimer > 0) {
            mAnimationTimer -= deltaTime;
        } else {
            // Reverts to Idle animation and disables animating state
            mDrawComponent->SetAnimation("Idle");
            mIsAnimating = false;
        }
    }
}

void Unit::PlayAnimation(const std::string &animName, float timer) {
    // Sets up the timer, changes the animation and sets the animating flag
    mAnimationTimer = timer;
    if (animName == "Attack")
        mDrawComponent->SetAnimation("Attack");
    else if (animName == "Hurt")
        mDrawComponent->SetAnimation("Hurt");
    else if (animName == "Death")
        mDrawComponent->SetAnimation("Death");
    mIsAnimating = true;
}

void Unit::SetStats(Stats stats) {
    mStats.name = stats.name;
    mStats.hp = stats.hp;
    mStats.currHp = stats.currHp;
    mStats.str = stats.str;
    mStats.mag = stats.mag;
    mStats.skl = stats.skl;
    mStats.spd = stats.spd;
    mStats.def = stats.def;
    mStats.res = stats.res;
    mStats.mov = stats.mov;
}

void Unit::ShowStats() {
    mGame->GetStatScreen()->SetDisplayStats(this);
    mGame->PushUI(mGame->GetStatScreen());
}

void Unit::Attack(class Unit *target, bool isCounter) {
    // Calculates parameters
    Stats target_stats = target->GetStats();
    int chance_to_hit = (GetEquippedWeapon()->hit + mStats.skl * 2) - (target_stats.spd * 2);
    int crit_chance = GetEquippedWeapon()->criticalChance + mStats.spd - target_stats.skl;
    crit_chance = std::max(crit_chance, 0);
    int true_hit = (rand() % 101) + (rand() % 101);
    int to_crit = rand() % 101;

    // If it is a hit
    if (chance_to_hit * 2 > true_hit) {
        // Calculates damage
        int damage = 0;
        if (GetEquippedWeapon()->magic) {
            damage = mStats.mag + GetEquippedWeapon()->might - target_stats.res;
        } else {
            damage = mStats.str + GetEquippedWeapon()->might - target_stats.def;
        }
        damage = std::max(damage, 0);
        if (crit_chance > to_crit) {
            damage = damage * 3;
        }

        // Applies damage to target
        target->SetCurrentHp(target_stats.currHp - damage);

        // Plays animation
        if (!isCounter) {
            PlayAnimation("Attack", 0.6f);
            target->PlayAnimation("Hurt", 0.4f);
        }

        // Shows damage particle
        mGame->GetParticleSystem()->CreateTextParticle(target->GetX(), target->GetY(), std::to_string(damage));

        // Kills target if hp becomes less than 0
        if (target_stats.currHp - damage <= 0) {
            if (mGame->GetGamePlayState() == Game::GamePlayState::EnemyTurn && target->IsEnemy()) {
                return;
            }
            target->Die();

            // Shows death particle
            mGame->GetParticleSystem()->CreateAnimatedParticle(target->GetX(), target->GetY(), "Death");

            mAvailable = false;
            return;
        }
    }
    // If it is a miss
    else {
        // Plays animation
        if (!isCounter)
            PlayAnimation("Attack", 0.6f);

        // Shows damage particle
        mGame->GetParticleSystem()->CreateTextParticle(target->GetX(), target->GetY(), "MISS!");
    }

    // Applies counter attack if applicable
    if (!isCounter) {
        int unitX = GetX();
        int unitY = GetY();
        int targetX = target->GetX();
        int targetY = target->GetY();
        if (abs(unitX - targetX) + abs(unitY - targetY) <= target->GetEquippedWeapon()->range) {
            target->Attack(this, true);
        }
    }

    // Makes the unit unusable any further
    mAvailable = false;
}

void Unit::EquipWeapon(int pos) {
    Weapon *target = mWeapons[pos];
    auto it = std::find(mWeapons.begin(), mWeapons.end(), target);

    // Check if found and not already the first element
    if (it != mWeapons.end() && it != mWeapons.begin()) {
        std::swap(*it, mWeapons.front());
    }
    mGame->ShowItems();
    mGame->GetActionScreen()->SetSelectedButtonIndex(0);
}

void Unit::UseItem(const std::string &item) {
    if (mStats.currHp < mStats.hp) {
        if (item == "Healing potion") {
            mStats.currHp = std::min(mStats.currHp + 10, mStats.hp);
        } else if (item == "Healing gem") {
            mStats.currHp = std::min(mStats.currHp + 20, mStats.hp);
        }
        mAvailable = false;
        mGame->GetUIStack().pop_back();
        mGame->SetGamePlayState(Game::GamePlayState::Map);
        mGame->SetSelectedUnit(nullptr);
        mGame->GetActionScreen()->SetSelectedButtonIndex(0);
    }
}

void Unit::Die() {
    SetState(ActorState::Destroy);
}

void Unit::Wait() {
    mOldPosition = mPosition;
    mAvailable = false;
    mGame->GetUIStack().pop_back();
    mGame->SetGamePlayState(Game::GamePlayState::Map);
    mGame->SetSelectedUnit(nullptr);
    mGame->GetActionScreen()->SetSelectedButtonIndex(0);
}
