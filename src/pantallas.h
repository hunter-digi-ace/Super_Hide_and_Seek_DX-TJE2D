#ifndef PANTALLAS_H
#define PANTALLAS_H

#include <fstream>
#include <map>
#include "image.h"
#include "input.h"
#include "functions.h"
#include "game.h"



class Stage {
public:
	static std::map<std::string, Stage*> s_stages;
	static Stage* current_stage;
	static void changeState(const char* name);
	void printText(Image& fb, sGameData& myGame);

	static void printUI(Image& fb, sGameData& myGame);
	static void Stage::printObject(Image& fb, sGameData& myGame, bool object);

	Stage(const char* name);
	virtual void render(Image& fb, float time, sGameData& myGame) {}
	virtual void update(double seconds_elapsed, sGameData& myGame) {}
};



class titleStage : public Stage { 
public:
	int order = 0;
	string imageActual = "intro1";
	titleStage() : Stage("title") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);
};


class IntroStage : public Stage {
public:
	int menupos = 0;
	IntroStage() : Stage("intro") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);

};



class PlayStage : public Stage { 
public:
	PlayStage() : Stage("play") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);
};


class MenuStage : public Stage { 
public:
	int menupos = 0;
	MenuStage() : Stage("menu") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);

};


class talkStage : public Stage { 
public:
	talkStage() : Stage("talk") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);
};

class talkobjectStage : public Stage {
public:
	talkobjectStage() : Stage("talkobject") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);
};


class catchStage : public Stage {
public:
	bool msgfinal = true;
	catchStage() : Stage("catch") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);
};

class endStage : public Stage {
public:
	endStage() : Stage("end") {}
	void render(Image& fb, float time, sGameData& myGame);
	void update(double seconds_elapsed, sGameData& myGame);

};



//function out of any class

#endif
