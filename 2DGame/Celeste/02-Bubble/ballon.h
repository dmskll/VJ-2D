#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Ballon
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
	glm::ivec2 tileMapDispl, posBallon, posPlayer;
	int movAngle, startY;
	float cd_ballon;
	Texture spritesheet, spritesheet2;
	Sprite *sprite, *sprite_string;
	TileMap *map;
	Player *player;

};

