#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	bool  win, lose;
	bool check_win();
	bool check_lose();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void setJumpSpring();
	void updateKeys();
	void doDash();
	glm::ivec2 getPosition();
	
private:
	bool bJumping, canJump, climb, walljumpleft, walljumpright, jumpSpring;
	bool past_up, past_down, past_left, past_right, past_f1, past_f3;;//booleanos que indican si la tecla estaba presionada en el ultimo update
	bool air; //indica si el jugador esta en el aire, se utiliza en las animaciones
	bool faceRight, moving;
	bool keyUp, keyLeft, keyDown, keyRight, dashing;

	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, wallJumpProgress, floatTime, dashTime; //TOFIX EL TIMER DEBERIA DEPENDER DEL DELTA TIME
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


