#include "Spike.h"


//Spike que spikea
//Orientaciónes: 1 = mirando hacia arriba, 2 = abajo, 3 = derecha, 4 = izquierda
//la posición se indica en CASILLAS, no pixeles individuales (1 casilla = cuadrado de mapa)

void Spike::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int orientation, glm::vec2 pos, Player *p) {
	spritesheet.loadFromFile("images/spikes.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0, 0));
	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0.5, 0));
	sprite->setAnimationSpeed(3, 8);
	sprite->addKeyframe(3, glm::vec2(0, 0.5));
	sprite->setAnimationSpeed(4, 8);
	sprite->addKeyframe(4, glm::vec2(0.5,0.5));



	tileMapDispl = tileMapPos;
	this->setPosition(pos);


	borderHigh = (posSpike - 1) * 32;
	borderLow = (posSpike + 1) * 32;

	switch (orientation) {//modificar caja decolisión segun orientación

	case 1://mirando hacia arriba
		borderHigh.y += 25;
		break;

	case 2://mirando hacia abajo
		borderLow.y -= 25;
		break;

	case 3://mirando hacia la derecha
		borderLow.x -= 25;
		break;

	case 4://mirando hacia la izquierda

		borderHigh.x += 25;
		break;
	}


	sprite->changeAnimation(orientation);
	setPlayer(p);
}

void Spike::update()
{
	posPlayer = player->getPosition();

	if (posPlayer.x > borderHigh.x && posPlayer.x < borderLow.x)
	{
		if (posPlayer.y > borderHigh.y  && posPlayer.y < borderLow.y)
		{
			player->touchSpike();
		}
	}
}

void Spike::render()
{
	sprite->render();
}

void Spike::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Spike::setPlayer(Player *p)
{
	player = p;
}
void Spike::setPosition(const glm::vec2 &pos)
{
	posSpike = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + 32 * posSpike.x), float(tileMapDispl.y + 32 * posSpike.y)));
}
