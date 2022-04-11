#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Spike
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int orientación, glm::vec2 pos, Player *p);
	void update();
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player *p);

private:
	glm::ivec2 tileMapDispl, posSpike, posPlayer;
	glm::vec2 borderHigh, borderLow;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;

};

