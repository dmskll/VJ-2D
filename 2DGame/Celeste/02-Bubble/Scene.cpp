#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 5

#define CAM_OFFSET 15.f  //es para que esté la camara bien ajustada
#define SHAKE_TIME 14.f


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

void Scene::initObjects()
{
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	player->setLevel(this);

	ballon = false;
	berry = false;
	spring = false;

	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i].type == "BERRY")
		{
			berry = true;
			berryObj = new Strawberry();
			berryObj->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			berryObj->setPosition(glm::vec2(objs[i].x * map->getTileSize(), objs[i].y * map->getTileSize()));
			berryObj->setTileMap(map);
			berryObj->setPlayer(player);
		}
		else if (objs[i].type == "SPRING")
		{
			spring = true;
			springObj.push_back(new Spring());
			springObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			springObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			springObj.back()->setTileMap(map);
			springObj.back()->setPlayer(player);
		}
		else if (objs[i].type == "SPIKE")
		{

		}
		else if (objs[i].type == "BALLON")
		{
			ballon = true;
			ballonObj.push_back(new Ballon());
			ballonObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			ballonObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			ballonObj.back()->setTileMap(map);
			ballonObj.back()->setPlayer(player);
		}
	}
}

void Scene::renderObjects()
{
	if (berry)
	{
		berryObj->render();
	}
	if (spring)
	{
		for (int i = 0; i < springObj.size(); ++i)
		{
			springObj[i]->render();
		}
	}
	if (ballon)
	{
		for (int i = 0; i < ballonObj.size(); ++i)
		{
			ballonObj[i]->render();
		}
	}
}

void Scene::updateObjects(int deltaTime)
{
	if (berry)
	{
		berryObj->update(deltaTime);
	}
	if (spring)
	{
		for (int i = 0; i < springObj.size(); ++i)
		{
			springObj[i]->update(deltaTime);
		}
	}
	if (ballon)
	{
		for (int i = 0; i < ballonObj.size(); ++i)
		{
			ballonObj[i]->update(deltaTime);
		}
	}
}

void Scene::updateShake(int deltaTime)
{
	shakeAngle += 30;			//cuanto mas alto mas brusco
	float shakeY = sin(3.14159f * shakeAngle / 180.f) * 4;
	float shakeX = shakeY * 2.f; //si no es igual queda mejor (?)

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

void Scene::setShake()
{
	shake = true;
	shake_duration = SHAKE_TIME;
	shakeAngle = 0;
}

void Scene::init(int level)
{
	initShaders();
	string s = "levels/level";
	if (level < 10) s += "0";
	s += std::to_string(level) + ".txt";
	
	map = TileMap::createTileMap(s, glm::vec2(SCREEN_X, SCREEN_Y), texProgram, objs);

	initObjects();

	shakeAngle = 0;
	shake = false;
	projection = glm::ortho(CAM_OFFSET, float(SCREEN_WIDTH - CAM_OFFSET) + 0, float(SCREEN_HEIGHT - CAM_OFFSET), CAM_OFFSET);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	updateObjects(deltaTime);
	//berry->update(deltaTime);
	//spring->update(deltaTime);
	if (shake) updateShake(deltaTime);
}

bool Scene::check_win() {
	return player->check_win();
}
bool Scene::check_lose() {
	return player->check_lose();
}


void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();

	renderObjects();
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



