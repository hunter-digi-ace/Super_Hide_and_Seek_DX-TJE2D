#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <fstream>
#include <sstream>
#include "sGameData.h"
#include "utils.h"




using namespace std;
class functions{
public:
    functions();

    static int* readCSV(std::string filesrc, int size);
    static void loadMap(string location, sGameData& myGame);
    static void initGame(sGameData& myGame);
    static void saveGameInfo(sGameData& myGame);
    static bool loadGameInfo(sGameData& myGame);
    static string timeformat(float time);
    static void reset(sGameData& myGame);
    static void changeMap(sGameData& myGame);
    static string* getMesagge(int npc, sGameData& myGame);
    static bool isKid(int npc);
    static void comunicatemaps(sGameData& myGame, int map1, int map2);
    static bool inHouse(sGameData& myGame); 
    static void updateTime(sGameData& myGame, float elapsedTime);
    static void setdinner(sGameData& myGame);
    static void hideChilds(sGameData& myGame, bool intutorial);
    static void updateSpeed(sGameData& myGame, float elapsedTime);
    static void addSpeed(sGameData& myGame);

    static int posActual(sGameData& myGame);
    //static int posFutura(sGameData& myGame);
    static int posFuturaExtended(int x, int y, sGameData& myGame);
    static int canMove(sGameData& myGame);
    static void move(int dir, double seconds_elapsed, sGameData& myGame);
    static void setPlayerPosX(int posX, int area, sGameData& myGame);
    static void setPlayerPosY(int posY, int area, sGameData& myGame);
    static void setPlayerPosXY(int posX, int posY, int area, sGameData& myGame);
};

//function out of any class

#endif
