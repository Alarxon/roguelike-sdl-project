#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <SDL.h>
#include "../generator/tile.hpp"

    
//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, vector<Tile*> tiles );

#endif