#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4       //el angulo que se suma al saltar
#define WALK_STEP 3
#define SPRING_ANGLE_STEP 3   //el angulo que se suma al utilizar un spring
#define JUMP_HEIGHT 96			//altura del salto
#define FALL_STEP 4			//velocidad a la que cae cuando acaba el salto
#define CLIMB_STEP 2			//velocidad a la que se cae cuando se esta CLIMB
#define WALL_JUMP_STEP 8		//incremento que se suma (o resta) en la componente 'x' y 'y' en cada instancia de wall jump


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB_LEFT, CLIMB_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	win = false;
	lose = false;
	bJumping = false;
	climb = false;
	jumpSpring = false;
	walljumpleft, walljumpright = false;
	past_f1, past_f3 = true; //para que no empieze a iniciar niveles a lo loco
	
	air = false;
	faceRight = false;
	moving = false;
	canJump = true; //canJump no se utiliza, la puse al principio para probar el walljump en clase
	dashing = false;

	spritesheet.loadFromFile("images/madeline.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.25f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.75f, 0.25f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.f));

		sprite->setAnimationSpeed(CLIMB_LEFT, 8);
		sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.5f, 0.75f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::updateKeys()
{
	keyUp = Game::instance().getSpecialKey(GLUT_KEY_UP);
	keyDown = Game::instance().getSpecialKey(GLUT_KEY_UP);
	keyLeft = Game::instance().getSpecialKey(GLUT_KEY_UP);
	keyRight = Game::instance().getSpecialKey(GLUT_KEY_UP);
}

void Player::doDash()
{
	if (keyUp)
	{
		if(keyLeft)  //arriba izq
		if(keyRight) //arriba derecha
		else //solo dash hacia arriba
	}
	else if (keyDown)
	{
		if (keyLeft)
		if (keyRight)
		else //solo dash hacia abajo
	}
	else if (keyLeft)
	{

	}
	else if (keyRight)
	{

	}
	
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	

	if (Game::instance().getSpecialKey(GLUT_KEY_F1) && !past_f1)
	{
		win = true;
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_F3) && !past_f3)
	{
		lose = true;
	}
	if (posPlayer.y <= -5) win = true;
	else if (posPlayer.y > 479) lose = true;


	//checks de si esta tocando la pared
	//indica si se puede hacer el walljump o si puede hacer climb
	//check left
	posPlayer.x -= WALK_STEP;
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) 
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !past_up && !canJump) //wall jump left to right
		{
			bJumping = false;
			walljumpleft = true;
			wallJumpProgress = 0;
		}
	}


	posPlayer.x += WALK_STEP*2;
	//check right
	if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) 
	{
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !past_up && !canJump) //wall jump right to left
		{
			bJumping = false;
			walljumpright = true;
			wallJumpProgress = 0;
		}
	}
	posPlayer.x -= WALK_STEP;

	if (!walljumpleft && !walljumpright) climb = false; //si no se toca ninguna de las dos paredes no puede estar haciendo climb

	if (air)
	{
		if (faceRight)
			sprite->changeAnimation(JUMP_RIGHT);
		else
			sprite->changeAnimation(JUMP_LEFT);
	}
	else if (!moving)
	{
		if (faceRight)
			sprite->changeAnimation(STAND_RIGHT);
		else
			sprite->changeAnimation(STAND_LEFT);
	}



	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		faceRight = false;
		moving = true;
		if (sprite->animation() != MOVE_LEFT && !air)
		{
				sprite->changeAnimation(MOVE_LEFT);
		}
		posPlayer.x -= WALK_STEP;
		
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) //antes 32
		{
			//canJump = true;
			sprite->changeAnimation(CLIMB_LEFT);
			climb = true; //cuando se toca una pared climb es true
			posPlayer.x += WALK_STEP;
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		faceRight = true;
		moving = true;
		if (sprite->animation() != MOVE_RIGHT && !air)
		{
				sprite->changeAnimation(MOVE_RIGHT);
		}
			
		posPlayer.x += WALK_STEP;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			sprite->changeAnimation(CLIMB_RIGHT);
			climb = true;
			posPlayer.x -= WALK_STEP;
		}
	}
	else
	{
		moving = false;
		climb = false; //si no se toca ninguna tecla no puedes hacer climb
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	
	if (dashing)
		doDash();
	else
	    updateKeys(); //actualizar que teclas se pulsan mienras no se dashea

	
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
				else posPlayer.y = int(startY - 150 * sin(3.14159f * jumpAngle / 180.f));

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
			air = false;
			if(!past_up && Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				air = true; //indica que esta en el aire
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
	past_f1 = Game::instance().getSpecialKey(GLUT_KEY_F1);
	past_f3 = Game::instance().getSpecialKey(GLUT_KEY_F3);
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
	air = true; //indica que esta en el aire
	jumpSpring = true;
	bJumping = true;
	canJump = false;
	jumpAngle = 0;
	startY = posPlayer.y;
}


bool Player::check_win() {
	return win;
}

bool Player::check_lose() {
	return lose;
}
