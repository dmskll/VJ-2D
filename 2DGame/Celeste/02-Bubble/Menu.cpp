#include "Menu.h"
#include <cmath>
#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#define SCREEN_X 16
#define SCREEN_Y 16


#define CAM_OFFSET 15.f  //es para que esté la camara bien ajustada
#define SHAKE_TIME 14.f


enum MenuStates
{
	MAIN, CONTROLS, CREDITS, PLAY
};

Menu::Menu()
{

}


Menu::~Menu()
{

}



void Menu::init()
{
	currentTime = 0;
	state = MAIN;
	engine = SoundControl::instance().getSoundEngine();

	initSnow();
	initShaders();
	projection = glm::ortho(CAM_OFFSET, float(SCREEN_WIDTH - CAM_OFFSET), float(SCREEN_HEIGHT - CAM_OFFSET), CAM_OFFSET);

	spritesheet.loadFromFile("images/menu-sprites.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(512, 512), glm::vec2(0.5, 0.5), &spritesheet, &texProgram);
	sprite->setNumberAnimations(3);

		sprite->setAnimationSpeed(MAIN, 8);
		sprite->addKeyframe(MAIN, glm::vec2(0.f, 0.f));

		sprite->setAnimationSpeed(CONTROLS, 8);
		sprite->addKeyframe(CONTROLS, glm::vec2(0.5f, 0.f));

		sprite->setAnimationSpeed(CREDITS, 8);
		sprite->addKeyframe(CREDITS, glm::vec2(0.f, 0.5f));

	sprite->changeAnimation(MAIN);
	sprite->setPosition(glm::vec2(10.f, 0.f));



}

void Menu::initSnow()
{
	snow = vector<SnowParticle *>(20);

	for (int i = 0; i < snow.size(); i++) {
		snow[i] = new SnowParticle;
		snow[i]->altura_inicial = i * 27 + (rand() % 40) - 20;
		snow[i]->lastPos_x = rand() % 530;
		snow[i]->divisor_velocidad_y = (rand() % 40) + 100;
		snow[i]->velocidad_x = (rand() % 8) + 1;
		snow[i]->random_offset = rand() % 100;
		snow[i]->altura_seno = (rand() % 30) + 10;

		float p_size = (rand() % 4) + 3;

		snow[i]->Particula = new Rectangulo();
		snow[i]->Particula->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(p_size, p_size), glm::vec2(snow[i]->lastPos_x, snow[i]->altura_inicial));
	}
}

void Menu::updateSnow(int deltaTime)
{
	currentTime += deltaTime;
	for (int i = 0; i < snow.size(); i++) {
		snow[i]->lastPos_x += snow[i]->velocidad_x;
		if (snow[i]->lastPos_x > 530) snow[i]->lastPos_x -= 550;
		float altura = glm::sin((currentTime / snow[i]->divisor_velocidad_y) + snow[i]->random_offset) * snow[i]->altura_seno + snow[i]->altura_inicial;
		snow[i]->Particula->setPosition(glm::vec2(snow[i]->lastPos_x, altura));
	}
}

void Menu::update(int deltaTime)
{
	//teclas para moverse por el menu
	if (shake) updateShake(deltaTime);
	sprite->update(deltaTime);

	updateSnow(deltaTime);

	if (state == MAIN)
	{
		if ((Game::instance().getKey(99) || Game::instance().getKey(67)))
		{
			engine->play2D("sounds/no-dash.wav", false);
			state = CONTROLS;
			sprite->changeAnimation(CONTROLS);
			setShake();
		}
		else if ((Game::instance().getKey(120) || Game::instance().getKey(88)))
		{
			engine->play2D("sounds/no-dash.wav", false);
			state = CREDITS;
			sprite->changeAnimation(CREDITS);
			setShake();
		}
		else if ((Game::instance().getKey(118) || Game::instance().getKey(86)))
		{
			state = PLAY;
			engine->stopAllSounds();
			engine->setSoundVolume(10);
			engine->play2D("sounds/end-music.wav", true);
			setShake();
		}
	}
	else if (state == PLAY)
	{
		if (shake == false)
		{
			Game::instance().setPlay();
		}
	}
	else //CONTROLS OR CREDITS 
	{
		if ((Game::instance().getKey(98) || Game::instance().getKey(66)))
		{
			engine->play2D("sounds/no-dash.wav", false);
			setShake();
			state = MAIN;
			sprite->changeAnimation(MAIN);
		}
	}




}

void Menu::setShake()
{
	shake = true;
	if (state == PLAY)
		shake_duration = 160.f;
	else
		shake_duration = SHAKE_TIME;
	shakeAngle = 0;
}

void Menu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	//snow

	sprite->render();

	for (int i = 0; i < snow.size(); i++) {
		snow[i]->Particula->render();
	}
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Menu::updateShake(int deltaTime)
{
	float shakeY, shakeX;
	if (state == PLAY)
	{
		shakeAngle += 20;			//cuanto mas alto mas brusco
		shakeY = sin(3.14159f * shakeAngle / 180.f) * 4;
		shakeX = shakeY * 2.f; //si no es igual queda mejor (?)
	}
	else
	{
		shakeAngle += 30;			//cuanto mas alto mas brusco
		shakeY = sin(3.14159f * shakeAngle / 180.f) * 4;
		shakeX = shakeY * 2.f; //si no es igual queda mejor (?)
	}



	projection = projection = glm::ortho(CAM_OFFSET + shakeX, float(SCREEN_WIDTH - CAM_OFFSET) + shakeX, float(SCREEN_HEIGHT - CAM_OFFSET) - shakeY, CAM_OFFSET - shakeY);

	if (shake_duration > 0)
	{
		shake_duration -= 0.1f * deltaTime;
	}
	else
	{
		projection = glm::ortho(CAM_OFFSET, float(SCREEN_WIDTH - CAM_OFFSET) + 0, float(SCREEN_HEIGHT - CAM_OFFSET), CAM_OFFSET);
		shake = false;
	}
}


