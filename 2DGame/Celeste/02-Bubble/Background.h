#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Background
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 dimensions, glm::vec2 position);
	void render();

private:
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};