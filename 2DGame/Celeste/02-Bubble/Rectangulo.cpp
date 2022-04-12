#include "Rectangulo.h"


//Te crea un rectangulo to guapo

void Rectangulo::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, string colour, glm::vec2 dimensions, glm::vec2 position)
{
	spritesheet.loadFromFile("images/Colours.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(dimensions, glm::vec2(0.25, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(30);

	//white
	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.25,0.5));
	//red
	sprite->setAnimationSpeed(1, 8);
	sprite->addKeyframe(1, glm::vec2(0.25, 0));
	//black
	sprite->setAnimationSpeed(2, 8);
	sprite->addKeyframe(2, glm::vec2(0, 0));
	//blue
	sprite->setAnimationSpeed(3, 8);
	sprite->addKeyframe(3, glm::vec2(0, 0.5));
	//madeline red
	sprite->setAnimationSpeed(4, 8);
	sprite->addKeyframe(4, glm::vec2(0.75, 0));
	//cloud
	sprite->setAnimationSpeed(5, 8);
	sprite->addKeyframe(5, glm::vec2(0.5, 0.5));

	changeColour(colour);
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
void Rectangulo::changeColour(string c) {

	if (c == "white") {
		sprite->changeAnimation(0);
	}
	else if (c == "red") {
		sprite->changeAnimation(1);
	}
	else if (c == "black") {
		sprite->changeAnimation(2);
	}
	else if (c == "blue") {
		sprite->changeAnimation(3);
	}
	else if (c == "madelineRed") {
		sprite->changeAnimation(4);
	}
	else if (c == "cloud") {
		sprite->changeAnimation(5);
	}



}