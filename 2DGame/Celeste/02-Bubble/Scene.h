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


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int level, float time);
	void update(int deltaTime);
	void render();
	void setShake();
	float getTime();
	bool check_win();
	bool check_lose();
	bool check_strawberry();

	float spawnX;
	float spawnY;
	

	float spawnX;
	float spawnY;
	

private:
	void initShaders();
	void initPlayer();
	void initObjects(int level);
	void renderObjects();
	void updateObjects(int deltaTime);
	void updateShake(int deltaTime);

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;

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

	struct SnowParticle {
		Rectangulo* Particula;
		float altura_inicial, altura_seno, lastPos_x, random_offset;
		int velocidad_x, divisor_velocidad_y;
	};
	
	struct HeightDisplay {
		Rectangulo* Background;
		vector<Number *> height;
		Character* letraM;
	};

	HeightDisplay heightOverlay;
	vector<Number *> Plus1000Obj;
	vector<Number *> Timer;
	vector<Rectangulo *> Timer_background;
	vector<SnowParticle *> snow;

	

	//faltan para los pinchos
};


#endif // _SCENE_INCLUDE

