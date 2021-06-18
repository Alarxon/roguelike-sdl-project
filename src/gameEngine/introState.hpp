#ifndef __INTROSTATE_H__
#define __INTROSTATE_H__

#include <SDL.h>
#include <SDL_image.h>

#include "gameState.hpp"

class IntroState : public GameState{
public:
	void Init(GameEngine* game);
	void Cleanup();

	void Pause();
	void Resume();

	void HandleEvents(GameEngine* game);
	void Update(GameEngine* game);
	void Draw(GameEngine* game);

	static IntroState* Instance() {
		return &m_IntroState;
	}

protected:
	IntroState() { }

private:
	static IntroState m_IntroState;

	SDL_Texture* bg;
	SDL_Texture* fader;
	int alpha;
};

#endif
