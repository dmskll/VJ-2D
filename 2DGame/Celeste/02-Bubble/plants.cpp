#include "plants.h"

enum PlantType
{
	GRASS1, GRASS2, FLOWER, BUSH
};

void plants::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int t)
{
	spritesheet.loadFromFile("images/plants.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	this->type = t;

	if (type == GRASS1)
	{
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
	}
	else if (type == GRASS2)
	{
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.2f));
	}
	else if (type == FLOWER)
	{
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.4f));
	}
	else if (type == BUSH)
	{
		sprite2 = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 0.2), &spritesheet, &shaderProgram);
		sprite2->setNumberAnimations(1);

		//tronco
		sprite->setAnimationSpeed(0, 8);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.8f));

		//hojas
		sprite2->setAnimationSpeed(0, 8);
		sprite2->addKeyframe(0, glm::vec2(0.f, 0.6f));
		sprite2->changeAnimation(0);
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
	if (type == BUSH)
		sprite2->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
}

void plants::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (type == BUSH)
		sprite2->update(deltaTime);
}

void plants::render()
{
	sprite->render();
	if (type == BUSH)
		sprite2->render();
}

void plants::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void plants::setPosition(const glm::vec2 &pos)
{
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
	if (type == BUSH)
		sprite2->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y - 30)));
}

