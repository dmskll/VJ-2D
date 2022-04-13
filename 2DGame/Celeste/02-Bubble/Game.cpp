#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	SoundControl::instance().init();
	engine = SoundControl::instance().getSoundEngine();
	engine->setSoundVolume(1);
	engine->play2D("sounds/menu.mp3", true);
	
	

	bPlay = true;
	playing = false; //empezamos en el menu
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	menu.init();
}

bool Game::update(int deltaTime)
{
	if (!playing)
	{
		updateMenu(deltaTime);
	}
	else
	{
		updateScene(deltaTime);
	}

	past1 = Game::instance().getKey(49);
	past2 = Game::instance().getKey(50);
	past3 = Game::instance().getKey(51);
	past4 = Game::instance().getKey(52);
	past5 = Game::instance().getKey(53);
	past6 = Game::instance().getKey(54);
	past7 = Game::instance().getKey(55);
	past8 = Game::instance().getKey(56);
	past9 = Game::instance().getKey(57);
	past0 = Game::instance().getKey(48);
	return bPlay;
}

void Game::updateMenu(int deltaTime)
{
	menu.update(deltaTime);
}

void Game::updateScene(int deltaTime) 
{
	scene.update(deltaTime);

	if (scene.check_win()) {
		level++;
		if (level == 11)
		{
			engine->stopAllSounds();
			engine->play2D("sounds/end-music.wav", true);
		}

		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		if (Strawberry_picked_up_in_level) {
			score += 1000;
			strawberry_counter++;
			Strawberry_picked_up_in_level = false;
		}
	}

	if (level != 11 && engine->isCurrentlyPlaying("sounds/end-music.wav"))
	{
		engine->stopAllSounds();
		engine->play2D("sounds/level-music.mp3", true);
	}

	if (scene.check_lose()) {
		float t = scene.getTime();
		scene = Scene();
		death_counter++;
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	if (scene.check_strawberry()) {
		Strawberry_picked_up_in_level = true;
	}
	levelSkip();
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!playing)
	{
		menu.render();
	}
	else
	{
		scene.render();
	}
}


void Game::levelSkip() {
	if (Game::instance().getKey(49) && !past1) {
		level = 1;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(50) && !past2) {
		level = 2;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(51) && !past3) {
		level = 3;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(52) && !past4) {
		level = 4;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(53) && !past5) {
		level = 5;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(54) && !past6) {
		level = 6;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(55) && !past7) {
		level = 7;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(56) && !past8) {
		level = 8;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(57) && !past9) {
		level = 9;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(48) && !past0) {
		level = 10;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t, death_counter, strawberry_counter);
		Strawberry_picked_up_in_level = false;
	}
}

void Game::setPlay()
{
	engine->setSoundVolume(1);
	engine->stopAllSounds();
	playing = true;

	engine->play2D("sounds/level-music.mp3", true);
	level = 1;
	death_counter = 0;
	score = 0;
	strawberry_counter = 0;
	Strawberry_picked_up_in_level = false;

	scene.init(level, 0, death_counter, strawberry_counter);
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





