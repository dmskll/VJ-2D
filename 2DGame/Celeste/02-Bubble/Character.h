#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Character
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setCharacter(char ch, string colour);

private:
	glm::vec2 lastPosition;
	glm::ivec2 tileMapDispl, posNumber;
	int movAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};