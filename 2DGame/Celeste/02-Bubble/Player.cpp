#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "Scene.h"


#define JUMP_ANGLE_STEP 4       //el angulo que se suma al saltar
#define WALK_STEP 4		//antes a 3 pero 4 es mas parecida al juego original		
#define SPRING_ANGLE_STEP 3   //el angulo que se suma al utilizar un spring
#define JUMP_HEIGHT 96			//altura del salto
#define FALL_STEP 4	 //antes 5 pero creo que el celeste es mas lento		//velocidad a la que cae cuando acaba el salto
#define CLIMB_STEP 2			//velocidad a la que se cae cuando se esta CLIMB
#define WALL_JUMP_STEP 6		//incremento que se suma (o resta) en la componente 'x' y 'y' en cada instancia de wall jump
#define DASH_STEP 10


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, JUMP_LEFT, JUMP_RIGHT, CLIMB_LEFT, CLIMB_RIGHT, CLIMB_LEFT_DASH, CLIMB_RIGHT_DASH, JUMP_LEFT_DASH, JUMP_RIGHT_DASH, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
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
	canDash = false;
	dashTime = 9; //se tiene que mover 5tiles
	//cd_dash = 0;

	dashInitSpeed = 13;
	dashDeceleration = -0.7f;

	//god mode 
	godDash = false; 

	spritesheet.loadFromFile("images/madeline.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.20), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(16);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.20f)); //horizontal, vertical
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.20f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.20f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.20f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));

		sprite->setAnimationSpeed(JUMP_LEFT, 8);
		sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.75f, 0.20f));

		sprite->setAnimationSpeed(JUMP_LEFT_DASH, 8);
		sprite->addKeyframe(JUMP_LEFT_DASH, glm::vec2(0.75f, 0.8f));

		sprite->setAnimationSpeed(JUMP_RIGHT, 8);
		sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.75f, 0.f));

		sprite->setAnimationSpeed(JUMP_RIGHT_DASH, 8);
		sprite->addKeyframe(JUMP_RIGHT_DASH, glm::vec2(0.5f, 0.8f));

		sprite->setAnimationSpeed(CLIMB_LEFT, 8);
		sprite->addKeyframe(CLIMB_LEFT, glm::vec2(0.5f, 0.4f));

		sprite->setAnimationSpeed(CLIMB_LEFT_DASH, 8);
		sprite->addKeyframe(CLIMB_LEFT_DASH, glm::vec2(0.f, 0.8f));

		sprite->setAnimationSpeed(CLIMB_RIGHT, 8);
		sprite->addKeyframe(CLIMB_RIGHT, glm::vec2(0.5f, 0.6f));

		sprite->setAnimationSpeed(CLIMB_RIGHT_DASH, 8);
		sprite->addKeyframe(CLIMB_RIGHT_DASH, glm::vec2(0.25f, 0.8f));

		sprite->setAnimationSpeed(DOWN_LEFT, 8);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0.f, 0.6f));

		sprite->setAnimationSpeed(DOWN_RIGHT, 8);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.f, 0.4f));

		sprite->setAnimationSpeed(UP_LEFT, 8);
		sprite->addKeyframe(UP_LEFT, glm::vec2(0.25f, 0.6f));

		sprite->setAnimationSpeed(UP_RIGHT, 8);
		sprite->addKeyframe(UP_RIGHT, glm::vec2(0.25f, 0.4f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::updateWallJump()
{
	//checks de si esta tocando la pared
	//indica si se puede hacer el walljump o si puede hacer climb
	//check left
	posPlayer.x -= WALK_STEP;
	if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
	{
		if ((Game::instance().getKey(67) || Game::instance().getKey(99)) && !past_C && !canJump) //wall jump left to right
		{
			bJumping = false;
			walljumpleft = true;
			wallJumpProgress = 0;
		}
	}


	posPlayer.x += WALK_STEP * 2;
	//check right
	if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
	{
		if ((Game::instance().getKey(67) || Game::instance().getKey(99)) && !past_C && !canJump) //wall jump right to left
		{
			bJumping = false;
			walljumpright = true;
			wallJumpProgress = 0;
		}
	}
	posPlayer.x -= WALK_STEP;

	if (!walljumpleft && !walljumpright) climb = false; //si no se toca ninguna de las dos paredes no puede estar haciendo climb
}

void Player::updateDash()
{
	if (!dashing)
	{
		keyUp = Game::instance().getSpecialKey(GLUT_KEY_UP);
		keyDown = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
		keyLeft = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
		keyRight = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
	}
	if (canDash)
	{
		if ((Game::instance().getKey(120) || Game::instance().getKey(88)) && !past_X)
		{
			level->setShake();
			dashing = true;
			bJumping = false; //si se hace un dash ya no se salta
			//dashTime = 12;
			dashSpeed = dashInitSpeed;

			if (!godDash) 
			{
				canDash = false; //si el godDash está a true no ponemos el canDash a false
			}
			else
			{
				canDash = true;
			}			
		}
	}
}

void Player::doDash()
{
	/*
	if (dashTime < 0)
	{
		dashing = false;
		//if(!godDash) canDash = false;
	}
	else
		dashTime -= 1;
	*/

	dashSpeed += dashDeceleration;
	if (dashSpeed < 0) dashing = false;

	if (keyUp)
	{
		if (keyLeft)  //arriba izq
		{
			posPlayer.y -= dashSpeed * 0.8;
			posPlayer.x -= dashSpeed * 0.8;
		}
		else if (keyRight) //arriba derecha
		{
			posPlayer.y -= dashSpeed * 0.8;
			posPlayer.x += dashSpeed * 0.8;
		}
		else //solo dash hacia arriba
			posPlayer.y -= dashSpeed;

		startY = posPlayer.y;
	}
	else if (keyDown)
	{
		if (keyLeft)
		{
			posPlayer.y += dashSpeed * 0.8;
			posPlayer.x -= dashSpeed * 0.8;
		}
		else if (keyRight)
		{
			posPlayer.y += dashSpeed * 0.8;
			posPlayer.x += dashSpeed * 0.8;
		}
		else //solo dash hacia abajo
			posPlayer.y += dashSpeed;

		startY = posPlayer.y;
	}
	else if (keyLeft || !faceRight)
	{
		posPlayer.x -= dashSpeed;
	}
	else if (keyRight || faceRight)
	{
		posPlayer.x += dashSpeed;
	}	
}

void Player::updateMeta()
{
	if (Game::instance().getSpecialKey(GLUT_KEY_F1) && !past_f1)
	{
		win = true;
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_F3) && !past_f3)
	{
		lose = true;
	}
	if((Game::instance().getKey(68) || Game::instance().getKey(100)) && !past_D)
	{
		godDash = !godDash;
	}

	if (posPlayer.y <= -5) win = true;
	else if (posPlayer.y > 479) lose = true;
}

void Player::horizontalMovement()
{
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !dashing)
	{
		faceRight = false;
		moving = true;
		if (!walljumpright && !walljumpleft) posPlayer.x -= WALK_STEP;

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) //antes 32
		{
			//canJump = true;
			sprite->changeAnimation(CLIMB_LEFT);
			climb = true; //cuando se toca una pared climb es true
			if (!walljumpright && !walljumpleft) posPlayer.x += WALK_STEP;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && !dashing)
	{
		faceRight = true;
		moving = true;
		if (!walljumpright && !walljumpleft) posPlayer.x += WALK_STEP;
		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			
			climb = true;
			if (!walljumpright && !walljumpleft) posPlayer.x -= WALK_STEP;
		}
	}
	else //no se mueve
	{
		moving = false;
		climb = false; //si no se toca ninguna tecla no puedes hacer climb
	}

}

void Player::updateAnimations()
{
	if (air || dashing) // or dashing para que se actualice bien el pelo y la animacion al dashear
	{
		if (climb)
		{
			if (faceRight)
			{
				if (canDash)
					sprite->changeAnimation(CLIMB_RIGHT);
				else
					sprite->changeAnimation(CLIMB_RIGHT_DASH);
			}
			else
			{
				if (canDash)
					sprite->changeAnimation(CLIMB_LEFT);
				else
					sprite->changeAnimation(CLIMB_LEFT_DASH);
			}
		}
		else
		{
			if (faceRight)
			{
				if (canDash)
					sprite->changeAnimation(JUMP_RIGHT);
				else
					sprite->changeAnimation(JUMP_RIGHT_DASH);
			}
			else
			{
				if (canDash)
					sprite->changeAnimation(JUMP_LEFT);
				else
					sprite->changeAnimation(JUMP_LEFT_DASH);
			}
		}
	}
	else if (!moving)
	{
		if (faceRight)
			if(keyDown) sprite->changeAnimation(DOWN_RIGHT);
			else if (keyUp) sprite->changeAnimation(UP_RIGHT);
			else sprite->changeAnimation(STAND_RIGHT);
			
		else
			if (keyDown) sprite->changeAnimation(DOWN_LEFT);
			else if (keyUp) sprite->changeAnimation(UP_LEFT);
			else sprite->changeAnimation(STAND_LEFT);
			
	}
	else if (climb)
	{
		if (faceRight)
			sprite->changeAnimation(CLIMB_RIGHT);
		else
			sprite->changeAnimation(CLIMB_LEFT);
	}
	else if (moving)
	{
		if (faceRight)
		{
			if (sprite->animation() != MOVE_RIGHT)
			{
				sprite->changeAnimation(MOVE_RIGHT);
			}
		}
		else
		{
			if (sprite->animation() != MOVE_LEFT)
			{
				sprite->changeAnimation(MOVE_LEFT);
			}
		}
	}
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);

	updateMeta();			//cosas relacionadas con el godmode o ganar/perder
	updateWallJump();		//actualizar si se esta tocando una pared
	horizontalMovement();	//gestiona el movimiento horizontal
	updateAnimations();		//gestiona animaciones 

	updateDash();			//actualiza parametros del dash
	if (dashing)
		doDash();			//ejecuta el dash
	else if(bJumping) //CALCULO DE SALTO
	{
		//con bJumping true se calcula el salto parabolico. Cuando se llega a angle 180, el jugador
		//toca el suelo (con angle > 90) 0 el jugador se choca con el techo bJumping pasa a ser false

		if (!past_C && Game::instance().getSpecialKey(GLUT_KEY_UP) && canJump)
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
				bJumping = false;
			}
		}
	}
	else if (walljumpleft) {
		if (wallJumpProgress == 16) {
			walljumpleft = false;
			floatTime = 10;
		}
		else {
			wallJumpProgress += 1;
			posPlayer.x += WALL_JUMP_STEP;
			posPlayer.y -= WALL_JUMP_STEP;

			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				walljumpleft = false;
				posPlayer.x -= WALL_JUMP_STEP*0.8;
				posPlayer.y += WALL_JUMP_STEP*0.8;
				floatTime = 10;

			}

		}

	}
	else if (walljumpright) {
		if (wallJumpProgress == 16) {
			walljumpright = false;
			floatTime = 10;
		}
		else {
			wallJumpProgress += 1;
			posPlayer.x -= WALL_JUMP_STEP;
			posPlayer.y -= WALL_JUMP_STEP;

			if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32), &posPlayer.y) || map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) {
				walljumpright = false;
				posPlayer.x += WALL_JUMP_STEP*0.8;
				posPlayer.y += WALL_JUMP_STEP*0.8;
				floatTime = 10;

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


		int distance;

		if (climb) distance = CLIMB_STEP;
		else distance = FALL_STEP;

		
		for (int i = 0; i < distance && !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y); i++) {
			posPlayer.y += 1;
		}

		posPlayer.y += 1;


		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{

			//gestionar si se puede o no se puede hacer dash
			//canDash tmb se utiliza para las animaciones por eso hay que mirar el timer
			//porque si haces dash hacia arriba de otra forma sigue siendo true
			if (dashing)
			{
				if(dashTime < 7)
					canDash = true;
			}
			else
				canDash = true;
				

			canJump = true;
			air = false;
			if(!past_C && (Game::instance().getKey(67) || Game::instance().getKey(99)))
			{
				air = true; //indica que esta en el aire
				bJumping = true;
				canJump = false;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		else
		{
			air = true; //si no toca el suelo esta en el aire
			canJump = false;
		}
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	//actualizar valores para el siguiente update
	updatePressedKeys();
}

void Player::updatePressedKeys()
{
	//actualizar valores para el siguiente update
	past_C = (Game::instance().getKey(67) || Game::instance().getKey(99));
	past_X = (Game::instance().getKey(120) || Game::instance().getKey(88));
	past_down = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
	past_left = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
	past_right = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
	past_f1 = Game::instance().getSpecialKey(GLUT_KEY_F1);
	past_f3 = Game::instance().getSpecialKey(GLUT_KEY_F3);
	past_D = (Game::instance().getKey(68) || Game::instance().getKey(100));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setLevel(Scene *scene)
{
	level = scene;
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
	canDash = true;
	jumpAngle = 0;
	startY = posPlayer.y;
}

void Player::touchBallon()
{
	canDash = true;
}


bool Player::check_win() {
	return win;
}

bool Player::check_lose() {
	return lose;
}
