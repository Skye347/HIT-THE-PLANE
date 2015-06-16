#include <SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<windows.h>
#ifndef AREAANDPLANE_H
#define AREAANDPLANE_H
#define LEFT 1
#define RIGHT 2
#define UP 4
#define DOWN 8

class Bullet;
class perBullet;
class Area;
class Plane;

typedef Plane* pPlane;

class perBullet:public sf::Transformable{
public:
	perBullet();
	void update(sf::RenderWindow*);

	sf::Sprite dot;
	sf::Texture texture;
	int pos_x;
	int pos_y;
	int Damage;
	int moveDirection;
};

class Bullet{
public:
	Bullet();
	void update(sf::RenderWindow*,Area*);
	void throwBullet(float, float, pPlane,int);
	std::vector<perBullet> bulletQueue;
	//perBullet peBiu;change for a queue 2015.5.23
};

class Plane:public sf::Transformable{
public:
	int object_id;
	int type;
	int moveXStatus;
	int moveYStatus;
	sf::Sprite plane;
	float position_x;
	float position_y;
	float bulletTimeInterval;
	int moveDistance;
	Area* field;
	bool usable;
	sf::Clock runningTime;
	sf::Clock bulletTmpTime;
	int Life;
	int score;
	sf::Texture texture;

	Plane();
	void addToArea(Area*);
	void sPosition(float, float);
	void move(int);
	void fire();
	void fireStop();
	int fireStatus;
	void onDamage(int);
	void onDestroy();
	void setPlane(int);
	void scoreAdd(int);
	void moveXStart(int);
	void moveYStart(int);
	void moveXStop();
	void moveYStop();

};

class Area{
public:
	int Level;
	sf::Vector2<float> left_up, left_down, right_up, right_down;
	int maxPlane, usageCount;
	pPlane pPlaneQueue[10];
	int gameStatus;
	//level set
	int timeIntervalMin, timeIntervalMax;
	int botSpeed;
	//
	//bullet manager!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
	Bullet bulletArea;
	//
	Area::Area(){
		timeIntervalMax = 4000;
		timeIntervalMin = 600;
		botSpeed = 1;
		Level = 1;
		left_down.x = 0;
		left_down.y = 900;
		left_up.x = 0;
		left_up.y = 0;
		right_up.x = 500;
		right_up.y = 0;
		right_down.x = 500;
		right_down.y = 900;
		maxPlane = 10;
		usageCount = 0;
		int temp = 0;
		gameStatus = 1;
		for (; temp < 10; temp++){
			pPlaneQueue[temp] = NULL;
		}
	}

	friend bool CollisionPlaneDamage(sf::Sprite*,Area*,int,int);
	void bulletUpdate(sf::RenderWindow*, Area*);
	void PlaneUpdate(sf::RenderWindow*);
	void PlaneAdd(pPlane);
	friend void AppendBotPlane(Area*);
};

bool isOutofField(float, float, Area*);

DWORD WINAPI thread2(PVOID);

DWORD WINAPI levelThread_f(PVOID);

void LevelSet(Area*);

#endif /*AAP_H*/