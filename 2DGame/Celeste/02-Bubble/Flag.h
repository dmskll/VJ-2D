#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Scene;
class Flag
{
public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setPlayer(Player *p);
	void setLevel(Scene *scene);

private: 
	bool touched;
	glm::ivec2 tileMapDispl, posPlayer, posFlag;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;
	Scene *level;
};

