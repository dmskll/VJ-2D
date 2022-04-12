#include "Background.h"


//Te crea un rectangulo to guapo

void Background::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, glm::vec2 dimensions, glm::vec2 position)
{
	spritesheet.loadFromFile("images/Background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(dimensions, glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0, 0));

	tileMapDispl = tileMapPos;
	sprite->setPosition(position);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
}


void Background::render()
{
	sprite->render();
}


