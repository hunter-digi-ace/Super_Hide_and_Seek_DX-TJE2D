#ifndef SGAMEDATA_H
#define SGANMEDATA_H

#include <fstream>
#include <sstream>
#include "framework.h"


enum { FACE_DOWN, FACE_RIGHT, FACE_LEFT, FACE_UP };
enum eCellType { EMPTY = 0, NPC = 1, WALL = 2, DOOR = 3, OBJECT = 4 };
enum eobjectType { NOTHING = 0, BOTA = 1 };
enum directions { DOWN, RIGHT, LEFT, UP };


class sArea {
public:
	int size_components[2] = { 10,7 };
	int size = 70;
	eCellType tipo[70];
	eobjectType objeto[70];
	int doorWarp;
	int doorPos[2];
	int mapa[70];
	int npcs[70];
	bool hasChild = false;
};

class sWorld {
public:
	int size_components[2] = { 5,8 };
	int size = 40;
	sArea areas[40]; //solo mundo, faltarian interiores
};

class splayer {
public:
	Vector2 pos;
	Vector2 future_pos;
	int facing = FACE_DOWN;
	float player_speed = 55; //*1.35 si botas
	bool speed = true;
	float speedtime = 0.0;
};




class sGameData {
public:
	sWorld world;
	sWorld Resetworld;
	splayer player;
	int actualArea;
	int talkinTo;
	int counter;
	int numChilds;
	bool inTutorial;
	float time;
	bool hasobject;
	bool doorpased;
};


#endif
