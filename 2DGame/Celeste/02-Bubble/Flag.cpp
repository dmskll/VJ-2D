#include "Flag.h"
#include "Scene.h"



void Flag::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	
	spritesheet.loadFromFile("images/flag-tr.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(0, 4);
		sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(0, glm::vec2(0.f, 0.5f));
		sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));
	touched = false;
}

void Flag::setLevel(Scene *scene)
{
	level = scene;
}

void Flag::update(int deltaTime)
{
	sprite->update(deltaTime);

	posPlayer = player->getPosition();
	if (posPlayer.x < posFlag.x + 25 && posPlayer.x > posFlag.x - 20)
	{
		if (posPlayer.y < posFlag.y + 25 && posPlayer.y > posFlag.y - 20)
		{
			touched = true;
			level->setTouchFlag();
		}
	}
}

void Flag::render()
{
	sprite->render();
}

void Flag::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Flag::setPlayer(Player *p)
{
	player = p;
}

void Flag::setPosition(const glm::vec2 &pos)
{
	posFlag = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + pos.x), float(tileMapDispl.y + pos.y)));
}

