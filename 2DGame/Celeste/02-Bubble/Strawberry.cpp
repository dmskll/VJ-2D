#include "Strawberry.h"


//la clase fresa es bastante parecida a la clase player. Es un objeto que levita y 
//desaparece cuando un jugador la toca
enum StrawberryAnims
{
	IDLE
};

void Strawberry::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	engine = SoundControl::instance().getSoundEngine();

	spritesheet.loadFromFile("images/berries.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

		sprite->setAnimationSpeed(IDLE, 8);
		sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posStrawBerry.x), float(tileMapDispl.y + posStrawBerry.y)));
	movAngle = 0;
	collected = false;
}

void Strawberry::update(int deltaTime)
{
	if (!collected) //si la fresa se ha recogido ya no se mueve
	{
		sprite->update(deltaTime);
		movAngle += 3;
		posStrawBerry.y = int(startY - 12 * sin(3.14159f * movAngle / 180.f));
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posStrawBerry.x), float(tileMapDispl.y + posStrawBerry.y)));

		posPlayer = player->getPosition();
		if (posPlayer.x < posStrawBerry.x + 25 && posPlayer.x > posStrawBerry.x - 20)
		{
			if (posPlayer.y < posStrawBerry.y + 25 && posPlayer.y > posStrawBerry.y - 20)
			{
				engine->play2D("sounds/fresa.wav", false);
				collected = true;
			}
		}
	}
}

void Strawberry::render()
{
	//si la fresa se ha recogido no se tiene que hacer render
	if(!collected)sprite->render();
}

void Strawberry::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Strawberry::setPlayer(Player *p)
{
	player = p;
}
void Strawberry::setPosition(const glm::vec2 &pos)
{
	posStrawBerry = pos;
	startY = posStrawBerry.y; //startY es la posicion desde la que se levita
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posStrawBerry.x), float(tileMapDispl.y + posStrawBerry.y)));
}