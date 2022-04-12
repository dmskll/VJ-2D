#pragma once

#include "Sprite.h"
#include "TileMap.h"


class plants
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int type);
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
private:
	int type;
	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite *sprite, *sprite2;
	TileMap *map;

};

