#include "Character.h"


//acepta los caracteres A B C E H L M N O R S T X Y, toca pasarlos por parametro en mayuscula
//los colores disponibles són WHITE, BLACK, GRAY y RED

void Character::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/text.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(14, 23), glm::vec2(0.125, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(55);


	sprite->setAnimationSpeed(0, 8);//A blanco
	sprite->addKeyframe(0, glm::vec2(0, 0));

	sprite->setAnimationSpeed(1, 8);//B blanco
	sprite->addKeyframe(1, glm::vec2(0.125, 0));

	sprite->setAnimationSpeed(2, 8);//C blanco
	sprite->addKeyframe(2, glm::vec2(0.25, 0));

	sprite->setAnimationSpeed(3, 8);//E blanco
	sprite->addKeyframe(3, glm::vec2(0.375, 0));

	sprite->setAnimationSpeed(4, 8);//H blanco
	sprite->addKeyframe(4, glm::vec2(0.5, 0));

	sprite->setAnimationSpeed(5, 8);//L blanco
	sprite->addKeyframe(5, glm::vec2(0.625, 0));

	sprite->setAnimationSpeed(6, 8);//M blanco
	sprite->addKeyframe(6, glm::vec2(0.75, 0));

	sprite->setAnimationSpeed(7, 8);//N blanco
	sprite->addKeyframe(7, glm::vec2(0.875, 0));

	sprite->setAnimationSpeed(8, 8);//O blanco
	sprite->addKeyframe(8, glm::vec2(0, 0.125));

	sprite->setAnimationSpeed(9, 8);//R blanco
	sprite->addKeyframe(9, glm::vec2(0.125, 0.125));

	sprite->setAnimationSpeed(10, 8);//S blanco
	sprite->addKeyframe(10, glm::vec2(0.25, 0.125));

	sprite->setAnimationSpeed(11, 8);//T blanco
	sprite->addKeyframe(11, glm::vec2(0.375, 0.125));

	sprite->setAnimationSpeed(12, 8);//X blanco
	sprite->addKeyframe(12, glm::vec2(0.5, 0.125));

	sprite->setAnimationSpeed(13, 8);//Y blanco
	sprite->addKeyframe(13, glm::vec2(0.625, 0.125));


	sprite->setAnimationSpeed(14, 8);//A negro
	sprite->addKeyframe(14, glm::vec2(0, 0.25));

	sprite->setAnimationSpeed(15, 8);//B negro
	sprite->addKeyframe(15, glm::vec2(0.125, 0.25));

	sprite->setAnimationSpeed(16, 8);//C negro
	sprite->addKeyframe(16, glm::vec2(0.25, 0.25));

	sprite->setAnimationSpeed(17, 8);//E negro
	sprite->addKeyframe(17, glm::vec2(0.375, 0.25));

	sprite->setAnimationSpeed(18, 8);//H negro
	sprite->addKeyframe(18, glm::vec2(0.5, 0.25));

	sprite->setAnimationSpeed(19, 8);//L negro
	sprite->addKeyframe(19, glm::vec2(0.625, 0.25));

	sprite->setAnimationSpeed(20, 8);//M negro
	sprite->addKeyframe(20, glm::vec2(0.75, 0.25));

	sprite->setAnimationSpeed(21, 8);//N negro
	sprite->addKeyframe(21, glm::vec2(0.875, 0.25));

	sprite->setAnimationSpeed(22, 8);//O negro
	sprite->addKeyframe(22, glm::vec2(0, 0.375));

	sprite->setAnimationSpeed(23, 8);//R negro
	sprite->addKeyframe(23, glm::vec2(0.125, 0.375));

	sprite->setAnimationSpeed(24, 8);//S negro
	sprite->addKeyframe(24, glm::vec2(0.25, 0.375));

	sprite->setAnimationSpeed(25, 8);//T negro
	sprite->addKeyframe(25, glm::vec2(0.375, 0.375));

	sprite->setAnimationSpeed(26, 8);//X negro
	sprite->addKeyframe(26, glm::vec2(0.5, 0.375));

	sprite->setAnimationSpeed(27, 8);//Y negro
	sprite->addKeyframe(27, glm::vec2(0.625, 0.375));



	sprite->setAnimationSpeed(28, 8);//A gris
	sprite->addKeyframe(28, glm::vec2(0, 0.5));

	sprite->setAnimationSpeed(29, 8);//B gris
	sprite->addKeyframe(29, glm::vec2(0.125, 0.5));

	sprite->setAnimationSpeed(30, 8);//C gris
	sprite->addKeyframe(30, glm::vec2(0.25, 0.5));

	sprite->setAnimationSpeed(31, 8);//E gris
	sprite->addKeyframe(31, glm::vec2(0.375, 0.5));

	sprite->setAnimationSpeed(32, 8);//H gris
	sprite->addKeyframe(32, glm::vec2(0.5, 0.5));

	sprite->setAnimationSpeed(33, 8);//L gris
	sprite->addKeyframe(33, glm::vec2(0.625, 0.5));

	sprite->setAnimationSpeed(34, 8);//M gris
	sprite->addKeyframe(34, glm::vec2(0.75, 0.5));

	sprite->setAnimationSpeed(35, 8);//N gris
	sprite->addKeyframe(35, glm::vec2(0.875, 0.5));

	sprite->setAnimationSpeed(36, 8);//O gris
	sprite->addKeyframe(36, glm::vec2(0, 0.625));

	sprite->setAnimationSpeed(37, 8);//R gris
	sprite->addKeyframe(37, glm::vec2(0.125, 0.625));

	sprite->setAnimationSpeed(38, 8);//S gris
	sprite->addKeyframe(38, glm::vec2(0.25, 0.625));

	sprite->setAnimationSpeed(39, 8);//T gris
	sprite->addKeyframe(39, glm::vec2(0.375, 0.625));

	sprite->setAnimationSpeed(40, 8);//X gris
	sprite->addKeyframe(40, glm::vec2(0.5, 0.625));

	sprite->setAnimationSpeed(41, 8);//Y gris
	sprite->addKeyframe(41, glm::vec2(0.625, 0.625));




	sprite->setAnimationSpeed(42, 8);//A rojo
	sprite->addKeyframe(42, glm::vec2(0, 0.75));

	sprite->setAnimationSpeed(43, 8);//B rojo
	sprite->addKeyframe(43, glm::vec2(0.125, 0.75));

	sprite->setAnimationSpeed(44, 8);//C rojo
	sprite->addKeyframe(44, glm::vec2(0.25, 0.75));

	sprite->setAnimationSpeed(45, 8);//E rojo
	sprite->addKeyframe(45, glm::vec2(0.375, 0.75));

	sprite->setAnimationSpeed(46, 8);//H rojo
	sprite->addKeyframe(46, glm::vec2(0.5, 0.75));

	sprite->setAnimationSpeed(47, 8);//L rojo
	sprite->addKeyframe(47, glm::vec2(0.625, 0.75));

	sprite->setAnimationSpeed(48, 8);//M rojo
	sprite->addKeyframe(48, glm::vec2(0.75, 0.75));

	sprite->setAnimationSpeed(49, 8);//N rojo
	sprite->addKeyframe(49, glm::vec2(0.875, 0.75));

	sprite->setAnimationSpeed(50, 8);//O rojo
	sprite->addKeyframe(50, glm::vec2(0, 0.875));

	sprite->setAnimationSpeed(51, 8);//R rojo
	sprite->addKeyframe(51, glm::vec2(0.125, 0.875));

	sprite->setAnimationSpeed(52, 8);//S rojo
	sprite->addKeyframe(52, glm::vec2(0.25, 0.875));

	sprite->setAnimationSpeed(53, 8);//T rojo
	sprite->addKeyframe(53, glm::vec2(0.375, 0.875));

	sprite->setAnimationSpeed(54, 8);//X rojo
	sprite->addKeyframe(54, glm::vec2(0.5, 0.875));

	sprite->setAnimationSpeed(55, 8);//Y rojo
	sprite->addKeyframe(55, glm::vec2(0.625, 0.875));


	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posNumber.x), float(tileMapDispl.y + posNumber.y)));
	movAngle = 0;
}


void Character::render()
{
	sprite->render();
}

void Character::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Character::setPosition(const glm::vec2 &pos)
{
	posNumber = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posNumber.x), float(tileMapDispl.y + posNumber.y)));
	lastPosition = pos;
}
void Character::setCharacter(char ch, string colour) {
	// A B C E H L M N O R S T X Y
	int num;
	if (ch == 'A') num = 0;
	else if (ch == 'B') num = 1;
	else if (ch == 'C') num = 2;
	else if (ch == 'E') num = 3;
	else if (ch == 'H') num = 4;
	else if (ch == 'L') num = 5;
	else if (ch == 'M') num = 6;
	else if (ch == 'N') num = 7;
	else if (ch == 'O') num = 8;
	else if (ch == 'R') num = 9;
	else if (ch == 'S') num = 10;
	else if (ch == 'T') num = 11;
	else if (ch == 'X') num = 12;
	else num = 13;

	if (colour == "BLACK") num += 14;
	else if (colour == "GRAY") num += 28;
	else if (colour == "RED") num += 42;


	sprite->changeAnimation(num);
}

