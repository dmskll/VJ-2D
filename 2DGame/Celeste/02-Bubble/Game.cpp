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
		scene.init(level, t);
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
		scene.init(level, t);
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
	if (Game::instance().getKey(49)) {
		level = 1;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(50)) {
		level = 2;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(51)) {
		level = 3;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(52)) {
		level = 4;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(53)) {
		level = 5;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(54)) {
		level = 6;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(55)) {
		level = 7;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(56)) {
		level = 8;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(57)) {
		level = 9;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
		Strawberry_picked_up_in_level = false;
	}
	else if (Game::instance().getKey(48)) {
		level = 10;
		float t = scene.getTime();
		scene = Scene();
		scene.init(level, t);
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

	scene.init(level, 0);
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





