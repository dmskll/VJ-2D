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
	engine = SoundControl::instance().getSoundEngine();

	spawning = true;
	spawnSpeed = 0.5f;
	spawnDeceleration = 0.003f;
	engine->play2D("sounds/next-level.wav", false);

	win = false;
	lose = false;
	pre_lose = false;
	bJumping = false;
	climb = false;
	jumpSpring = false;
	strawberry = false;
	walljumpleft = false;
	walljumpright = false;

	past_f1 = true;
	past_f3 = true; //para que no empieze a iniciar niveles a lo loco
	
	air = false;
	faceRight = true;
	moving = false;

	canJump = true; 
	dashing = false;
	canDash = true;
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
		
	sprite->changeAnimation(1);
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
			engine->play2D("sounds/jump-wall.wav", false);
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
			engine->play2D("sounds/jump-wall.wav", false);
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
			engine->play2D("sounds/dash.wav", false);

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
	else
	{
		if ((Game::instance().getKey(120) || Game::instance().getKey(88)) && !past_X)
		{
			engine->play2D("sounds/no-dash.wav", false);
		}

	}
}

void Player::doDash()
{
	dashSpeed += dashDeceleration;
	if (dashSpeed < 0) dashing = false;

	if (keyUp)
	{
		if (keyLeft)  //arriba izq
		{
			if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				posPlayer.y -= dashSpeed * 0.8;
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) 
				moveLeft(dashSpeed * 0.8);
		}
		else if (keyRight) //arriba derecha
		{
			if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				posPlayer.y -= dashSpeed * 0.8;
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) 
				moveRight(dashSpeed * 0.8);
		}
		else //solo dash hacia arriba
			if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				posPlayer.y -= dashSpeed;

		startY = posPlayer.y;
	}
	else if (keyDown)
	{
		if (keyLeft)
		{
			if (!map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				moveDown(dashSpeed * 0.8);
			if (!map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
				moveLeft(dashSpeed * 0.8);
		}
		else if (keyRight)
		{
			if (!map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				moveDown(dashSpeed * 0.8);
			if (!map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))  
				moveRight(dashSpeed * 0.8);
		}
		else //solo dash hacia abajo
			if (!map->collisionMoveDown(posPlayer, glm::ivec2(16, 16), &posPlayer.y)) 
				moveDown(dashSpeed);

		startY = posPlayer.y;
	}
	else if (keyLeft || !faceRight)
	{
		if (!map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32))) 
			moveLeft(dashSpeed);
	}
	else if (keyRight || faceRight)
	{
		if (!map->collisionMoveRight(posPlayer, glm::ivec2(32, 32))) 
			moveRight(dashSpeed);
	}	
}

void Player::moveLeft(float distance)
{
	float pool;
	pool = 0;
	glm::ivec2 posAux = posPlayer;

	for (int i = 0; i < (distance * 2) && !map->collisionMoveLeft(posAux, glm::ivec2(32, 32)); i++) {
		pool += 0.5f;
		if (pool == 1.f)
		{
			posAux.x -= 1;
		}
		else if (pool == 2.f)
		{
			posAux.x -= 1;
			posPlayer.x -= 2;
			pool = 0;
		}
	}
}

void Player::moveRight(float distance)
{
	float pool;
	pool = 0;
	glm::ivec2 posAux = posPlayer;

	for (int i = 0; i < (distance * 2) && !map->collisionMoveRight(posAux, glm::ivec2(32, 32)); i++) {
		pool += 0.5f;
		if (pool == 1.f)
		{
			posAux.x += 1;
		}
		else if (pool == 2.f)
		{
			posAux.x += 1;
			posPlayer.x += 2;
			pool = 0;
		}
	}
}





void Player::moveDown(float distance) {
	for (int i = 0; i < distance && !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y); i++) {
		posPlayer.y += 1;
	}

	posPlayer.y += 1;

}

void Player::updateMeta(int deltaTime)
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
	else if (posPlayer.y > 479 && !pre_lose && !spawning)
	{
		engine->play2D("sounds/muerte.wav", false);
		level->setShake();
		pre_lose = true;
		cd_lose = 300;
	}

	if (pre_lose)
	{
		if (cd_lose > 0)
			cd_lose -= 1.f * deltaTime;
		else
			lose = true;
	}


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

void Player::doSpawn(int deltaTime)
{
	if (posPlayer.y > spawnY)
	{
		posPlayer.y += -spawnSpeed * deltaTime;
	}
	else
	{
		spawnSpeed += -spawnDeceleration * deltaTime;
		posPlayer.y += -spawnSpeed * deltaTime;

		if (posPlayer.y > spawnY)
		{
			posPlayer.y = spawnY;
			level->setShake();
			spawning = false;
		}
	}

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	updateMeta(deltaTime); //cosas relacionadas con el godmode o ganar/perder

	if (spawning)
	{
		doSpawn(deltaTime);
	}
	else if (!pre_lose)
	{
		sprite->update(deltaTime);

		updateWallJump();		//actualizar si se esta tocando una pared
		horizontalMovement();	//gestiona el movimiento horizontal
		updateAnimations();		//gestiona animaciones 

		updateDash();			//actualiza parametros del dash
		if (dashing)
			doDash();			//ejecuta el dash
		else if (bJumping) //CALCULO DE SALTO
		{
			//con bJumping true se calcula el salto parabolico. Cuando se llega a angle 180, el jugador
			//toca el suelo (con angle > 90) 0 el jugador se choca con el techo bJumping pasa a ser false

			if (!past_C && Game::instance().getSpecialKey(GLUT_KEY_UP) && canJump)
			{
				engine->play2D("sounds/jump.wav", false);
				canJump = false;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
			if (!jumpSpring) jumpAngle += JUMP_ANGLE_STEP;
			else jumpAngle += SPRING_ANGLE_STEP;

			if (jumpAngle == 180)
			{
				canJump = true;
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				if (!map->collisionMoveUp(posPlayer, glm::ivec2(16, 16), &posPlayer.y))
				{
					if (!jumpSpring) posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
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
		
			moveDown(distance);


			if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
			{

				//gestionar si se puede o no se puede hacer dash
				//canDash tmb se utiliza para las animaciones por eso hay que mirar el timer
				//porque si haces dash hacia arriba de otra forma sigue siendo true
				if (dashing)
				{
					if (dashTime < 7)
					{
						if (!canDash) engine->play2D("sounds/can-dash.wav", false);
						canDash = true;
					}
				}
				else
				{
					if(!canDash) 
						engine->play2D("sounds/can-dash.wav", false);
					canDash = true;				
				}


				canJump = true;
				air = false;
				if (!past_C && (Game::instance().getKey(67) || Game::instance().getKey(99)))
				{
					engine->play2D("sounds/jump.wav", false);
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
	}

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
	if(!pre_lose) sprite->render();
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
	spawnX = pos.x;
	spawnY = pos.y;

	posPlayer = glm::vec2 (pos.x, 600);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

void Player::setJumpSpring()
{
	engine->play2D("sounds/muelle.wav", false);
	air = true; //indica que esta en el aire
	jumpSpring = true;
	bJumping = true;
	canJump = false;
	canDash = true;
	dashing = false;
	jumpAngle = 0;
	startY = posPlayer.y;
}

void Player::touchBallon()
{
	canDash = true;
}

void Player::touchStrawBerry() {
	strawberry = true;
}

bool Player::check_win() {
	return win;
}

bool Player::check_lose() {
	return lose;
}

bool Player::check_strawberry() {
	if (strawberry) {
		strawberry = false;
		return true;
	}
	return false;
}
