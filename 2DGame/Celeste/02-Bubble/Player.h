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
	void touchBallon();
	glm::ivec2 getPosition();
	
private:
	void updateDash();
	void doDash();
	void updateWallJump();
	void updateMeta(); //cosas relacionadas con el god mode/ganar/perder
	void horizontalMovement();
	void updatePressedKeys();
	void updateAnimations();


	bool bJumping, canJump, climb, walljumpleft, walljumpright, jumpSpring;
	bool past_C, past_X, past_down, past_left, past_right, past_f1, past_f3, past_D;;//booleanos que indican si la tecla estaba presionada en el ultimo update
	bool air; //indica si el jugador esta en el aire, se utiliza en las animaciones
	bool faceRight, moving;
	bool keyUp, keyLeft, keyDown, keyRight, dashing, canDash, godDash;

	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY, wallJumpProgress, floatTime, dashTime, cd_dash; //TOFIX EL TIMER DEBERIA DEPENDER DEL DELTA TIME
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


