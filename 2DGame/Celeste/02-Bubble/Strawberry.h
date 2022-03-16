#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Strawberry
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player *p);

	private:
		bool collected;
		glm::ivec2 tileMapDispl, posStrawBerry, posPlayer;
		int movAngle, startY;
		Texture spritesheet;
		Sprite *sprite;
		TileMap *map;
		Player *player;

};

