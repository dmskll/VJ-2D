#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Strawberry.h"
#include "Spring.h"
#include "Ballon.h"
#include "Number.h"
#include "Rectangulo.h"
#include "Character.h"
#include "Spike.h"
#include "plants.h"
#include "Background.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level, float time, int deaths, int strawberries);
	void update(int deltaTime);
	void render();
	void setShake();
	float getTime();
	bool check_win();
	bool check_lose();
	bool check_strawberry();

	float spawnX;
	float spawnY;
	

private:
	void initShaders();
	void initPlayer();
	void initObjects(int level, int deaths, int strawberries);
	void renderObjects();
	void updateObjects(int deltaTime);
	void updateShake(int deltaTime);

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime, finalTime;

	float shakeAngle;
	bool shake;
	float shake_duration;
	int picked_up_strawberry_progress;
	float overlay_progress;
	glm::mat4 projection;
	vector<objects> objs;  //contiene info de los objetos de la escena (mulles, fresas, pinchos...)
	bool berry;			   //hay una fresa en el mapa
	Strawberry *berryObj;  //instancia de fresa
	bool spring;		   //hay un muelle en el mapa
	vector<Spring *> springObj; //tiene las instancias de los muelles
	bool ballon;
	vector<Ballon *> ballonObj;
	bool spike;
	vector<Spike *> spikeObj;
	bool plant;
	vector<plants *> plantsObj;
	bool summit;

	struct SnowParticle {
		Rectangulo* Particula;
		float altura_inicial, altura_seno, lastPos_x, random_offset;
		int velocidad_x, divisor_velocidad_y;
	};
	
	struct HeightDisplay {
		Rectangulo* Background;
		vector<Number *> height;
		Character* letraM;
		vector<Character *> summit;
	};

	HeightDisplay heightOverlay;
	vector<Number *> Plus1000Obj;
	vector<Number *> Timer;
	vector<Rectangulo *> Timer_background;
	vector<SnowParticle *> snow;

	struct Cloud {
		Rectangulo* particula;
		int velocidad, altura, lastPos_x;
	};

	vector<Cloud *> clouds;

	Background background;

	struct Counter {
		vector<Character *> letters;
		Rectangulo *point_1, *point_2;
		vector<Number *> nums;
	};

	Counter deaths_indicator;

	
	Strawberry decorative_berry;
	vector<Number *> strawberry_counter;
	Character iks;

};


#endif // _SCENE_INCLUDE

