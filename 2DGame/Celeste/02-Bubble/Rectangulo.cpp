#include "Rectangulo.h"


//Te crea un rectangulo to guapo

void Rectangulo::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string colour, glm::vec2 dimensions, glm::vec2 position)
{
	spritesheet.loadFromFile("images/Numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(dimensions, glm::vec2(0.033333, 0.333333), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(30);

	//white
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0,0));
	//red
	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0, 0.3333));
	//black
	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0, 0.6666));

	if (colour == "white") {
		sprite->changeAnimation(0);
	}
	else if (colour == "red") {
		sprite->changeAnimation(1);
	}
	else if (colour == "black") {
		sprite->changeAnimation(2);
	}



	tileMapDispl = tileMapPos;
	sprite->setPosition(position);
	movAngle = 0;
}


void Rectangulo::render()
{
	sprite->render();
}

void Rectangulo::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Rectangulo::setPosition(const glm::vec2 &pos)
{
	posNumber = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posNumber.x), float(tileMapDispl.y + posNumber.y)));
}
