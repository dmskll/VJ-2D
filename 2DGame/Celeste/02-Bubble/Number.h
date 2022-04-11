#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Number
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int number);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setNumber(int number);
	void increaseHeight();

private:
	glm::vec2 lastPosition;
	glm::ivec2 tileMapDispl, posNumber;
	int movAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};