#include <stdio.h>

#include "introState.hpp"
//#include "gameEngine.hpp"
#include "playState.hpp"

IntroState IntroState::m_IntroState;

void IntroState::Init(GameEngine* game){
	SDL_Texture* newTexture = NULL;
	SDL_Texture* newTexture_fader = NULL;

	SDL_Surface* temp = IMG_Load("src/assets/intro.bmp");
	SDL_Surface* temp_fader = NULL;

	if( temp == NULL ){
		printf( "Unable to load image! Error: %s\n", IMG_GetError() );
	}else{
		SDL_SetColorKey( temp, SDL_TRUE, SDL_MapRGB( temp->format, 0, 0xFF, 0xFF ) );
		newTexture = SDL_CreateTextureFromSurface( game->renderer, temp );
		if( newTexture == NULL ){
			printf( "Unable to create texture from image! SDL Error: %s\n", SDL_GetError() );
		}else{
            // create the fader surface like the background with alpha
            temp_fader = SDL_CreateRGBSurface( 0, temp->w, temp->h,
                                        temp->format->BitsPerPixel,
                                        temp->format->Rmask, temp->format->Gmask,
                                        temp->format->Bmask, temp->format->Amask );
            
            // fill the fader surface with black
            SDL_FillRect (temp_fader, NULL, SDL_MapRGB (temp->format, 0, 0, 0)) ;
            // start off opaque
            alpha = 255;

            SDL_SetSurfaceAlphaMod(temp_fader, alpha);

            newTexture_fader = SDL_CreateTextureFromSurface( game->renderer, temp_fader );

            SDL_FreeSurface(temp);
            SDL_FreeSurface(temp_fader);

            bg = newTexture;
            fader = newTexture_fader;

            printf("IntroState Init\n");
        }
	}
}

void IntroState::Cleanup(){
	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(fader);

	printf("IntroState Cleanup\n");
}

void IntroState::Pause(){
	printf("IntroState Pause\n");
}

void IntroState::Resume(){
	printf("IntroState Resume\n");
}

void IntroState::HandleEvents(GameEngine* game){
	SDL_Event event;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game->Quit();
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
						game->ChangeState( PlayState::Instance() );
						break;

					case SDLK_ESCAPE:
						game->Quit();
						break;

					case SDLK_f:
						int new_w = 0;
						int new_h = 0;
						if( game->mFullScreen ){
							SDL_SetWindowFullscreen( game->screen, SDL_FALSE );
							game->mFullScreen = false;
						}else{
							//SDL_WINDOW_FULLSCREEN_DESKTOP y SDL_WINDOW_FULLSCREEN
							SDL_SetWindowFullscreen( game->screen, SDL_WINDOW_FULLSCREEN_DESKTOP );
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

void IntroState::Update(GameEngine* game){
	alpha--;

	if (alpha < 0)
		alpha = 0;

	SDL_SetTextureAlphaMod(fader, alpha);
}

void IntroState::Draw(GameEngine* game){
	SDL_RenderClear( game->renderer );
	SDL_RenderCopy( game->renderer, bg, NULL, NULL );

	// no need to blit if it's transparent
	if ( alpha != 0 )
		SDL_RenderCopy( game->renderer, fader, NULL, NULL );

	SDL_RenderPresent( game->renderer );
}

