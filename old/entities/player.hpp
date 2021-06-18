#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL.h>
#include "../generator/tile.hpp"

class Player{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 10;

		//Initializes the variables
		Player();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move( vector<Tile*> tiles, int level_width, int level_height );

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera, int screen_width, int screen_height,  int level_width, int level_height );

		//Shows the dot on the screen
		void render( SDL_Rect& camera, Texture* texture, GameEngine* game );

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
};


#endif