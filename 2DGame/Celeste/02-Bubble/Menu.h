#pragma once

#include "Sprite.h"

#include <glm/glm.hpp>
//#include <vector>
#include "ShaderProgram.h"
//#include "TileMap.h"
#include "Rectangulo.h"
#include "SoundControl.h"




class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void updateShake(int deltaTime);
	void initSnow();
	void updateSnow(int deltaTime);
	void setShake();

private:

	Texture spritesheet;
	Sprite *sprite;
	ShaderProgram texProgram;

	int state;
	float currentTime;
	float shakeAngle;
	bool shake;
	float shake_duration;
	glm::mat4 projection;

	ISoundEngine* engine;

	struct SnowParticle {
		Rectangulo* Particula;
		float altura_inicial, altura_seno, lastPos_x, random_offset;
		int velocidad_x, divisor_velocidad_y;
	};
	vector<SnowParticle *> snow;

};

