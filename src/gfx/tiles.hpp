#ifndef __TILES_H__
#define __TILES_H__

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "texture.hpp"

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 2;
const int TILE_BLUE = 1;
const int TILE_CENTER = 3;
const int TILE_TOPLEFT = 11;


//The tile
class Tile{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType, int tile_width, int tile_height, bool state, int id );

		//Shows the tile
		void render( SDL_Rect& camera, Texture* texture, SDL_Rect gTileClips[], GameEngine* game);

		//Get the tile type
		int getType();

		//Set the tile type
		void setType(int type);

		//Get state
		bool getState();

		//Set state
		void setState(bool state);

		//Get id
		int getId();

		//Set id
		void setId(int id);

		//Get the collision box
		SDL_Rect getBox();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//State flag
		bool state_flag;

		//The tile type
		int mType;

		//Tile ID
		int tile_id;
};

bool checkCollision( SDL_Rect a, SDL_Rect b );
bool touchesWall( SDL_Rect box, vector<Tile> tiles);
void activate_room(int index,vector<vector<int>> rooms,vector<Tile> &tiles);
void check_room_activation(SDL_Rect box,vector<vector<int>> rooms,vector<Tile> &tiles,
                            int level_width_tiles, int level_height_tiles,int &player_tileid);

#endif