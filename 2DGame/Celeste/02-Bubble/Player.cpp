#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4       //el angulo que se suma al saltar
#define SPRING_ANGLE_STEP 3   //el angulo que se suma al utilizar un spring
#define JUMP_HEIGHT 96			//altura del salto
#define FALL_STEP 4			//velocidad a la que cae cuando acaba el salto
#define CLIMB_STEP 2			//velocidad a la que se cae cuando se esta CLIMB
#define WALL_JUMP_STEP 8		//incremento que se suma (o resta) en la componente 'x' y 'y' en cada instancia de wall jump


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	climb = false;
	jumpSpring = false;
	walljumpleft = false;
	walljumpright = false;

	canJump = true; //canJump no se utiliza, la puse al principio para probar el walljump en clase
	
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}



void Player::update(int deltaTime)
{
	sprite->update(deltaTime);


	//checks de si esta tocando la pared
	//indica si se puede hacer el walljump o si puede hacer climb
	//check left
	posPlayer.x -= 2;
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) 
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !past_up && !canJump) //wall jump left to right
		{
			bJumping = false;
			walljumpleft = true;
			wallJumpProgress = 0;
		}
	}
	else climb = false; //si no toca ninguna pared no puede hacer climb

	posPlayer.x += 2;
	//check right
	if (map->collisionMoveRight(posPlayer, glm::ivec2(34, 32))) 
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !past_up && !canJump) //wall jump right to left
		{
			bJumping = false;
			walljumpright = true;
			wallJumpProgress = 0;
		}
	}
	else climb = false;

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= 2;
		
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) //antes 32
		{
			//canJump = true;
			climb = true; //cuando se toca una pared climb es true
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += 2;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			climb = true;
			posPlayer.x -= 2;

			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		climb = false; //si no se toca ninguna tecla no puedes hacer climb
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	//CALCULO DE SALTO
	if(bJumping) 
	{
		//con bJumping true se calcula el salto parabolico. Cuando se llega a angle 180, el jugador
		//toca el suelo (con angle > 90) 0 el jugador se choca con el techo bJumping pasa a ser false

		if (!past_up && Game::instance().getSpecialKey(GLUT_KEY_UP) && canJump)
		{
			canJump = false;
			jumpAngle = 0;
			startY = posPlayer.y;
		}

		if(!jumpSpring) jumpAngle += JUMP_ANGLE_STEP;
		else jumpAngle += SPRING_ANGLE_STEP;

		if(jumpAngle == 180)
		{
			canJump = true;
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y))
			{
				if(!jumpSpring) posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				else posPlayer.y = int(startY - 190 * sin(3.14159f * jumpAngle / 180.f));

				if (jumpAngle > 90)
					if (climb) bJumping = false;
					else bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 52), &posPlayer.y); //antes 32 32
			}
			else
			{
				//jumpAngle += 90;
				bJumping = false;
			}

		}
	}
	else if (walljumpleft) {
		if (wallJumpProgress == 10) { 
			walljumpleft = false;
			floatTime = 5;
		}
		else {
			wallJumpProgress += 1;
			posPlayer.x += WALL_JUMP_STEP;
			posPlayer.y -= WALL_JUMP_STEP;

			if(map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				walljumpleft = false;
				posPlayer.x -= WALL_JUMP_STEP;
				posPlayer.y += WALL_JUMP_STEP;
				floatTime = 5;
				
			}

		}

	}
	else if (walljumpright) {
		if (wallJumpProgress == 10) {
			walljumpright = false;
			floatTime = 5;
		}
		else {
			wallJumpProgress += 1;
			posPlayer.x -= WALL_JUMP_STEP;
			posPlayer.y -= WALL_JUMP_STEP;

			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				walljumpright= false;
				posPlayer.x += WALL_JUMP_STEP;
				posPlayer.y += WALL_JUMP_STEP;
				floatTime = 5;

			}

		}

	}
	else if (floatTime > 0) {
		floatTime -= 1;
	}
	else
	{
		//caida no parabolica dependiendo de si se esta climb o no
		jumpSpring = false;
		if (climb) posPlayer.y += CLIMB_STEP;
		else posPlayer.y += FALL_STEP;
		
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			canJump = true;
			if(!past_up && Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				canJump = false;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		else canJump = false;
	}
	

	



	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//actualizar valores para el siguiente update
	past_up = Game::instance().getSpecialKey(GLUT_KEY_UP);
	past_down = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
	past_left = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
	past_right = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

void Player::setJumpSpring()
{
	jumpSpring = true;
	bJumping = true;
	canJump = false;
	jumpAngle = 0;
	startY = posPlayer.y;
}



