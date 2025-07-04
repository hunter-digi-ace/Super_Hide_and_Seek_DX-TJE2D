#include "pantallas.h"

void Stage::changeState(const char* name) {
	current_stage = s_stages[name];
}
Stage::Stage(const char* name) {
	s_stages[name] = this;
	if (current_stage == NULL) {
		current_stage = this;
	}
}
void Stage::printText(Image& fb, sGameData& myGame) {
	string* patata;
	patata = functions::getMesagge(myGame.world.areas[myGame.actualArea].npcs[myGame.talkinTo], myGame);
	for (size_t i = 0; i < 5; i++) {
		fb.drawText(patata[i], 24, 24 + 7 * i, *Image::Get("minifont_w"), 4, 6);
	}
}
void Stage::printObject(Image& fb, sGameData& myGame, bool object) {
	if (object) {
		string patata[5] = { "Has encontrado un poco ",	"de mantequilla, con ","esto podras deslizar" ,	"mas rapido durante" ,"unos segundos!" };
		for (size_t i = 0; i < 5; i++) {
			fb.drawText(patata[i], 24, 24 + 7 * i, *Image::Get("minifont_w"), 4, 6);
		}
	}
	else {
		string patata[5] = { "Esto esta vacio...",	"seguro que no has","recogido algo de " , "aqui antes?" ,"" };
		for (size_t i = 0; i < 5; i++) {
			fb.drawText(patata[i], 24, 24 + 7 * i, *Image::Get("minifont_w"), 4, 6);
		}
	}
	
}

void Stage::printUI(Image& fb, sGameData& myGame) {
	fb.drawImage(*Image::Get("menu"), 1, 112, Area(25, 0, 160, 8));
	fb.drawImage(*Image::Get("menu"), 42, 112, Area(0, 0, (myGame.player.speedtime / 15.0 * 23), 8));
	fb.drawText(functions::timeformat(myGame.time), 110, 114, *Image::Get("minifont_b"), 4, 6);
	fb.drawText(toString(myGame.counter) + "/" + toString(myGame.numChilds), 81, 114, *Image::Get("minifont_b"), 4, 6);

}

void titleStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    INTROSTAGE
	
	
	fb.drawImage(*Image::Get(imageActual), 0, 0, 160, 120);
	fb.drawText("  Press Z to continue", 40, 100, *Image::Get("minifont_b"), 4, 6);
}
void titleStage::update(double seconds_elapsed, sGameData& myGame) {
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
		if (order == 0) {

			imageActual = "intro2";
			order = 1;
		}
		else if (order == 1) {
			imageActual = "intro3";
			order = 2;
		}
		else if (order == 2) {
			imageActual = "titlescreen";
			order = 3;
		}
		else if (order == 3) {


			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/player-select.wav", 1, false);
			changeState("intro");

		}
	}
}

void IntroStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    MENUSTAGE
	//Stage::s_stages["play"]->render(fb, time);

	fb.drawImage(*Image::Get("menuintro"), 0, 0, 160, 120);
	fb.drawText(" >", 75, menupos * 10 + 72, *Image::Get("minifont_b"), 4, 6);
	fb.drawText("   * Tutorial *", 75, 72, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("   Nueva partida", 75, 82, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("   Cargar partida", 75, 92, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("   Salir del Juego", 75, 102, *Image::Get("minifont_w"), 4, 6);


}
void IntroStage::update(double seconds_elapsed, sGameData& myGame) {

	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		menupos = 0;
		Game::instance->synth.samples_playback->stop();
		Game::instance->synth.playSample("data/mydata/intro.wav", 1, false);
		changeState("title");
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {

		Game::instance->synth.playSample("data/mydata/cursor.wav", 1, false);
		menupos -= 1;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {

		Game::instance->synth.playSample("data/mydata/cursor.wav", 1, false);
		menupos += 1;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {

		Game::instance->synth.playSample("data/mydata/select.wav", 1, false);
		if (menupos == 0) {
			menupos = 0;
			myGame.inTutorial = true;
			functions::reset(myGame);


			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
			changeState("play");
		}
		else if (menupos == 1) {
			menupos = 0;
			myGame.inTutorial = false;
			functions::reset(myGame);


			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
			changeState("play");
		}
		else if (menupos == 2) {
			menupos = 0;
			functions::loadGameInfo(myGame);


			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
			changeState("play");
		}
		else if (menupos == 3) {
			menupos = 0;
			Game::instance->must_exit = true;
		}
	}
	if (menupos < 0) { menupos = 0; }
	else if (menupos > 3) { menupos = 3; }
}

void PlayStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    PLAYSTAGE

	
	fb.drawMap(*Image::Get("tileSet"), myGame.world.areas[myGame.actualArea].mapa, myGame.world.areas->size_components, time);
	fb.drawNpcs(*Image::Get("sprite"), myGame.world.areas[myGame.actualArea].npcs, myGame.world.areas->size_components, time);
	fb.drawImage(*Image::Get("sprite"), myGame.player.pos.x, myGame.player.pos.y, Area((int(time * 5) % 2) * 16, myGame.player.facing * 16, 16, 16));
	Stage::printUI(fb, myGame);
}
void PlayStage::update(double seconds_elapsed, sGameData& myGame) {

	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		Game::instance->synth.samples_playback->stop();
		changeState("menu");
	}
	if (myGame.doorpased == true) {
		Game::instance->synth.playSample("data/mydata/door.wav", 1, false);
		myGame.doorpased = false;
	}

	functions::changeMap(myGame);
	functions::updateTime(myGame, Game::instance->elapsed_time);
	//Read the keyboard state, to see all the keycodes: https://wiki.libsdl.org/SDL_Keycode
	if (Input::isKeyPressed(SDL_SCANCODE_UP)) {//if key up
		functions::move(UP, seconds_elapsed, myGame);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_DOWN)) {//if key down
		functions::move(DOWN, seconds_elapsed, myGame);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_LEFT)) {//if key down
		functions::move(LEFT, seconds_elapsed, myGame);
	}
	if (Input::isKeyPressed(SDL_SCANCODE_RIGHT)) {//if key down
		functions::move(RIGHT, seconds_elapsed, myGame);
	}

	if (Input::isKeyPressed(SDL_SCANCODE_Y)) {//if key down
		myGame.counter = myGame.numChilds;
	}
	if (myGame.player.speed) {//
		myGame.player.player_speed = 110;
	}
	else {
		myGame.player.player_speed = 55;
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) { //if key Z was pressed
		int aux = functions::posActual(myGame);
		if (myGame.player.facing == FACE_UP) {
			myGame.talkinTo = aux - 10;
			if (myGame.world.areas[myGame.actualArea].tipo[aux - 10] == NPC) {
				if (functions::isKid(myGame.world.areas[myGame.actualArea].npcs[aux - 10]) && !functions::inHouse(myGame)) {///////////////////////////////////////////////////////////////////
					Game::instance->synth.playSample("data/mydata/catch.wav", 1, false);
					changeState("catch");
				} else {
					changeState("talk");
				}
			}else if (myGame.world.areas[myGame.actualArea].tipo[aux - 10] == OBJECT) {
				if (myGame.world.areas[myGame.actualArea].objeto[myGame.talkinTo] == BOTA) {
					functions::addSpeed(myGame);
					myGame.hasobject = true;
					Game::instance->synth.playSample("data/mydata/object.wav", 1, false);
				}
				changeState("talkobject");
			}
		}
		if (myGame.player.facing == FACE_DOWN) {
			myGame.talkinTo = aux + 10;
			if (myGame.world.areas[myGame.actualArea].tipo[aux + 10] == NPC) {
				if (functions::isKid(myGame.world.areas[myGame.actualArea].npcs[aux + 10]) && !functions::inHouse(myGame)) {///////////////////////////////////////////////////////////////////
					Game::instance->synth.playSample("data/mydata/catch.wav", 1, false);
					changeState("catch");
				} else {
					changeState("talk");
				}
			}
			else if (myGame.world.areas[myGame.actualArea].tipo[aux + 10] == OBJECT) {
				if (myGame.world.areas[myGame.actualArea].objeto[myGame.talkinTo] == BOTA) {
					functions::addSpeed(myGame);
					myGame.hasobject = true;
					Game::instance->synth.playSample("data/mydata/object.wav", 1, false);
				}
				changeState("talkobject");
			}
		}
		if (myGame.player.facing == FACE_LEFT) {
			myGame.talkinTo = aux - 1;
			if (myGame.world.areas[myGame.actualArea].tipo[aux - 1] == NPC) {
				if (functions::isKid(myGame.world.areas[myGame.actualArea].npcs[aux - 1]) && !functions::inHouse(myGame)) {///////////////////////////////////////////////////////////////////
					Game::instance->synth.playSample("data/mydata/catch.wav", 1, false);
					changeState("catch");
				} else {
					changeState("talk");
				}
			}
			else if (myGame.world.areas[myGame.actualArea].tipo[aux - 1] == OBJECT) {
				if (myGame.world.areas[myGame.actualArea].objeto[myGame.talkinTo] == BOTA) {
					functions::addSpeed(myGame);
					myGame.hasobject = true;
					Game::instance->synth.playSample("data/mydata/object.wav", 1, false);
				}
				changeState("talkobject");
			}
		}
		if (myGame.player.facing == FACE_RIGHT) {
			myGame.talkinTo = aux + 1;
			if (myGame.world.areas[myGame.actualArea].tipo[aux + 1] == NPC ) {
				if (functions::isKid(myGame.world.areas[myGame.actualArea].npcs[aux + 1]) && !functions::inHouse(myGame)) {///////////////////////////////////////////////////////////////////
					Game::instance->synth.playSample("data/mydata/catch.wav", 1, false);
					changeState("catch");
				} else {
					changeState("talk");
				}
			}
			else if (myGame.world.areas[myGame.actualArea].tipo[aux + 1] == OBJECT) {
				if (myGame.world.areas[myGame.actualArea].objeto[myGame.talkinTo] == BOTA) {
					functions::addSpeed(myGame);
					myGame.hasobject = true;
					Game::instance->synth.playSample("data/mydata/object.wav", 1, false);
				}
				changeState("talkobject");
			}
		}
	}
}



void MenuStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    MENUSTAGE
	//Stage::s_stages["play"]->render(fb, time);
	Stage::s_stages["play"]->render(fb, time, myGame);
	fb.fillBlend(Color(0, 0, 0, 180));
	fb.drawText(" >", 46, menupos * 10 + 50, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("  Volver al juego", 50, 50, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("  Guardar y salir", 50, 60, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("  Reiniciar juego", 50, 70, *Image::Get("minifont_w"), 4, 6);
	fb.drawText("  Volver al intro", 50, 80, *Image::Get("minifont_w"), 4, 6);

}
void MenuStage::update(double seconds_elapsed, sGameData& myGame) {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_X)) {
		menupos = 0;


		Game::instance->synth.samples_playback->stop();
		Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
		changeState("play");
	}
	
	if (Input::wasKeyPressed(SDL_SCANCODE_UP)) {
		Game::instance->synth.playSample("data/mydata/cursor.wav", 1, false);
		menupos -= 1;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN)) {
		Game::instance->synth.playSample("data/mydata/cursor.wav", 1, false);
		menupos += 1;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {
		

		if (menupos == 0) {
			

			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
			Game::instance->synth.playSample("data/mydata/select.wav", 1, false);
			changeState("play");
		}
		else if (menupos == 1) {
			functions::saveGameInfo(myGame);



			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/player-select.wav", 1, false);
			Game::instance->synth.playSample("data/mydata/select.wav", 1, false);

			myGame.time = Game::instance->elapsed_time ;
			menupos = 0;
			changeState("intro");
		}
		else if (menupos == 2) {


			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/mabe-village.wav", 1, true);
			Game::instance->synth.playSample("data/mydata/select.wav", 1, false);
			functions::reset(myGame); //por mejorar

			changeState("play");
		}
		else if (menupos == 3) {

			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/player-select.wav", 1, false);
			Game::instance->synth.playSample("data/mydata/select.wav", 1, false);

			menupos = 0;
			myGame.inTutorial = false;
			changeState("intro");
		}
	}
	if (menupos < 0) {	menupos = 0;}
	else if (menupos > 3) {	menupos = 3;}
}

void talkStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    TALKSTAGE
	Stage::s_stages["play"]->render(fb, time, myGame);
	Stage::printUI(fb, myGame);
	fb.fillBlend(Color(0, 0, 0, 180));
	fb.drawRectangle(20, 20, 119, 42, Color(200, 200, 200));// , 230));
	Stage::printText(fb, myGame);
}
void talkStage::update(double seconds_elapsed, sGameData& myGame) {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {//if key Z was pressed
		if (myGame.counter == myGame.numChilds && myGame.world.areas[myGame.actualArea].npcs[myGame.talkinTo] == 12 && !myGame.inTutorial) {
			functions::setdinner(myGame);
			Game::instance->synth.samples_playback->stop();
			Game::instance->synth.playSample("data/mydata/end.wav", 1, true);
			changeState("end");
		}
		else {

			
			changeState("play");
		}
	}
}

void talkobjectStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    TALKSTAGE
	Stage::s_stages["play"]->render(fb, time, myGame);
	fb.fillBlend(Color(0, 0, 0, 180));
	fb.drawRectangle(20, 20, 119, 42, Color(200, 200, 200));
	if (myGame.hasobject) {
		fb.drawImage(*Image::Get("sprite"), 118, 41, Area(0, 4 * 16, 16, 16));
		fb.drawImage(*Image::Get("menu"), 122, 32, Area(0, 0, 7, 8));
		Stage::printObject(fb, myGame, true);
	}
	else {
		fb.drawImage(*Image::Get("sprite"), 118, 41, Area(16, 4 * 16, 16, 16));
		Stage::printObject(fb, myGame, false);
	}
}
void talkobjectStage::update(double seconds_elapsed, sGameData& myGame) {
	
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {//if key Z was pressed

		myGame.world.areas[myGame.actualArea].objeto[myGame.talkinTo] = NOTHING;
		myGame.hasobject = false;

		changeState("play");
	}
}



void catchStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    CATCHSTAGE


	Stage::s_stages["play"]->render(fb, time, myGame);
	fb.fillBlend(Color(0, 0, 0, 180));
	
	if (myGame.counter == myGame.numChilds ) {
		fb.drawRectangle(20, 20, 119, 42, Color(100, 200, 100, 230));
		for (size_t i = 0; i < 5; i++) {
			fb.drawText(functions::getMesagge(-2, myGame)[i], 24, 24 + 7 * i, *Image::Get("minifont_w"), 4, 6);
		}
		msgfinal = false;
	}	
	else {
		fb.drawRectangle(20, 20, 119, 42, Color(200, 200, 200, 230));
		Stage::printText(fb, myGame);
	}
	
}
void catchStage::update(double seconds_elapsed, sGameData& myGame) {
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) {//if key Z was pressed
		
		if (myGame.counter != myGame.numChilds) {
			if (myGame.inTutorial == true) {
				myGame.world.areas[35].npcs[36 + myGame.counter] = myGame.world.areas[myGame.actualArea].npcs[myGame.talkinTo]; ////// hacer que se puedan hablar mas de una vez
				myGame.world.areas[35].tipo[36 + myGame.counter] = NPC;
			}
			else {
				myGame.world.areas[32].npcs[31 + myGame.counter] = myGame.world.areas[myGame.actualArea].npcs[myGame.talkinTo]; ////// hacer que se puedan hablar mas de una vez
				myGame.world.areas[32].tipo[31 + myGame.counter] = NPC;
			}
			myGame.world.areas[myGame.actualArea].npcs[myGame.talkinTo] = -1;
			myGame.world.areas[myGame.actualArea].tipo[myGame.talkinTo] = EMPTY;
			myGame.counter++;
		}		
		if (myGame.counter == myGame.numChilds && msgfinal) {

			changeState("catch");
		}
		else {

			changeState("play");
		}

		std::cout << toString(myGame.counter) + "/" + toString(myGame.numChilds) << std::endl;
	}
}


void endStage::render(Image& fb, float time, sGameData& myGame) {///////////////////////////////////////////////////////////////////////    MENUSTAGE
	//Stage::s_stages["play"]->render(fb, time);
	Stage::s_stages["play"]->render(fb, time, myGame);
	fb.drawRectangle(18, 82, 125, 24, Color(200, 200, 200));
																	//max 30 caracteres
	string patata[3] = {"Felicidades! has tardado "+ functions::timeformat(myGame.time),"  Intenta mejorar tu tiempo!","    Pulsa Z + X para salir"};
	for (size_t i = 0; i < 3; i++) {
		fb.drawText(patata[i], 21, 84 + 7 * i, *Image::Get("minifont_b"), 4, 6);
	}

}

void endStage::update(double seconds_elapsed, sGameData& myGame) {


	if (Input::wasKeyPressed(SDL_SCANCODE_X) && Input::wasKeyPressed(SDL_SCANCODE_Z)) {


		Game::instance->synth.samples_playback->stop();
		Game::instance->synth.playSample("data/mydata/intro.wav", 1, false);
		changeState("title");
	}


}