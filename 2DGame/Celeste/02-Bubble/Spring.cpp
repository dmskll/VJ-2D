#include "Spring.h"

enum StrawberryAnims
{
	IDLE, USE
};


void Spring::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/springs.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(IDLE, 8);
		sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(USE, 8);
		sprite->addKeyframe(USE, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpring.x), float(tileMapDispl.y + posSpring.y)));
}

void Spring::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpring.x), float(tileMapDispl.y + posSpring.y)));
	posPlayer = player->getPosition();
	if (posPlayer.x < posSpring.x + 50 && posPlayer.x > posSpring.x - 20)
	{
		if (posPlayer.y < posSpring.y + 20 && posPlayer.y > posSpring.y - 20)
		{
			player->setJumpSpring();
			sprite->changeAnimation(USE);
			timeAnim = 25;
		}
	}
	if (timeAnim > 0) timeAnim -= 1;
	else sprite->changeAnimation(IDLE);
}

void Spring::render()
{
	sprite->render();
}

void Spring::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Spring::setPlayer(Player *p)
{
	player = p;
}
void Spring::setPosition(const glm::vec2 &pos)
{
	posSpring = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posSpring.x), float(tileMapDispl.y + posSpring.y)));
}
