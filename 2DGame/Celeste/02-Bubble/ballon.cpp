#include "ballon.h"


//la clase fresa es bastante parecida a la clase player. Es un objeto que levita y 
//desaparece cuando un jugador la toca
enum BallonAnims
{
	IDLE, STRING
};

void Ballon::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/ballon.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y)));

	sprite_string = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite_string->setNumberAnimations(1);

		sprite_string->setAnimationSpeed(0, 2);
			sprite_string->addKeyframe(0, glm::vec2(0.5f, 0.f));
			sprite_string->addKeyframe(0, glm::vec2(0.f, 0.5f));
			sprite_string->addKeyframe(0, glm::vec2(0.5f, 0.5f));

	sprite_string->changeAnimation(0);
	sprite_string->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y + 30)));

	movAngle = 0;
	collected = false;
}

void Ballon::update(int deltaTime)
{
	if (!collected) //si la fresa se ha recogido ya no se mueve
	{
		sprite->update(deltaTime);
		sprite_string->update(deltaTime);

		movAngle += 3;
		posBallon.y = int(startY - 2 * sin(3.14159f * movAngle / 180.f));
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y)));
		sprite_string->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y + 30)));

		posPlayer = player->getPosition();
		if (posPlayer.x < posBallon.x + 25 && posPlayer.x > posBallon.x - 20)
		{
			if (posPlayer.y < posBallon.y + 25 && posPlayer.y > posBallon.y - 20)
			{
				player->touchBallon();
				collected = true;
				cd_ballon = 5;
			}
		}
	}
	else
	{
		cd_ballon -= 0.001f * deltaTime;
		if (cd_ballon < 0)
			collected = false;
	}
}

void Ballon::render()
{
	//si la fresa se ha recogido no se tiene que hacer render
	if (!collected)
	{
		sprite_string->render();
		sprite->render();
	}
}

void Ballon::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Ballon::setPlayer(Player *p)
{
	player = p;
}
void Ballon::setPosition(const glm::vec2 &pos)
{
	posBallon = pos;
	startY = posBallon.y; //startY es la posicion desde la que se levita
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y)));
	sprite_string->setPosition(glm::vec2(float(tileMapDispl.x + posBallon.x), float(tileMapDispl.y + posBallon.y + 30)));
}