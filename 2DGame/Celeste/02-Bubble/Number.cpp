#include "Number.h"


//Se crea un numero en la posición dada, en el parametro number se debe introducir el numero entre 0 y 9 deseado
//por defecto tiene color blanco, si se quiere rojo toca sumarle 10 al numero, y si se desea negro sumar 20.
//ejemplo: 14 = 4 en rojo, 9 = 9 en blanco, 23 = 3 en negro

void Number::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int number)
{	
	spritesheet.loadFromFile("images/Numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(14, 23), glm::vec2(0.1, 0.333333), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(30);

	for (int i = 0; i <= 30; i++) {
		sprite->setAnimationSpeed(i,8);
		float xpos = i%10;
		xpos = xpos / 10;

		float ypos = i / 10;
		ypos = ypos / 3;
		sprite->addKeyframe(i, glm::vec2(xpos, ypos) );
	}

	if (number >= 0 && number < 30) sprite->changeAnimation(number);
	else sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posNumber.x), float(tileMapDispl.y + posNumber.y)));
	movAngle = 0;
}


void Number::render()
{
	sprite->render();
}

void Number::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Number::setPosition(const glm::vec2 &pos)
{
	posNumber = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posNumber.x), float(tileMapDispl.y + posNumber.y)));
	lastPosition = pos;
}
void Number::setNumber(int number) {
	if(number >= 0 && number < 30) sprite->changeAnimation(number);
}

void Number::increaseHeight() {
	//se usa exclusivamente para la animación de recoger una fresa

	lastPosition.y--;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + lastPosition.x), float(tileMapDispl.y + lastPosition.y)));
}