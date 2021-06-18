#include "player.hpp"

int up_bool = 0;
int down_bool = 0;
int left_bool = 0;
int right_bool = 0;

Player::Player(){
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Player::handleEvent( SDL_Event& e ){
    
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }

    //printf("key state: %d %d\n", e.key.state == SDL_PRESSED, e.key.state == SDL_RELEASED);
    //printf("key type: %d\n", e.type);
    //printf("X: %d, Y: %d\n", (int)mVelX, (int)mVelY);
}

void Player::move( vector<Tile> &tiles, int level_width, int level_height,vector<vector<int>> rooms,
                        int level_width_tiles, int level_height_tiles, float timeStep ){
    //Move the dot left or right
    mBox.x += mVelX;
    //mBox.x += (int)(mVelX * timeStep);
    //printf("X: %d %f %f\n", mBox.x, mVelX, mVelX * timeStep);
    //printf("Time: %d %d %f %f\n",mBox.x,prueba, mVelX, 640 * timeStep );

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > level_width ) || touchesWall( mBox, tiles ) ){
        //move back
        mBox.x -= mVelX;
        //mBox.x -= 1;
    }

    //Move the dot up or down
    mBox.y += mVelY;
    //mBox.y  += (int)(mVelY * timeStep);

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > level_height ) || touchesWall( mBox, tiles ) ){
        //move back
        mBox.y -= mVelY;
        //mBox.x -= 1;
    }

	check_room_activation(mBox,rooms,tiles,level_width_tiles,level_height_tiles, tile_id_actual);
}

void Player::setCamera( SDL_Rect& camera, int screen_width, int screen_height,  int level_width, int level_height ){
	//Center the camera over the dot
	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - screen_width / 2;
	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - screen_height / 2;

	//Keep the camera in bounds
	if( camera.x < 0 ){
		camera.x = 0;
	}
	if( camera.y < 0 ){
		camera.y = 0;
	}
	if( camera.x > level_width - camera.w ){
		camera.x = level_width - camera.w;
	}
	if( camera.y > level_height - camera.h ){
		camera.y = level_height - camera.h;
	}
}

void Player::render( SDL_Rect& camera, Texture* texture,  GameEngine* game ){
    //Show the dot
	texture->render( mBox.x - camera.x, mBox.y - camera.y, game );
}

//Set player position (initial)
void Player::setPosition(int x, int y){
	//Player hitbox
    mBox.x = x;
    mBox.y = y;
}

//Get player position
SDL_Rect Player::getPosition(){
	return mBox;
}

//Set player tile ID
void Player::setPlayerTileId(int id){
	//Player ID
    tile_id_actual = id;
}

//Get player tile ID
int Player::getPlayerTileId(){
	return tile_id_actual;
}

