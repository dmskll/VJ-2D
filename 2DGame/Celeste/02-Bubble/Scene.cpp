#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 16
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 5
#define INIT_PLAYER_Y_TILES 5


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
	berry, spring = false;
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
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram, objs);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);
	initObjects();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	updateObjects(deltaTime);
	//berry->update(deltaTime);
	//spring->update(deltaTime);
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
	//berry->render();
	//spring->render();
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



