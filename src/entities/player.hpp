#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "../gfx/tiles.hpp"

class Player{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 5;
		//static const int DOT_VEL = 640;

		//Initializes the variables
		Player();

		//Takes key presses and adjusts the player's velocity
		void handleEvent( SDL_Event& e );

		//Moves the player and check collision against tiles
		void move( vector<Tile> &tiles, int level_width, int level_height,vector<vector<int>> rooms,
                        int level_width_tiles, int level_height_tiles, float timeStep );

		//Centers the camera over the player
		void setCamera( SDL_Rect& camera, int screen_width, int screen_height,  int level_width, int level_height );

		//Shows the player on the screen
		void render( SDL_Rect& camera, Texture* texture, GameEngine* game );

		//Set player position (initial)
		void setPosition(int x, int y);

		//Get player position
		SDL_Rect getPosition();

		//Set player tile ID
		void setPlayerTileId(int id);

		//Get player tile ID
		int getPlayerTileId();

    private:
		//Collision box of the player
		SDL_Rect mBox;

		//The velocity of the player
		//int mVelX, mVelY;
		float mVelX, mVelY;

		int tile_id_actual;
};

#endif