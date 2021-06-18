#ifndef __GAMEENGINE_H__
#define __GAMEENGINE_H__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>

using namespace std;

//Only declaration
class GameState;

class GameEngine{
public:
	bool Init(const char* title, int width=640, int height=480, bool fullscreen=false);
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; }

	bool exit_play;
	int screen_width;
	int screen_height;

	SDL_Window* screen;
	SDL_Renderer* renderer;
	bool mFullScreen;
	bool mMinimized;

private:
	// the stack of states
	vector<GameState*> states;

	bool m_running;
	bool m_fullscreen;
};

#endif