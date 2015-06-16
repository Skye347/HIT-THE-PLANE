#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include"AreaAndPlane.h"
#include<ctime>
#include<vector>
#pragma once

Plane::Plane(){//plane type update 2015.5.23
	sf::Sprite plane_triangle;
	plane = plane_triangle;
	plane.setPosition(position_x, position_y);
	field = NULL;
	usable = false;
	fireStatus = 0;
	score = 0;
}

void Plane::sPosition(float x, float y){
	if (usable == false){
		std::cout << "object Plane is not usable" << std::endl;
		return;
	}
	plane.setPosition((float)x, (float)y);
}

void Plane::move(int direction){
	int offset_x=0;
	int offset_y=0;
	//direction control
	if (direction == UP){
		offset_y -= moveDistance;
	}
	else if (direction == DOWN){
		offset_y += moveDistance;
	}
	else if (direction==RIGHT){
		offset_x += moveDistance;
	}
	else if (direction == LEFT){
		offset_x -= moveDistance;
	}
	if (usable == false){
		std::cout << "object Plane is not usable" << std::endl;
		return;
	}
	position_x += offset_x;
	position_y += offset_y;
	if (type == 1){
		if (!(position_x <= field->right_down.x - 160 && position_x >= field->left_down.x)){
			position_x -= offset_x;
		}
		if (!(position_y <= field->right_down.y&&position_y >= field->left_up.y)){
			position_y -= offset_y;
		}
	}
	//std::cout << "moving" << std::endl;
	//std::cout << "current position:" << position_x << "," << position_y << std::endl;
	plane.setPosition(position_x, position_y);
}

void Plane::addToArea(Area* Field){
	if (Field == NULL){
		std::cout << "Field is NULL , add failed" << std::endl;
		return;
	}
	field = Field;
	//lnk2019 field->Area::PlaneAdd(this);
}

perBullet::perBullet(){
	sf::Sprite Dot;
	sf::Texture bullettexture;
	if (!bullettexture.loadFromFile("1.png")) {
		std::cout << "load texture plane_trangle failed" << std::endl;
	}
	texture = bullettexture;
	Dot.setTexture(texture);
	pos_x = 20;
	pos_y = 700;
	Dot.setPosition(pos_x, pos_y);
	dot = Dot;
	Damage = 1;
}

void Plane::fire(){
	//std::cout << "Plane fire" << std::endl;
	fireStatus = 1;
}

void Plane::fireStop(){
	//std::cout << "Plane fire stopped" << std::endl;
	fireStatus = 0;
}

void Bullet::update(sf::RenderWindow* pWindow,Area* pField){
	//for every bullet
	std::vector<perBullet>::iterator searchElem;
	for (searchElem = bulletQueue.begin(); searchElem != bulletQueue.end();){
		searchElem->pos_y += searchElem->moveDirection;
		//
		bool outCheck = isOutofField(searchElem->pos_x, searchElem->pos_y, pField);
		if (outCheck){
			std::cout << "bullet destroy" << std::endl;
			searchElem = bulletQueue.erase(searchElem);
		}
		else if (CollisionPlaneDamage(&(searchElem->dot),pField,searchElem->Damage,searchElem->moveDirection)) {//collision and plane destroy
			std::cout << "bullet destroy" << std::endl;//bullet destroy
			searchElem = bulletQueue.erase(searchElem);
		}
		else{
			searchElem->dot.setPosition(searchElem->pos_x, searchElem->pos_y);//bullet move steps 2015.5.23
			searchElem->update(pWindow);
			bool endCheck = (searchElem == bulletQueue.end());
			if (endCheck){
				break;
			}
			else{
				searchElem++;
			}
		}
		//
		if (bulletQueue.empty()){
			break;
		}
	}
	//position status update 2015.5.23
}

void perBullet::update(sf::RenderWindow* pWindow){
	//if collision or other condition 2015.5.23
	//else
	pWindow->draw(dot);
}

void Bullet::throwBullet(float start_x, float start_y,pPlane target_plane,int combination=0){//use time interval
	//create a perBullet 2015.5.23
	//std::cout << "tmpSecond:" << target_plane->bulletTmpTime.getElapsedTime().asSeconds() << std::endl;
	float timeInterval = 1;
	if (target_plane->type==1) {
		timeInterval = target_plane->bulletTimeInterval;
	}
	else if(target_plane->type==2){
		timeInterval = target_plane->bulletTimeInterval;
	}
	if (target_plane->bulletTmpTime.getElapsedTime().asSeconds() > timeInterval){
		perBullet insertTmp;
		if (target_plane->type == 1) {
			insertTmp.moveDirection = -5;
		}
		else if (target_plane->type == 2) {
			insertTmp.moveDirection = 2;
		}
		insertTmp.pos_x = start_x;
		insertTmp.pos_y = start_y;
		bulletQueue.push_back(insertTmp);
  		//std::cout << "throw a bullet" << std::endl;
		if (combination == 0) {
			target_plane->bulletTmpTime.restart();
		}
	}
	else{
		//std::cout << "not now" << std::endl;
	}
}

Bullet::Bullet(){
	//init for empty bullet 2015.5.23
}

bool isOutofField(float target_x, float target_y, Area* pField){
	if (target_x<=pField->right_down.x&&target_x>=pField->left_down.x){
		if (target_y >= pField->right_up.y&&target_y <= pField->right_down.y)
			return false;
	}
	return true;
}

void Area::bulletUpdate(sf::RenderWindow* pWindow, Area* pField){
	int queueRotTemp = 0;
	for (; queueRotTemp < 10; queueRotTemp++){
		if (pPlaneQueue[queueRotTemp] != NULL){
			if (pPlaneQueue[queueRotTemp]->fireStatus == 1){
				//std::cout << pPlaneQueue[queueRotTemp]->bulletTmpTime.getElapsedTime().asSeconds() << std::endl;
				float bullet_x = pPlaneQueue[queueRotTemp]->position_x;
				float bullet_y = pPlaneQueue[queueRotTemp]->position_y;
				if (queueRotTemp == 0) {
					bulletArea.throwBullet(bullet_x + 90, bullet_y, pPlaneQueue[queueRotTemp], 1);
				}
				bulletArea.throwBullet(bullet_x, bullet_y, pPlaneQueue[queueRotTemp]);//use time interval 2015.5.23
				//std::cout << "throw bullet" << std::endl;
			}
			else{
				pPlaneQueue[queueRotTemp]->bulletTmpTime.restart();
			}
			//pPlaneQueue[queueRotTemp]->bulletTmpTime.restart(); 2015.5.23 for test function
		}
	}
	bulletArea.update(pWindow, pField);
}

void Area::PlaneAdd(pPlane pObject){//lnk2019
	int rouTmp = 0;
	if (usageCount < 10){
		for (rouTmp = 0; rouTmp < 10; rouTmp++){
			if (pPlaneQueue[rouTmp] == NULL){
				pPlaneQueue[rouTmp] = pObject;
				pObject->object_id = rouTmp;
				break;
			}
		}
		usageCount++;
	}
}

void Plane::setPlane(int type){
	if (type == 1){//player plane
		plane.setPosition(170, 700);
		this->type = 1;
		position_x = 170;
		position_y = 700;
		moveDistance = 1;
		Life = 30;
		bulletTimeInterval = 0.2;
		sf::Texture plane_triangle_texture;
		if (!plane_triangle_texture.loadFromFile("feiji.png")) {
			std::cout << "load texture plane_trangle failed" << std::endl;
		}
		texture = plane_triangle_texture;
		plane.setTexture(texture);
		if (field == NULL){
			usable = false;
		}
		else{
			usable = true;
		}
	}
	else if(type==2){//bot plane
		position_x = 170; //random
		position_y = 0;
		this->type = 2;
		bulletTimeInterval = 2;
		moveDistance = field->botSpeed;
		plane.setPosition(position_x, position_y);
		sf::Texture plane_triangle_texture;
		if (!plane_triangle_texture.loadFromFile("dj.png")) {
			std::cout << "load texture plane_trangle failed" << std::endl;
		}
		texture = plane_triangle_texture;
		plane.setTexture(texture);
		if (field == NULL){
			usable = false;
		}
		else{
			usable = true;
		}
		Life = 3;
	}
}

bool isCollisionWithPlane(sf::FloatRect* pBound1, sf::FloatRect* pBound) {
	if (pBound1->intersects(*pBound)) {
		return true;
	}
	return false;
}

void Area::PlaneUpdate(sf::RenderWindow* pWindow){
	int rouTmp;
	if (pPlaneQueue[0] != NULL) {
		if (pPlaneQueue[0]->moveXStatus == -1) {
			pPlaneQueue[0]->move(LEFT);
		}
		else if (pPlaneQueue[0]->moveXStatus == 1) {
			pPlaneQueue[0]->move(RIGHT);
		}
		if (pPlaneQueue[0]->moveYStatus == 1) {
			pPlaneQueue[0]->move(UP);
		}
		else if (pPlaneQueue[0]->moveYStatus == -1) {
			pPlaneQueue[0]->move(DOWN);
		}
	for (rouTmp = 0; rouTmp < 10; rouTmp++){
		if (pPlaneQueue[rouTmp] == NULL){

		}
		else{
			if (pPlaneQueue[rouTmp]->usable == 1){
				if (pPlaneQueue[rouTmp]->type == 2){
					bool rangeCheck = isOutofField(pPlaneQueue[rouTmp]->position_x, pPlaneQueue[rouTmp]->position_y, this);
					bool collisionCheck = isCollisionWithPlane(&(pPlaneQueue[rouTmp]->plane.getGlobalBounds()), &(pPlaneQueue[0]->plane.getGlobalBounds()));
					if (collisionCheck) {
						pPlaneQueue[rouTmp]->onDestroy();
						pPlaneQueue[0]->onDamage(10);
						std::cout << "Crash!" << std::endl;
						continue;
					}
					else if (rangeCheck){
						pPlaneQueue[rouTmp]->onDestroy();
						continue;
					}
					else{
						pPlaneQueue[rouTmp]->move(DOWN);
						pPlaneQueue[rouTmp]->fire();
					}
				}
					__try {
						pWindow->draw(pPlaneQueue[rouTmp]->plane);//nothing
					}
					__except (EXCEPTION_EXECUTE_HANDLER) {
						std::cout << "draw error" << std::endl;
					}
				}
			}
		}
	}
}

void Plane::onDestroy(){
	std::cout << "enemy Destroy!!" << std::endl;
	if (this->type == 1) {
		this->field->gameStatus = 0;
	}
	if (this->type == 2) {
		field->pPlaneQueue[this->object_id] = NULL;
		field->usageCount--;
	}
	//destroy am

	//play audio
	sf::SoundBuffer background_audio_buffer;
	//if (!background_audio_buffer.loadFromFile("test_b.flac")){
	//	std::cout << "load test_b.flac failed" << std::endl;
	//}
	//sf::Sound background_audio;
	//background_audio.setBuffer(background_audio_buffer);
	//background_audio.play();
}

bool CollisionPlaneDamage(sf::Sprite* pBullet, Area* Field,int damage,int moveDirection) {
	sf::FloatRect bulletBounds = pBullet->getGlobalBounds();
	int queueRotTemp = 1;
	if (pBullet->getGlobalBounds().intersects(Field->pPlaneQueue[0]->plane.getGlobalBounds())) {
		if (moveDirection > 0) {
			std::cout << "You get hit" << std::endl;
			Field->pPlaneQueue[0]->onDamage(damage);
			return true;
		}
	}
	for (; queueRotTemp < 10; queueRotTemp++) {
		if (Field->pPlaneQueue[queueRotTemp] != NULL) {
			sf::FloatRect planeBounds = Field->pPlaneQueue[queueRotTemp]->plane.getGlobalBounds();
			if (bulletBounds.intersects(planeBounds)) {
				if(Field->pPlaneQueue[queueRotTemp]->type == 2 && moveDirection < 0) {
					std::cout << "Collision! Plane Destroy" << std::endl;
					Field->pPlaneQueue[0]->scoreAdd(2);
					Field->pPlaneQueue[queueRotTemp]->onDamage(damage);
					return true;
				}
			}
		}
	}
	return false;
}

void Plane::onDamage(int damage=1) {
	if (Life<=0) {
		onDestroy();
	}
	else {
		Life -= damage;
	}
}

void AppendBotPlane(Area* pField) {
	while (1) {
		srand(pField->usageCount+rand());
		int timer = 0;
		while (1) {
			timer = rand();
			if (timer < pField->timeIntervalMax && timer>pField->timeIntervalMin) {
				break;
			}
		}
		Sleep(timer);
		if (pField->usageCount < 10) {
		//random time
			Plane* bot = new Plane;
			bot->addToArea(pField);
			pField->PlaneAdd(bot);
			int loc_x = 0;
			while (1) {
				loc_x = rand();
				if (loc_x > 500) {
					loc_x = loc_x / 100;
				}
				if (loc_x < 500 && loc_x>10) {
					break;
				}
			}
		//random location
			bot->setPlane(2);
			bot->position_x = loc_x;
		}
	}
}

DWORD WINAPI thread2(PVOID pvParam) {
	AppendBotPlane((Area*)pvParam);
	return 1;
}

void Plane::scoreAdd(int scoreAdded) {
   	score += scoreAdded;
}

void Plane::moveXStart(int direction) {
	if (direction == RIGHT) {
		moveXStatus = 1;
	}
	else if (direction == LEFT) {
		moveXStatus = -1;
	}
}

void Plane::moveXStop() {
	moveXStatus = 0;
}

void Plane::moveYStart(int direction) {
	if (direction == UP) {
		moveYStatus = 1;
	}
	else if (direction == DOWN) {
		moveYStatus = -1;
	}
}

void Plane::moveYStop() {
	moveYStatus = 0;
}

void LevelSet(Area* field) {
	while (field->gameStatus == 1) {
		if (field->pPlaneQueue[0] != NULL) {
			if (field->pPlaneQueue[0]->score < 10) {
				field->Level = 1;
			}
			else if(field->pPlaneQueue[0]->score > 10&& field->pPlaneQueue[0]->score < 900){
				field->Level = 2;
				field->timeIntervalMax = 1000;
				field->timeIntervalMin = 300;
				field->botSpeed = 2;
			}
		}
	}
}

DWORD WINAPI levelThread_f(PVOID pvParam) {
	LevelSet((Area*)pvParam);
	return 1;
}