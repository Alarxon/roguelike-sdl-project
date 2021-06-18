#ifndef __TILE_H__
#define __TILE_H__

#include <SDL.h>
#include <SDL_image.h>
#include "../common/texture.hpp"

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOPLEFT = 11;
int TOTAL_TILES = 0;


//The tile
class Tile{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType, int tile_width, int tile_height );

		//Shows the tile
		void render( SDL_Rect& camera, Texture* texture, SDL_Rect gTileClips[], GameEngine* game);

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();


    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

#endif