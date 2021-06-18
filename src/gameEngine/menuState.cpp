#include <stdio.h>

//#include "gameEngine.hpp"
//#include "gameState.hpp"
#include "menuState.hpp"
//#include "playState.hpp"

MenuState MenuState::m_MenuState;

void MenuState::Init(GameEngine* game){
	SDL_Surface* temp = IMG_Load("src/assets/menu.bmp");
	SDL_Texture* newTexture = NULL;

	if( temp == NULL ){
		printf( "Unable to load image! Error: %s\n", IMG_GetError() );
	}else{
		SDL_SetColorKey( temp, SDL_TRUE, SDL_MapRGB( temp->format, 0, 0xFF, 0xFF ) );
		newTexture = SDL_CreateTextureFromSurface( game->renderer, temp );
		if( newTexture == NULL ){
			printf( "Unable to create texture from image! SDL Error: %s\n", SDL_GetError() );
		}else{
			bg = newTexture;

			SDL_FreeSurface(temp);

			printf("MenuState Init\n");
		}
	}
}

void MenuState::Cleanup(){
	SDL_DestroyTexture(bg);

	printf("MenuState Cleanup\n");
}

void MenuState::Pause(){
	printf("MenuState Pause\n");
}

void MenuState::Resume(){
	printf("MenuState Resume\n");
}

void MenuState::HandleEvents(GameEngine* game){
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->Quit();
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						game->PopState();
						break;
					case SDLK_SPACE:
						game->exit_play = true;
						game->PopState();
						break;
					case SDLK_f:
						int new_w = 0;
						int new_h = 0;
						if( game->mFullScreen ){
							SDL_SetWindowFullscreen( game->screen, SDL_FALSE );
							game->mFullScreen = false;
						}else{
							SDL_SetWindowFullscreen( game->screen, SDL_TRUE );
							game->mFullScreen = true;
							game->mMinimized = false;
						}
						SDL_GetWindowSize(game->screen, &new_w, &new_h);

						game->screen_width = new_w;
						game->screen_height = new_h;
						break;
				}
				break;
		}
	}
}

void MenuState::Update(GameEngine* game){

}

void MenuState::Draw(GameEngine* game){
	SDL_RenderClear( game->renderer );
	SDL_RenderCopy( game->renderer, bg, NULL, NULL );
	SDL_RenderPresent( game->renderer );
}

