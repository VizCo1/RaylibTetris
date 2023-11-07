#include "raylib.h"
#include "Game.h"
#include "Settings.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Game game{settings::screenWidth,
              settings::screenHeight, 
              settings::fps, 
              "Tetris Raylib" };

    while (!game.GameShouldClose()) {
        game.Tick();
    }
    
    return 0;
}
