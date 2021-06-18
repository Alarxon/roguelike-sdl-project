#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "gameState.hpp"


class PlayState : public GameState{
public:
	int TOTAL_TILES = 0;
	bool SHOW_MAP = false;
	void Init(GameEngine* game);
	bool loadMedia( GameEngine* game, vector<vector<char>> map);
	bool setTiles( vector<vector<char>> map );
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	void draw_map(GameEngine* game, SDL_Rect box);
	void show_map(GameEngine* game, SDL_Rect box);

	static PlayState* Instance() {
		return &m_PlayState;
	}
	
protected:
	PlayState() { }

private:
	int level_width;
	int level_height;
	
	static PlayState m_PlayState;

	SDL_Texture* bg;
};

#endif
