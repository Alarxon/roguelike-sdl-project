#include "playState.hpp"
#include <stdio.h>
#include <chrono>

#include "../generator/generator.hpp"
//#include "gameEngine.hpp"
#include "menuState.hpp"
#include "introState.hpp"

#include "../gfx/texture.hpp"
#include "../gfx/tiles.hpp"
#include "../entities/player.hpp"

#include "timer.hpp"
#include <sstream>

PlayState PlayState::m_PlayState;

//Scene textures
Texture gDotTexture;
Texture gTileTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

vector<Tile> tiles;

//The dot that will be moving around on the screen
Player dot;

//Level camera
SDL_Rect camera;

//Room generator
vector<vector<int>> rooms;
vector<vector<char>> level_implementation;
//vector<vector<int>> corridors;

int level_width_tiles;
int level_height_tiles;

Timer stepTimer;

//Globally used font
TTF_Font* gFont = NULL;
//Scene textures
Texture gFPSTextTexture;
Texture levelMapTexture;

//Set text color as black
SDL_Color textColor = { 255, 255, 255, 255 };

//The frames per second timer
Timer fpsTimer;

//In memory text stream
std::stringstream timeText;

//Start counting frames per second
int countedFrames = 0;

bool PlayState::loadMedia( GameEngine* game,  vector<vector<char>> map ){
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "src/assets/dot.bmp", game ) ){
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "src/assets/level_1_tiles.png", game ) ){
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}


	//Load tile map
	if( !setTiles( map ) ){
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	//Open the font
	gFont = TTF_OpenFont( "src/assets/lazy.ttf", 28 );
	if( gFont == NULL ){
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	return success;
}


bool PlayState::setTiles(vector<vector<char>> map){
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

	int id_tile = 0;

	//Initialize the tiles
	for(int i = 0; i < (int)map.size(); i++){
		for(int j = 0; j < (int)map[i].size(); j++){

			int tileType = -1;
			char tile_char = map[i][j];
			bool state = false;
			if(tile_char == FLOOR){
				tileType = TILE_BLUE;
			}else if(tile_char == WALL){
				tileType = TILE_GREEN;
			}else if(tile_char == DOOR){
				tileType = TILE_RED;
			}else if(tile_char == STONE){
				tileType = TILE_CENTER;
				state = true;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) ){
				tiles.push_back(Tile( x, y, tileType, TILE_WIDTH, TILE_HEIGHT, state, id_tile ));
			}//If we don't recognize the tile type
			else{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= level_width ){
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}

			id_tile++;
		}
	}

	//Clip the sprite sheet
	if( tilesLoaded ){
		gTileClips[ TILE_RED ].x = 0;
		gTileClips[ TILE_RED ].y = 0;
		gTileClips[ TILE_RED ].w = TILE_WIDTH;
		gTileClips[ TILE_RED ].h = TILE_HEIGHT;

		gTileClips[ TILE_GREEN ].x = 0;
		gTileClips[ TILE_GREEN ].y = 80;
		gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
		gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

		gTileClips[ TILE_BLUE ].x = 0;
		gTileClips[ TILE_BLUE ].y = 160;
		gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
		gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

		gTileClips[ TILE_TOPLEFT ].x = 80;
		gTileClips[ TILE_TOPLEFT ].y = 0;
		gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
		gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

		gTileClips[ TILE_CENTER ].x = 160;
		gTileClips[ TILE_CENTER ].y = 80;
		gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
		gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;
	}


	//If the map was loaded fine
    return tilesLoaded;
}


void PlayState::Init(GameEngine* game){
	game->exit_play = false;

	Generator level;
	long long int seed = static_cast<unsigned> (chrono::system_clock::now().time_since_epoch().count());
	level.set_seed(seed);

	int w = 64;
	int h = 64;
	int max_r = 15;
	int min_r_xy = 5;
	int max_r_xy = 15;

	level_width_tiles = w;
	level_height_tiles = h;

	level_implementation = level.gen_level(w,h,max_r,min_r_xy,max_r_xy);
	level.print_with_ascii(level_implementation);

	//Level width and height
	level_width = w * TILE_WIDTH;
	level_height = h * TILE_HEIGHT;

	//Total map tiles
	TOTAL_TILES = w * h;

	//Rooms list generated
	rooms = level.get_rooms_list();

	//Corridors list
	//corridors = level.get_corridors_list();

	//Load media
	if( !loadMedia( game, level_implementation ) ){
		printf( "Failed to load media!\n" );
	}else{
		//Intiliaze camera and point position
		int x_initial = ( (rooms[0][0] * TILE_WIDTH) + ( (rooms[0][2] / 2)  * TILE_WIDTH) );
		int y_initial = ( (rooms[0][1] * TILE_HEIGHT) + ( (rooms[0][3] / 2) * TILE_HEIGHT) );;

		camera = { x_initial, y_initial, game->screen_width, game->screen_height };
		dot.setPosition(x_initial, y_initial);
		fpsTimer.start();
		printf("PlayState Init\n");
	}

}

//Draw map
void PlayState::draw_map(GameEngine* game, SDL_Rect box){
	//Border
	int temp_var = 0;
	int offset_map = game->screen_width / 80;
	for(int i = 0; i < (int)level_implementation.size(); i++){
		for(int j = 0; j < (int)level_implementation[i].size(); j++){
			if(tiles[temp_var].getState()){
				int x = ( (game->screen_width - ((level_width / (TILE_WIDTH - offset_map) ) * 2) ) - 8) +  (tiles[temp_var].getBox().x / ( (TILE_WIDTH - offset_map) / 2));
				int y =  (tiles[temp_var].getBox().y / ( (TILE_HEIGHT - offset_map) / 2)) + 8;
				//int x = ( (game->screen_width - ((level_width / TILE_WIDTH) * 2) ) - 8) +  (tiles[temp_var].getBox().x / (TILE_WIDTH / 2));
				//int y =  (tiles[temp_var].getBox().y / (TILE_HEIGHT / 2)) + 8;

				SDL_Rect rec = {x, y, 3, 3};
				//SDL_Rect rec = {x, y, 2, 2};

				//printf("*** %d %d\n",tiles[temp_var].getType(), TILE_BLUE);


				if (i == 0 || j == 0 || i == ((int)level_implementation.size() - 1) || j == ((int)level_implementation[i].size() - 1)){
					SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0xFF, 0xFF );
					SDL_RenderFillRect( game->renderer, &rec );
				}else if(tiles[temp_var].getType() == TILE_GREEN){
					SDL_SetRenderDrawColor( game->renderer, 0xFF, 0x00, 0x00, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}else if(tiles[temp_var].getType() == TILE_BLUE){
					SDL_SetRenderDrawColor( game->renderer, 0x80, 0x80, 0x00, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}
			}
			temp_var++;
		}
	}
	int x = ( (game->screen_width - ((level_width / (TILE_WIDTH - offset_map) ) * 2) ) - 8) +  (box.x / ( (TILE_WIDTH - offset_map) / 2));
	int y =  (box.y / ( (TILE_HEIGHT - offset_map) / 2)) + 8;
	SDL_Rect rec = {x, y, 5, 5};
	/*
	int x = ( (game->screen_width - ((level_width / TILE_WIDTH) * 2) ) - 8) +  (box.x / (TILE_WIDTH / 2));
	int y =  (box.y / (TILE_HEIGHT / 2)) + 8;
	SDL_Rect rec = {x, y, 4, 4};
	*/
	SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0xFF, 0xff );
	SDL_RenderFillRect( game->renderer, &rec );


}

void PlayState::show_map(GameEngine* game, SDL_Rect box){
	stringstream textLevelMap;
	//Set text to be rendered
	textLevelMap.str( "" );
	textLevelMap << "Nivel: Primer Nivel";

	//Render text
	if( !levelMapTexture.loadFromRenderedText( textLevelMap.str().c_str(), textColor, gFont, game) ){
		printf( "Unable to render Level Map texture!\n" );
	}

	int initial_width = game->screen_width / 15;
	int final_width = game->screen_width - initial_width;
	int initial_height = game->screen_height / 15;
	int final_height = game->screen_height - initial_height;

	//printf("Level Width: %d\n", level_width / 80);
	int level_tile_width = (final_width - initial_width) / (level_width / 80);
	int level_tile_height = (final_height - initial_height) / (level_height / 80);
	//printf("Level Width: %d Height: %d\n", level_tile_width,level_tile_height );
	int temp_var = 0;

	int x = initial_width;
	int y = initial_height;

	for(int i = 0; i < (int)level_implementation.size(); i++){
		for(int j = 0; j < (int)level_implementation[i].size(); j++){
			SDL_Rect rec = {x, y, level_tile_width, level_tile_height};

			if(tiles[temp_var].getState()){
				if(tiles[temp_var].getType() == TILE_GREEN){
					SDL_SetRenderDrawColor( game->renderer, 0xFF, 0x00, 0x00, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}else if(tiles[temp_var].getType() == TILE_BLUE){
					SDL_SetRenderDrawColor( game->renderer, 0x80, 0x80, 0x00, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}else if(tiles[temp_var].getType() == TILE_RED){
					SDL_SetRenderDrawColor( game->renderer, 0x22, 0x8B, 0x22, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}else{
					SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0x00, 0xff );
					SDL_RenderFillRect( game->renderer, &rec );
				}
			}else{
				SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0x00, 0xff );
				SDL_RenderFillRect( game->renderer, &rec );
			}

			if(dot.getPlayerTileId() == tiles[temp_var].getId()){
				SDL_Rect rec_player = {x + (level_tile_width / 2) ,(y + (level_tile_height / 2)), 
					level_tile_width / 2, level_tile_height / 2};
				SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0xFF, 0xff );
				SDL_RenderFillRect( game->renderer, &rec_player );
			}
			
			x = x + level_tile_width;

			temp_var++;
		}
		x = initial_width;
		y =  y + level_tile_height;
	}

	//printf("Player tile Id: %d\n", dot.getPlayerTileId());

	/*
	int x_percent = (box.x * 100) / level_width;
	int y_percent = (box.y * 100) / level_height;
	
	
	int x_player = (((final_width - initial_width) * x_percent) / 100);
	int y_player = (((final_height - initial_height) * y_percent) / 100);
	*/
	/*
	int x_player = (((final_width - initial_width) * x_percent) / 100);
	int y_player = (((final_height - initial_height) * y_percent) / 100);

	SDL_Rect rec = {x_player ,y_player, level_tile_width / 2, level_tile_height / 2};

	SDL_SetRenderDrawColor( game->renderer, 0x00, 0x00, 0xFF, 0xff );
	SDL_RenderFillRect( game->renderer, &rec );
	*/
}


void PlayState::Cleanup(){

	//Deallocate tiles
	vector<Tile>().swap(tiles);

	SDL_DestroyTexture(bg);
	//Free loaded images
	gDotTexture.free();
	gTileTexture.free();
	gFPSTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;


	printf("PlayState Cleanup\n");
}

void PlayState::Pause(){
	printf("PlayState Pause\n");
}

void PlayState::Resume(){
	printf("PlayState Resume\n");
}

void PlayState::HandleEvents(GameEngine* game){
	SDL_Event event;
	while( SDL_PollEvent( &event ) != 0 ) {
		//User requests quit
		if( event.type == SDL_QUIT ){
			game->Quit();
		}else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ){
			game->Quit();
		}else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_m ){
			game->PushState( MenuState::Instance() );
		}else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_f ){
			int new_w = 0;
			int new_h = 0;
			if( game->mFullScreen ){
				SDL_SetWindowFullscreen( game->screen, SDL_FALSE );
				game->mFullScreen = false;
			}else{
				SDL_SetWindowFullscreen( game->screen, SDL_WINDOW_FULLSCREEN_DESKTOP );
				game->mFullScreen = true;
				game->mMinimized = false;
			}
			SDL_GetWindowSize(game->screen, &new_w, &new_h);

			game->screen_width = new_w;
			game->screen_height = new_h;
			camera.w = new_w;
			camera.h = new_h;

			//printf("w: %d, h: %d\n",new_w, new_h);
		}else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB && event.key.repeat == 0 ){
			SHOW_MAP = true;
		}else if( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_TAB && event.key.repeat == 0 ){
			SHOW_MAP = false;
		}


		//Handle input for the dot
		dot.handleEvent( event );
	}
}

void PlayState::Update(GameEngine* game){
	if(game->exit_play){
		game->ChangeState( IntroState::Instance() );
	}else{
		//Time step calculation
		float timeStep = stepTimer.getTicks() / 1000.f;

		//Move the dot
		dot.move( tiles, level_width, level_height, rooms,level_width_tiles,level_height_tiles, timeStep );
		//Restart step timer
		stepTimer.start();

		dot.setCamera( camera, game->screen_width, game->screen_height, level_width, level_height );

		//Calculate and correct fps
		float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
		if( avgFPS > 2000000 ){
			avgFPS = 0;
		}

		//Set text to be rendered
		timeText.str( "" );
		timeText << "FPS: " << avgFPS;

		//Render text
		if( !gFPSTextTexture.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, game) ){
			printf( "Unable to render FPS texture!\n" );
		}

	}
}

void PlayState::Draw(GameEngine* game){
	//Clear screen
	SDL_SetRenderDrawColor( game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( game->renderer );

	//Render level
	for( int i = 0; i < TOTAL_TILES; ++i ){
		tiles[ i ].render( camera, &gTileTexture, gTileClips, game );
	}

	//Render dot
	dot.render( camera, &gDotTexture, game );

	draw_map(game, dot.getPosition());

	if(SHOW_MAP){
		show_map(game, dot.getPosition());
		int screen_width = (game->screen_width / 2) - (levelMapTexture.getWidth() / 2);
		int screen_height = game->screen_height / 15;
		//printf("Text W: %d\n",levelMapTexture.getWidth());
		levelMapTexture.render( screen_width, screen_height, game);
	}

	//Render textures
	gFPSTextTexture.render( 1, 1, game);

	//Update screen
	SDL_RenderPresent( game->renderer );
	++countedFrames;
}

