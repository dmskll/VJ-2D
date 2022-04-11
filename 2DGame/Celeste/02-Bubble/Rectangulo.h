#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Rectangulo
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string colour, glm::vec2 dimensions, glm::vec2 position);
	void render();

	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMap *tileMap);

private:
	glm::ivec2 tileMapDispl, posNumber;
	int movAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};