#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "gameEngine.hpp"
#include "gameState.hpp"

bool GameEngine::Init(const char* title, int width, int height, bool fullscreen){
    //Flags
    int flags = 0;
    bool success = true;
	screen_width = width;
	screen_height = height;
    // initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
	}else{
        //Set texture filtering to linear, opcional
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		if ( fullscreen ) {
			flags = SDL_WINDOW_FULLSCREEN;
		}
        //Create window
		screen = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags );
		if( screen == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
		}else{
            //Create renderer for window
			//renderer = SDL_CreateRenderer( screen, -1, SDL_RENDERER_ACCELERATED );
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer( screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

			if( renderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
			}else{
                //Initialize renderer color
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
                //Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
				}else{
					//Initialize SDL_ttf
					if( TTF_Init() == -1 ){
						printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
						success = false;
					}else{
						//Engine global variables
						m_fullscreen = fullscreen;
						m_running = true;
						printf("GameEngine Init\n");
					}
                }
			}
		}
	}
    return success;
}

void GameEngine::Cleanup(){
	// cleanup the all states
	while ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// switch back to windowed mode so other
	// programs won't get accidentally resized
	if ( m_fullscreen ) {
        screen = SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480,
                          0);
	}

    screen = NULL;
	renderer = NULL;

	printf("GameEngine Cleanup\n");

	// shutdown SDL
	SDL_Quit();
    SDL_Quit();
}

void GameEngine::ChangeState(GameState* state){
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PushState(GameState* state){
	// pause current state
	if ( !states.empty() ) {
		states.back()->Pause();
	}

	// store and init the new state
	states.push_back(state);
	states.back()->Init(this);
}

void GameEngine::PopState(){
	// cleanup the current state
	if ( !states.empty() ) {
		states.back()->Cleanup();
		states.pop_back();
	}

	// resume previous state
	if ( !states.empty() ) {
		states.back()->Resume();
	}
}

void GameEngine::HandleEvents(){
	// let the state handle events
	states.back()->HandleEvents(this);
}

void GameEngine::Update(){
	// let the state update the game
	states.back()->Update(this);
}

void GameEngine::Draw(){
	// let the state draw the screen
	states.back()->Draw(this);
}

