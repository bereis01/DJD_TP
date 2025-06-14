#include "Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

int main(int argc, char **argv) {
    Game game = Game(SCREEN_WIDTH, SCREEN_HEIGHT);
    bool success = game.Initialize();
    if (success) {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
