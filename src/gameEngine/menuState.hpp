#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__

#include <SDL.h>
#include <SDL_image.h>

#include "gameState.hpp"

class MenuState : public GameState{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static MenuState* Instance() {
		return &m_MenuState;
	}

protected:
	MenuState() { }

private:
	static MenuState m_MenuState;

	SDL_Texture* bg;
};

#endif