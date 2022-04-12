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

void Scene::initPlayer()
{
	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i].type == "SPAWN")
		{
			spawnX = objs[i].x * map->getTileSize();
			spawnY = objs[i].y * map->getTileSize();
		}
	}
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(spawnX, spawnY));
	player->setTileMap(map);
	player->setLevel(this);

}

void Scene::initObjects(int level)
{
	ballon = false;
	berry = false;
	spring = false;
	spike = false;
	plant = false;




	background.init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, glm::vec2(580,580), glm::vec2(0, 0));

	//cosas de las nuves

	clouds = vector<Cloud *>(20);
	for (int i = 0; i < clouds.size(); i++) {
		clouds[i] = new Cloud;
		clouds[i]->particula = new Rectangulo();

		int anchura = 20 + (rand()%40);
		int longitud = 100 + (rand()% 60);

		clouds[i]->particula->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "cloud", glm::vec2(longitud,anchura),glm::vec2(100,100));
		clouds[i]->altura = i*27 + (rand()%80) - 40;
		clouds[i]->velocidad = (rand() % 15) + 1;
		clouds[i]->lastPos_x = rand() % 800 - 150;
	}

	//cosas de la nieve

	snow = vector<SnowParticle *>(20);

	for (int i = 0; i < snow.size(); i++) {
		snow[i] = new SnowParticle;
		snow[i]->altura_inicial = i * 27 + (rand()%40) -20;
		snow[i]->lastPos_x = rand() % 530;
		snow[i]->divisor_velocidad_y = (rand() % 40) + 100;
		snow[i]->velocidad_x = (rand() % 8) + 1;
		snow[i]->random_offset = rand() % 100;
		snow[i]->altura_seno = (rand() % 30) + 10;

		float p_size = (rand() % 4) + 3;

		snow[i]->Particula = new Rectangulo();
		snow[i]->Particula->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(p_size,p_size), glm::vec2(snow[i]->lastPos_x, snow[i]->altura_inicial));
	}

	//cosas del temporizador
	Timer = vector<Number *>(6);
	auto sum = glm::vec2(0, 0);
	if (summit) sum = glm::vec2(200, 50);
	for (int i = 0; i < 6; i++) {
		Timer[i] = new Number();
		Timer[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
		auto pos = glm::vec2(i * 20 + 20, 20);
		if (i > 1) pos.x += 13;
		if (i > 3) pos.x += 13;
		pos += sum;
		Timer[i]->setPosition(pos);
	}
	Timer_background = vector<Rectangulo *>(5);
	for(int i = 0; i < 5; i++) Timer_background[i] = new Rectangulo();

	Timer_background[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 4), glm::vec2(77, 39) + sum);
	Timer_background[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 5), glm::vec2(77, 51) + sum);
	Timer_background[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 4), glm::vec2(130, 39) + sum);
	Timer_background[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 5), glm::vec2(130, 51) + sum);

	if(summit) Timer_background[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "black", glm::vec2(270, 135), glm::vec2(170,25));
	else Timer_background[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "black", glm::vec2(152, 35), glm::vec2(30, 30));
	//cosas del overlay de altura

	int height = level * 100;
	while (height != 0) {
		int n = height % 10;
		height /= 10;
		Number* num = new Number();
		num->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, n);
		num->setPosition(glm::vec2(250 - heightOverlay.height.size() * 18,245));
		heightOverlay.height.push_back(num);
	}

	heightOverlay.letraM = new Character;
	heightOverlay.letraM->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	heightOverlay.letraM->setPosition(glm::vec2(280 , 245));
	heightOverlay.letraM->setCharacter('M',"WHITE");

	heightOverlay.Background = new Rectangulo;
	heightOverlay.Background->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "black", glm::vec2(300,45), glm::vec2(120,250));

	if (summit) {
		heightOverlay.summit = vector<Character *>(6);
		for (int i = 0; i < 6; i++) heightOverlay.summit[i] = new Character();
		heightOverlay.summit[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[0]->setCharacter('S', "WHITE");
		heightOverlay.summit[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[1]->setCharacter('U', "WHITE");
		heightOverlay.summit[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[2]->setCharacter('M', "WHITE");
		heightOverlay.summit[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[3]->setCharacter('M', "WHITE");
		heightOverlay.summit[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[4]->setCharacter('I', "WHITE");
		heightOverlay.summit[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		heightOverlay.summit[5]->setCharacter('T', "WHITE");

		for (int i = 0; i < 6; i++) heightOverlay.summit[i]->setPosition(glm::vec2((19 * i) + 200, 245));

		deaths_indicator.letters = vector<Character*>(6);
		for (int i = 0; i < 6; i++) deaths_indicator.letters[i] = new Character();

		deaths_indicator.letters[0]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[0]->setCharacter('O',"WHITE");

		deaths_indicator.letters[1]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[1]->setCharacter('E', "WHITE");

		deaths_indicator.letters[2]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[2]->setCharacter('A', "WHITE");

		deaths_indicator.letters[3]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[3]->setCharacter('T', "WHITE");

		deaths_indicator.letters[4]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[4]->setCharacter('H', "WHITE");

		deaths_indicator.letters[5]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		deaths_indicator.letters[5]->setCharacter('S', "WHITE");


		for (int i = 0; i < 6; i++) deaths_indicator.letters[i]->setPosition(glm::vec2((19 * i) + 205, 105));

		deaths_indicator.point_1 = new Rectangulo();
		deaths_indicator.point_2 = new Rectangulo();


		deaths_indicator.point_1->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 4), glm::vec2(337, 124));
		deaths_indicator.point_2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, "white", glm::vec2(5, 5), glm::vec2(337, 136));


	}

	//indicador de +1000 puntos
	Plus1000Obj = vector<Number *>(4);
	picked_up_strawberry_progress = 10000;

	//objetos del nivel
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
		else if (objs[i].type == "SPIKE-L")
		{
			spike = true;
			spikeObj.push_back(new Spike());
			spikeObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3, glm::vec2(objs[i].x, objs[i].y), player);
		}
		else if (objs[i].type == "SPIKE-U")
		{
			spike = true;
			spikeObj.push_back(new Spike());
			spikeObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1, glm::vec2(objs[i].x, objs[i].y), player);
		}
		else if (objs[i].type == "SPIKE-R")
		{
			spike = true;
			spikeObj.push_back(new Spike());
			spikeObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 4, glm::vec2(objs[i].x, objs[i].y), player);
		}
		else if (objs[i].type == "SPIKE-D")
		{
			spike = true;
			spikeObj.push_back(new Spike());
			spikeObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2, glm::vec2(objs[i].x, objs[i].y), player);
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
		else if (objs[i].type == "GRASS1")
		{
			plant = true;
			plantsObj.push_back(new plants());
			plantsObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
			plantsObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			plantsObj.back()->setTileMap(map);
		}
		else if (objs[i].type == "GRASS2")
		{
			plant = true;
			plantsObj.push_back(new plants());
			plantsObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
			plantsObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			plantsObj.back()->setTileMap(map);
		}
		else if (objs[i].type == "FLOWER")
		{
			plant = true;
			plantsObj.push_back(new plants());
			plantsObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2);
			plantsObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			plantsObj.back()->setTileMap(map);
		}
		else if (objs[i].type == "BUSH")
		{
			plant = true;
			plantsObj.push_back(new plants());
			plantsObj.back()->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3);
			plantsObj.back()->setPosition(glm::vec2((objs[i].x) * map->getTileSize(), (objs[i].y) * map->getTileSize()));
			plantsObj.back()->setTileMap(map);
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

	if (spike)
	{
		for (int i = 0; i < spikeObj.size(); ++i)
		{
			spikeObj[i]->render();
		}
	}

	if (picked_up_strawberry_progress < 50) {
		for (int i = 0; i < 4; i++) {

			if (picked_up_strawberry_progress % 6 == 3) {
				int digit = 0;
				if (i == 0) digit += 1;
				Plus1000Obj[i]->setNumber(digit);
			}
			else if (picked_up_strawberry_progress % 6 == 0) {
				int digit = 10;
				if (i == 0) digit += 1;
				Plus1000Obj[i]->setNumber(digit);
			};
			Plus1000Obj[i]->increaseHeight();
			Plus1000Obj[i]->render();

		}
		picked_up_strawberry_progress++;
	}
	
	//timer y altura
	if (overlay_progress < 80) {
		overlay_progress++;

		heightOverlay.Background->render();
		if (!summit) {
			int time2 = currentTime / 1000;
			int hours = time2 / 3600;
			int minutes = (time2 % 3600) / 60;
			int seconds = time2 % 60;
			for (int i = 0; i < 5; i++) Timer_background[i]->render();

			Timer[0]->setNumber(hours / 10);
			Timer[1]->setNumber(hours % 10);
			Timer[2]->setNumber(minutes / 10);
			Timer[3]->setNumber(minutes % 10);
			Timer[4]->setNumber(seconds / 10);
			Timer[5]->setNumber(seconds % 10);


			for (int i = 0; i < 6; i++) {
				Timer[i]->render();
			}



			for (int i = 0; i < heightOverlay.height.size(); i++) {
				heightOverlay.height[i]->render();
			}
			heightOverlay.letraM->render();
		}
		else {
			for (int i = 0; i < 6; i++) heightOverlay.summit[i]->render();
		}
	}
	if (summit) {//display score
		int time2 = finalTime / 1000;
		int hours = time2 / 3600;
		int minutes = (time2 % 3600) / 60;
		int seconds = time2 % 60;
		for (int i = 0; i < 5; i++) Timer_background[i]->render();

		Timer[0]->setNumber(hours / 10);
		Timer[1]->setNumber(hours % 10);
		Timer[2]->setNumber(minutes / 10);
		Timer[3]->setNumber(minutes % 10);
		Timer[4]->setNumber(seconds / 10);
		Timer[5]->setNumber(seconds % 10);


		for (int i = 0; i < 6; i++) {
			Timer[i]->render();
		}

		for (int i = 0; i < 6; i++) deaths_indicator.letters[i]->render();

		deaths_indicator.point_1->render();
		deaths_indicator.point_2->render();
	}


	//nieve

	for (int i = 0; i < snow.size(); i++) {
		snow[i]->Particula->render();
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
	if (spike)
	{
		for (int i = 0; i < spikeObj.size(); ++i)
		{
			spikeObj[i]->update();
		}
	}
	if (plant)
	{
		for (int i = 0; i < plantsObj.size(); ++i)
		{
			plantsObj[i]->update(deltaTime);
		}
	}

	//particulas de nieve
	for (int i = 0; i < snow.size(); i++) {
		snow[i]->lastPos_x += snow[i]->velocidad_x;
		if (snow[i]->lastPos_x > 530) snow[i]->lastPos_x -= 550;
		float altura = glm::sin((currentTime / snow[i]->divisor_velocidad_y) + snow[i]->random_offset) * snow[i]->altura_seno + snow[i]->altura_inicial;
		snow[i]->Particula->setPosition(glm::vec2(snow[i]->lastPos_x, altura));
	}
	//clouds
	for (int i = 0; i < clouds.size(); i++) {
		clouds[i]->lastPos_x = clouds[i]->lastPos_x + clouds[i]->velocidad;
		if (clouds[i]->lastPos_x > 700) clouds[i]->lastPos_x -= 900;
		clouds[i]->particula->setPosition(glm::vec2(clouds[i]->lastPos_x, clouds[i]->altura));
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

void Scene::init(int level, float time)
{
	initShaders();
	string s = "levels/level";
	if (level < 10) s += "0";
	s += std::to_string(level) + ".txt";
	
	if (level == 11) summit = true;
	else summit = false;

	map = TileMap::createTileMap(s, glm::vec2(SCREEN_X, SCREEN_Y), texProgram, objs);
	initPlayer();
	initObjects(level);

	shakeAngle = 0;
	shake = false;

	overlay_progress = 0;

	projection = glm::ortho(CAM_OFFSET, float(SCREEN_WIDTH - CAM_OFFSET) + 0, float(SCREEN_HEIGHT - CAM_OFFSET), CAM_OFFSET);
	currentTime = time;
	finalTime = time;
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
	if(!summit)	return player->check_win();
	else return false;
}
bool Scene::check_lose() {
	return player->check_lose();
}
bool Scene::check_strawberry() {
	if (player->check_strawberry()) {


		for (int i = 0; i < 4; i++) {
			Plus1000Obj[i] = new Number();
			if (i == 0) Plus1000Obj[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
			else Plus1000Obj[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
			glm::vec2 pos = player->getPosition();
			pos.x = player->getPosition().x + (i * 20) - 20;
			pos.y = player->getPosition().y - 30;
			Plus1000Obj[i]->setPosition(pos);
			picked_up_strawberry_progress = 0;
		}

		return true;
	}
	return false;
}

void Scene::render()
{
	for (int i = 0; i < clouds.size(); i++) clouds[i]->particula->render();
	
	if(!summit) background.render();

	glm::mat4 modelview;
	
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	for (int i = 0; i < plantsObj.size(); ++i)
	{
		plantsObj[i]->render();
	}


	player->render();


	renderObjects();
}

float Scene::getTime() {
	return currentTime;
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



