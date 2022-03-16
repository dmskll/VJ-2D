#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"


class Spring
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player *p);

private:
	glm::ivec2 tileMapDispl, posSpring, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;
};

