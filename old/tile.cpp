#include "tile.hpp"
#include "../common/collision.hpp"

Tile::Tile( int x, int y, int tileType, int tile_width, int tile_height ){
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = tile_width;
    mBox.h = tile_height;

    //Get the tile type
    mType = tileType;
}


void Tile::render( SDL_Rect& camera, Texture* texture, SDL_Rect gTileClips[], GameEngine* game){
    //If the tile is on screen
    if( checkCollision( camera, mBox ) ){
        //Show the tile
        texture->render( mBox.x - camera.x, mBox.y - camera.y, game, &gTileClips[ mType ] );
    }
}

int Tile::getType(){
    return mType;
}

SDL_Rect Tile::getBox(){
    return mBox;
}



