#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Menu.h"
#include "Scene.h"
#include "SoundControl.h"


#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 540


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;
	void setPlay();
	void levelSkip();
	

private:
	void updateScene(int deltaTime);
	void updateMenu(int deltaTime);

	ISoundEngine* engine;

	bool past1, past2, past3, past4, past5, past6, past7, past8, past9, past0;

	bool bPlay, playing;                       // Continue to play game?
	int level, death_counter, score, strawberry_counter;
	bool Strawberry_picked_up_in_level;
	Menu menu;						  //Menu to render
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	
};
	

#endif // _GAME_INCLUDE


