#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	level = 1;
	death_counter = 0;
	score = 0;
	strawberry_counter = 0;
	Strawberry_picked_up_in_level = false;
	scene.init(level);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);

	if (scene.check_win()) {
		level++;
		scene = Scene();
		scene.init(level);
		if (Strawberry_picked_up_in_level) {
			score += 1000;
			strawberry_counter++;
			Strawberry_picked_up_in_level = false;
		}
	}

	if (scene.check_lose()) {
		scene = Scene();
		death_counter++;
		scene.init(level);
		scene.setShake();
		Strawberry_picked_up_in_level = false;
	}
	if (scene.check_strawberry()) {
		Strawberry_picked_up_in_level = true;
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
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





