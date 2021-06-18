#include "tiles.hpp"

bool checkCollision( SDL_Rect a, SDL_Rect b ){
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB ){
        return false;
    }

    if( topA >= bottomB ){
        return false;
    }

    if( rightA <= leftB ){
        return false;
    }

    if( leftA >= rightB ){
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool touchesWall( SDL_Rect box, vector<Tile> tiles){
    //Go through the tiles
    for( int i = 0; i < (int)tiles.size(); i++ ){
        //If the tile is a wall type tile
        if( ( tiles[ i ].getType() >= TILE_GREEN ) && ( tiles[ i ].getType() <= (TILE_TOPLEFT - 1) ) ){
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ].getBox() ) ){
				return true;
            }
        }
    }

    //If no wall tiles were touched
    return false;
}

void activate_room(int index,vector<vector<int>> rooms,vector<Tile> &tiles){
	int x_corner = rooms[index][0] - 1;
	int y_corner = rooms[index][1] - 1;
	int w_corner = rooms[index][2] + 3;
	int h_corner = rooms[index][3] + 3;

	for(int i = 0; i < (int)tiles.size(); i++){
		if((tiles[i].getBox().x + tiles[i].getBox().w) > (x_corner * TILE_WIDTH) && 
			(tiles[i].getBox().x + tiles[i].getBox().w) < (x_corner * TILE_WIDTH + w_corner * TILE_WIDTH) &&
			(tiles[i].getBox().y + tiles[i].getBox().h) > (y_corner * TILE_HEIGHT) &&
			(tiles[i].getBox().y + tiles[i].getBox().h) < (y_corner * TILE_HEIGHT + h_corner * TILE_HEIGHT) ){
                tiles[i].setState(true);
		}
	}
	
}


void check_room_activation(SDL_Rect box,vector<vector<int>> rooms,vector<Tile> &tiles,
                            int level_width_tiles, int level_height_tiles, int &player_tileid){
    	for(int i = 0; i < (int)rooms.size(); i++){
		if(rooms[i][5] == 0){
			if((box.x + box.w) >= (rooms[i][0] * TILE_WIDTH) && 
				(box.x + box.w) <= (rooms[i][0] * TILE_WIDTH + rooms[i][2] * TILE_WIDTH) &&
				(box.y + box.h) >= (rooms[i][1] * TILE_HEIGHT) &&
				(box.y + box.h) <= (rooms[i][1] * TILE_HEIGHT + rooms[i][3] * TILE_HEIGHT) ){
					rooms[i][5] = 1;
					activate_room(i,rooms,tiles);
					//printf("*** Room: %d\n", i);
			}
		}
	}

	//Corridors activation
    for( int i = 0; i < (int)tiles.size(); i++ ){
        if( ((tiles[ i ].getType() == TILE_BLUE)) ||
		    ((tiles[ i ].getType() == TILE_RED)) ){
            if( checkCollision( box, tiles[ i ].getBox() ) 
				&& (tiles[i].getState() == false || tiles[i-1].getState() == false || 
					tiles[i+1].getState() == false || tiles[i-level_width_tiles].getState() == false ||
					tiles[i+level_width_tiles].getState() == false) ){
				
				tiles[i].setState(true);
				if( (tiles[i-1].getType() == TILE_GREEN) ){
					tiles[i-1].setState(true);
				}
				if( (tiles[i+1].getType() == TILE_GREEN) ){
					tiles[i+1].setState(true);
				}
				if( (tiles[i-level_width_tiles].getType() == TILE_GREEN) ){
					tiles[i-level_width_tiles].setState(true);
				}
				if( (tiles[i+level_width_tiles].getType() == TILE_GREEN) ){
					tiles[i+level_width_tiles].setState(true);
				}

            }else if( checkCollision( box, tiles[ i ].getBox() )){
                player_tileid = tiles[i].getId();
            }
        }
    }
}

Tile::Tile( int x, int y, int tileType, int tile_width, int tile_height, bool state,int id){
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = tile_width;
    mBox.h = tile_height;

    //Get the tile type
    mType = tileType;

	//State tile
	state_flag = state;

    //Tile ID
    tile_id = id;
}


void Tile::render( SDL_Rect& camera, Texture* texture, SDL_Rect gTileClips[], GameEngine* game){
    //If the tile is on screen
    if( checkCollision( camera, mBox ) ){
        //Show the tile
		if(getState()){
        	texture->render( mBox.x - camera.x, mBox.y - camera.y, game, &gTileClips[ mType ] );
		}else{
			texture->render( mBox.x - camera.x, mBox.y - camera.y, game, &gTileClips[ TILE_TOPLEFT ] );
			//texture->render( mBox.x - camera.x, mBox.y - camera.y, game, &gTileClips[ mType ] );
		}
        //texture->render( mBox.x - camera.x, mBox.y - camera.y, game, &gTileClips[ mType ] );
    }
}

int Tile::getType(){
    return mType;
}

void Tile::setType(int type){
	mType = type;
}

bool Tile::getState(){
	return state_flag;
}

void Tile::setState(bool state){
	state_flag = state;
}

int Tile::getId(){
	return tile_id;
}

void Tile::setId(int id){
	tile_id = id;
}

SDL_Rect Tile::getBox(){
    return mBox;
}

