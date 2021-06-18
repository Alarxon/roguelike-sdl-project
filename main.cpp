#include <iostream>
#include <vector>
//#include <ctime>
#include <chrono>
#include "src/gameEngine/gameEngine.hpp"
#include "src/gameEngine/introState.hpp"
//#include <winuser.h>

using namespace std;

int main(int argc, char *argv[]){
    //SystemParametersInfo (SPI_SETKEYBOARDDELAY, 0, 0, 0);

    GameEngine game;

    // initialize the engine
    bool success = game.Init( "Roguelike alpha 1.0" );
    game.mFullScreen = false;
    game.mMinimized = false;

    if(!success){
       	printf( "Failed to initialize!\n" );
    }else{
         // load the intro
        game.ChangeState( IntroState::Instance() );

        // main loop
        while ( game.Running() ){
            game.HandleEvents();
            game.Update();
            game.Draw();
        }
    }

    // cleanup the engine
    game.Cleanup();

    //Generator level;
    //long long int seed = static_cast<unsigned> (chrono::system_clock::now().time_since_epoch().count());
    //level.set_seed(seed);
    //vector<vector<char>> level_implementation = level.gen_level(70,70,30,10,10);
    //level.print_with_ascii(level_implementation);

    //SystemParametersInfo (SPI_SETKEYBOARDDELAY, 1, 0, 0);

    return 0;
}

