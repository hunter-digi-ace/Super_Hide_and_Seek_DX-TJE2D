#include "functions.h"
#include <iostream>
#include <random>


functions::functions()
{
}

int* functions::readCSV(std::string filesrc, int size) { //archivo y tamaño de area
	int pos = 0;
	int* mapborder = new int[size];
	ifstream file;
	file.open(filesrc, fstream::in);
	if (!file.is_open()) {
		fprintf(stderr, "Error locating the file map");
	}
	int aux = 0;
	while (file.good()) {
		string line;
		while (getline(file, line)) {   // get a whole line
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				int aux;
				istringstream(line) >> aux;
				mapborder[pos++] = aux;
			}
		}
	}
	/*
	fprintf(stderr, "%d, \n", myGame.actualArea);
	int actualpos = 0;
	while (10 * 7 > actualpos) {
		fprintf(stderr, "%d,", myGame.world.areas[myGame.actualArea].tipo[actualpos]);
		if (actualpos == 0) {
		}
		else if ((actualpos + 1) % 10 == 0)
		{
			cout << '\n';
		}
		actualpos += 1;
	}
	*/
	return mapborder;
}
void functions::setdinner(sGameData& myGame) {
	myGame.world.areas[31].npcs[21] = 18;
	myGame.world.areas[31].npcs[22] = 20;
	myGame.world.areas[31].npcs[23] = 22;
	myGame.world.areas[31].npcs[24] = 24;
	myGame.world.areas[31].npcs[41] = 26;
	myGame.world.areas[31].npcs[42] = 28;
	myGame.world.areas[31].npcs[43] = 30;
	myGame.world.areas[31].npcs[44] = 32;
	myGame.player.pos = Vector2(16*7,16*4);
	myGame.player.facing = 5;

}
void functions::updateTime(sGameData& myGame, float elapsedTime) {
	myGame.time += elapsedTime;
	functions::updateSpeed(myGame, elapsedTime);
}

void functions::updateSpeed(sGameData& myGame, float elapsedTime) {
	myGame.player.speedtime -= elapsedTime;
	if (myGame.player.speedtime <= 0) {
		myGame.player.speedtime = 0;
		myGame.player.speed = false;
	}
	
}
void functions::addSpeed(sGameData& myGame) {
	myGame.player.speedtime += 5;
	myGame.player.speed = true;
	if (myGame.player.speedtime > 15) {
	myGame.player.speedtime = 15;
	}
}

void functions::saveGameInfo(sGameData& myGame){
	
	sGameData save = myGame;
	//fill here game_info with all game data
	//...
	//save to file
	FILE* fp = fopen("savegame.bin", "wb");
	fwrite(&save, sizeof(sGameData), 1, fp);
	fclose(fp);
}


bool functions::loadGameInfo(sGameData& myGame){
	std::ifstream infile("savegame.bin");
	if (infile.good()) {
		sGameData load;
		//load file
		FILE* fp = fopen("savegame.bin", "rb");
		if (fp == NULL) //no savegame found
			return false;

		fread(&load, sizeof(sGameData), 1, fp);
		fclose(fp);

		myGame = load;

		return true;
	}
	else {
		functions::reset(myGame);
	}
}

string functions::timeformat(float time) {
	int timemins = (int)time / 60;
	if (timemins >99) {
		timemins = 99;
	}
	string mins = toString(timemins);
	if (((int)time / 60) < 10) {
		mins = "0" + mins;
	}
	string timesecs = toString(time - timemins * 60);
	if ((time - timemins * 60 )<10) {
		timesecs = "0"+timesecs;
	}
	return mins + ":" + timesecs.substr(0, timesecs.find("."));//+ 2);
}

void functions::setPlayerPosX(int posX, int area, sGameData& myGame) {
	myGame.player.pos.x = posX;
	myGame.actualArea = area;
	myGame.player.future_pos = myGame.player.pos;
}


void functions::setPlayerPosY(int posY, int area, sGameData& myGame) {
	myGame.player.pos.y = posY;
	myGame.actualArea = area;
	myGame.player.future_pos = myGame.player.pos;
}

void functions::setPlayerPosXY(int posX, int posY, int area, sGameData& myGame) {
	
	myGame.player.pos.y = posY ;
	myGame.player.pos.x = posX;
	myGame.actualArea = area;
	myGame.player.future_pos = myGame.player.pos;
}

void functions::changeMap(sGameData& myGame) {
	if (myGame.world.areas[myGame.actualArea].tipo[functions::posActual(myGame)] == DOOR) { ///pasar por puertas
		if (myGame.player.facing == UP) {
		}
		else if (myGame.player.facing == DOWN) {
		}
		myGame.doorpased = true;
		//std::cout << "posicion: " + toString(myGame.world.areas[myGame.world.areas[myGame.actualArea].doorWarp].doorPos[0]) + ", " +toString(myGame.world.areas[myGame.world.areas[myGame.actualArea].doorWarp].doorPos[1]) << std::endl;

		functions::setPlayerPosXY(myGame.world.areas[myGame.world.areas[myGame.actualArea].doorWarp].doorPos[0],
			myGame.world.areas[myGame.world.areas[myGame.actualArea].doorWarp].doorPos[1],
			myGame.world.areas[myGame.actualArea].doorWarp, myGame);
	}

	if (myGame.player.pos.x +8> 160) { //salir del mapa por provar //152
		functions::setPlayerPosX(-8, myGame.actualArea + 1, myGame);
	}
			
	if (myGame.player.pos.x + 8 < 0) { //salir del mapa // este funciona bien
		functions::setPlayerPosX(152, myGame.actualArea - 1, myGame);
	}

	//la y funciona bien
	if (myGame.player.pos.y + 8 > 112) { //salir del mapa
		functions::setPlayerPosY(-8, myGame.actualArea + 5, myGame);
	}
	if (myGame.player.pos.y + 8 < 0) { //salir del mapa
		functions::setPlayerPosY(104, myGame.actualArea - 5, myGame);
	}
}

int functions::posActual(sGameData& myGame) {
	int aux = ((int(myGame.player.pos.x + 8) /16)) + (10 * (int(myGame.player.pos.y + 8) / 16)); //pasa la posicion pixel a celda
	//std::cout << toString(aux) + ", ";
	return aux;
}

void functions::hideChilds(sGameData& myGame, bool inTutorial) {

	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distrArea(0, 39); // define the range
	std::uniform_int_distribution<> distrCell(11, 48); // define the range

	//distr(eng)  //for generate numbers
		
	int child[8] = { 18, 20, 22, 24, 26, 28, 30, 32 };
	int aux = 0;
	
	while (aux < 8) {
		int randArea = distrArea(eng);
		if (randArea != 25 && randArea != 26 && randArea != 30 && randArea != 35 && randArea != 31 && randArea != 32) {
			if (!myGame.world.areas[randArea].hasChild) {
				int randCell = distrCell(eng);
				if (randCell != 32 && randCell != 32 && randCell != 22 && randCell%10 !=0 && randCell != 19 && randCell != 29 && randCell != 39) {
					if (myGame.world.areas[randArea].tipo[randCell] == EMPTY) {
						myGame.world.areas[randArea].npcs[randCell] = child[aux]; ////// hacer que se puedan hablar mas de una vez
						myGame.world.areas[randArea].tipo[randCell] = NPC;
						myGame.world.areas[randArea].hasChild = true;
						aux++;
					}
				}
			}
		}
	}
}

int functions::posFuturaExtended(int x, int y, sGameData& myGame) {
	int xpos = (int(myGame.player.future_pos.x + x) / 16);
	int ypos = (int(myGame.player.future_pos.y + y) / 16);
	if (xpos > 9) { xpos = 9; }///////////////////////////////////el maldito error estaba aqui 2 dias perdidos e.e
	if (xpos < 0) { xpos = 0; }
	if (ypos > 6) { ypos = 6; }
	if (ypos < 0) {	ypos = 0; }
	int aux = ( xpos + (10 * ypos));
	return aux;
}

int functions::canMove(sGameData& myGame) { 

	int canmove = 0;

	if ((myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(4, 6, myGame)] == EMPTY
		|| myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(4, 6, myGame)] == DOOR)
		&& (myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(11, 6, myGame)] == EMPTY
			|| myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(11, 6, myGame)] == DOOR)
		&& (myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(4, 15, myGame)] == EMPTY
			|| myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(4, 15, myGame)] == DOOR)
		&& (myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(11, 15, myGame)] == EMPTY
			|| myGame.world.areas[myGame.actualArea].tipo[functions::posFuturaExtended(11, 15, myGame)] == DOOR)
		
		) {
		canmove =  1;
	}
	else {
		canmove = 0;
	}
	//std::cout << myGame.world.areas[((int(myGame.player.pos.x) * 20 / 160) + 1) + (20 * (int(myGame.player.pos.y) * 15 / 120) + 2)];
	return canmove;
}


void functions::move(int dir, double seconds_elapsed, sGameData& myGame) {
	if (dir == UP) {
		myGame.player.future_pos.y -= seconds_elapsed * myGame.player.player_speed;
		myGame.player.facing = FACE_UP;
	}
	if (dir == DOWN) {
		myGame.player.future_pos.y += seconds_elapsed * myGame.player.player_speed;
		myGame.player.facing = FACE_DOWN;
	}
	if (dir == LEFT) {
		myGame.player.future_pos.x -= seconds_elapsed * myGame.player.player_speed;
		myGame.player.facing = FACE_LEFT;
	}
	if (dir == RIGHT) {
		myGame.player.future_pos.x += seconds_elapsed * myGame.player.player_speed;
		myGame.player.facing = FACE_RIGHT;
	}


	if (functions::canMove(myGame)) {   ////pasar esto 4 veces con las 4 posiciones
		
		if (dir == UP) {
			myGame.player.pos.y -= seconds_elapsed * myGame.player.player_speed;
			myGame.player.facing = FACE_UP;
		}
		if (dir == DOWN) {
			myGame.player.pos.y += seconds_elapsed * myGame.player.player_speed;
			myGame.player.facing = FACE_DOWN;
		}
		if (dir == LEFT) {
			myGame.player.pos.x -= seconds_elapsed * myGame.player.player_speed;
			myGame.player.facing = FACE_LEFT;
		}
		if (dir == RIGHT) {
			myGame.player.pos.x += seconds_elapsed * myGame.player.player_speed;
			myGame.player.facing = FACE_RIGHT;
		}
	}
	else {
		myGame.player.future_pos = myGame.player.pos;
	}
}

bool functions::inHouse(sGameData& myGame) {
	if (myGame.actualArea == 31 || myGame.actualArea == 32 || myGame.actualArea == 35) {
		return 1;
	}
	return 0;
}

void functions::reset(sGameData& myGame) { ////poner todo a 0 y reiniciar partida
	int actualpos = 0;
	while (50 * 56 > actualpos) {

		memcpy(&myGame.world.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].mapa[(actualpos % 10 + (actualpos / 50) * 10) % 70], &myGame.Resetworld.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].mapa[(actualpos % 10 + (actualpos / 50) * 10) % 70], sizeof(int));
		memcpy(&myGame.world.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].npcs[(actualpos % 10 + (actualpos / 50) * 10) % 70], &myGame.Resetworld.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].npcs[(actualpos % 10 + (actualpos / 50) * 10) % 70], sizeof(int));
		memcpy(&myGame.world.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].tipo[(actualpos % 10 + (actualpos / 50) * 10) % 70], &myGame.Resetworld.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].tipo[(actualpos % 10 + (actualpos / 50) * 10) % 70], sizeof(int));
		myGame.world.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].hasChild = false;
		myGame.world.areas[(actualpos / 10) % 5 + actualpos / 350 * 5].objeto[(actualpos % 10 + (actualpos / 50) * 10) % 70] = BOTA;
		actualpos += 1;
	}

	functions::initGame(myGame);
}

void functions::comunicatemaps(sGameData& myGame, int map1, int map2) {
	myGame.world.areas[map1].doorWarp = map2; //mundo tutorial
	myGame.world.areas[map2].doorWarp = map1; //casa tutorial
}



void functions::initGame(sGameData& myGame) {

	myGame.player.pos.y = 16 * 4;
	myGame.player.future_pos.y = 16 * 4;
	myGame.player.speedtime = 0.0;

	if (myGame.inTutorial == true) {
		myGame.actualArea = 35;
		myGame.numChilds = 3;
		myGame.player.future_pos.x = 16 * 4;
		myGame.player.pos.x = 16 * 4;
	}
	else {
		myGame.actualArea = 31;
		myGame.numChilds = 8; 
		myGame.player.pos.x = 16 * 6;
		myGame.player.future_pos.x = 16 * 6;
		functions::hideChilds(myGame, myGame.inTutorial);
	}
	myGame.time = 0;
	myGame.player.facing = DOWN;
	myGame.counter = 0;






}

void functions::loadMap(string location, sGameData& myGame) { ///////////cargamos el mundo y los interiores

	int* bigMap[3][50 * 56];
	memcpy(&bigMap[0], functions::readCSV(location + "_mapa.csv", (50 * 56)), (50 * 56) * sizeof(int));// cargar datos de los npc mundo
	memcpy(&bigMap[1], functions::readCSV(location + "_npc.csv", (50 * 56)), (50 * 56) * sizeof(int));// cargar datos de los npc
	memcpy(&bigMap[2], functions::readCSV(location + "_tipo.csv", (50 * 56)), (50 * 56) * sizeof(int));// cargar datos de los npc


	int actualpos = 0;
	while (50 * 56 > actualpos) {
		int aux1 = (actualpos / 10) % 5 + actualpos / 350 * 5;
		int aux2 = (actualpos % 10 + (actualpos / 50) * 10) % 70;
		memcpy(&myGame.world.areas[aux1].mapa[aux2], &bigMap[0][actualpos], sizeof(int));  //copiamos en la estructura
		memcpy(&myGame.world.areas[aux1].npcs[aux2], &bigMap[1][actualpos], sizeof(int));
		memcpy(&myGame.world.areas[aux1].tipo[aux2], &bigMap[2][actualpos], sizeof(int));
		
		//borrar si implemento npc fisico
		memcpy(&myGame.Resetworld.areas[aux1].mapa[aux2], &myGame.world.areas[aux1].mapa[aux2], sizeof(int)); //copiamos para el reset
		memcpy(&myGame.Resetworld.areas[aux1].npcs[aux2], &myGame.world.areas[aux1].npcs[aux2], sizeof(int));
		memcpy(&myGame.Resetworld.areas[aux1].tipo[aux2], &myGame.world.areas[aux1].tipo[aux2], sizeof(int));
				
		actualpos += 1;
	}
	
	
	for (size_t i = 0; i < 40; i++) {
		for (int n = 0; n < 10; n++) {
			for (int m = 0; m < 7; m++) {
				if (myGame.world.areas[i].tipo[m * 10 + n] == DOOR) { //guardamos las posiciones de las puertas
					myGame.world.areas[i].doorPos[0] = n * 16;
					myGame.Resetworld.areas[i].doorPos[0] = n * 16;
					if (i <= 25 ||i ==37) {
						myGame.world.areas[i].doorPos[1] = m * 16 + 8;
						myGame.Resetworld.areas[i].doorPos[1] = m * 16 + 8;
					}
					else {
						myGame.world.areas[i].doorPos[1] = m * 16 - 8;
						myGame.Resetworld.areas[i].doorPos[1] = m * 16 - 8;
					}
				}
			}
		}
	}
}

bool functions::isKid(int npc) {
	if ( npc == 18 || npc == 20 || npc == 22 || npc == 24 || npc == 26 || npc == 28 || npc == 30 || npc == 32){
		//std::cout << "is a kid" << std::endl;
		return true;
	}
	else {
		return false;
	}
}

string* functions::getMesagge(int npc, sGameData& myGame ) { //maximo 28 caracteres por linea, 5 lineas
	
	if (npc == -1) {
		if (myGame.actualArea == 10 || myGame.actualArea == 25) {
			static string message[5] = {	"Sabias que si pulsas la ",      //cartel area 25
											"tecla X puedes abrir el",
											"menu del juego? ",
											"",
											""
			};
			return message;
		}
	}
	if (npc == -2) {
		static string message[5] = {		"Ya los he encontrado a todos",  //todos encontrados
											"deberia ir a casa y hablar  ",
											"con sus padres, seguro que  ",
											"estaran contentos!",
											"-*-  vuelve a su casa  -*-"
		};
		return message;
	}
	if (npc == 16) {
		static string message[5] = { "A ver si puedes  jugar con",  //mensaje madre
											"ellos mas a menudo, te lo ",
											"agradeceriamos mucho! ",
											"",
											""
		};
		return message;
	}
	if (npc == 34) {
		static string message[5] = { "Cuando era joven en esta",  //mensaje abuela
											"isla se comerciaba con    ",
											"mantequilla, y estabamos   ",
											"bien, ahora estan todos",
											"delgaduchos..."
		};
		return message;
	}
	if (npc == 36) {
		static string message[5] = { "Cuando estabamos vivos ",  //mensaje pirata
											"eramos los reyes de esta  ",
											"isla, escondiamos la        ",
											"mantequilla por todas partes",
											""
		};
		return message;
	}
	if (npc == 12) {
		if (myGame.counter == myGame.numChilds && !myGame.inTutorial) {      ///mensajes padre
			static string message[5] = {	"Muchas gracias por ir a ",      //fin juego
											"buscarlos tan rapido, ahora",
											"podremos cenar todos juntos",
											"si quieres puedes quedarte a",
											"cenar con nosotros <3"
			};
			return message;
		}
		else if (myGame.counter == myGame.numChilds && myGame.inTutorial) {
			static string message[5] = {	"Muy bien, has completado el",   //fin tutorial
											"tutorial, para volver al",
											"menu pulsa X y selecciona",
											"    'volver al intro'",
											""
			};
			return message;
		}
		else {
			static string message[5] = {	"Mis hijos ya han salido, ",     //mensaje padre inicial
											"ahora te toca ir a buscarlos",
											"por  toda la isla, si les   " ,
											"hablas volveran a casa solos" ,
											"-no tengas mucha prisa!-" };
			return message;
		}
	}
	////animales

	if (npc == 38 || npc == 40 || npc == 42 ) {
		static string message[5] = { "Cucuu.. cucuu.. ",  //mensaje loros
											"'que olorcillo a mantequilla ",
											"tiene este duende'   ",
											"cucuuu... cucuu..",
											""
		};
		return message;
	}
	if (npc == 44 || npc == 46 ) {
		static string message[5] = { "U-aaaa! u-aaa!",  //mensaje monos
											"'Duende malholiente fuera de",
											"mi lado'   ",
											"u-uuu! u-aaaa!",
											"u-aaaaaaaaa!"
		};
		return message;
	}

	if (npc == 48 || npc == 50) {
		static string message[5] = { "Boing boing",  //mensaje conejos
											"'porque me habla este ",
											"duende ? '",
											"boing",
											""
		};
		return message;
	}
	if (npc == 54 ) {
		static string message[5] = { "Guau! Guau!",  //mensaje perro
											"'que bien huele este   ",
											"duendecillo' le importara si",
											"le mordisqueo un poco?",
											"Guau!"
		};
		return message;
	}
	if (npc == 52) {
		static string message[5] = { "Aaaaaaaaa",  //mensaje zorro
											"'que hambre, mira este  ",
											"duendecillo podria comermelo",
											"si estubiera con mis amigos'",
											"Aaaaaa"
		};
		return message;
	}

	if (npc == 56) {
		static string message[5] = { "Miauuuu",  //mensaje gato
											"'dios mio que no se me ",
											"acerque esta cosa grasienta",
											"que me acabo de limpiar'",
											"Miauuu"
		};
		return message;
	}

	///////////niños
	if (npc == 18) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Jolin que poco ha durado la  ",  //mensaje niña amarilla
											"diversion!, a ver si jugamos ",
											"otra partida rapido!    " ,
											"" ,
											"" };
			return message;
		}
		else {
			static string message[5] = { "Rapido acaba la partida y ",  
											"jugamos otra vez!",
											" " ,
											" " ,
											" " };
			return message;
		}
	}
	if (npc == 20) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Wow que rapido me has",  //mensaje niño pelo
											"encontrado, yo pensaba que ",
											"este sitio era el mejor..." ,
											"bueno me voy a casa" ,
											"" };
			return message;
		}
		else {
			static string message[5] = { "Ya veras, estoy pensando un",   //mensaje 
											"sitio aun mejor para",
											"esconderme, seguro que esta  " ,
											"vez te cuesta mas! " ,
											" " };
			return message;
		}
	}

	if (npc == 22) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "jooo, si tuviera las piernas",  //mensaje niña coleta
											"mas largas me podria haber ",
											"escondido mejor.. " ,
											"" ,
											"" };
			return message;
		}
		else {
			static string message[5] = { "Ya veras cuando crezca, me ",   //mensaje 
											"dara tiempo allegar a lo",
											"alto de la torre de los" ,
											"piratas!" ,
											" " };
			return message;
		}
	}

	if (npc == 24) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Glup, esto de correr por ",  //mensaje niño pez
											"encima del agua nunca se me",
											"ha dado bien, haber si me" ,
											"adapto a estar fuera del " ,
											"agua de una vez!" };
			return message;
		}
		else {
			static string message[5] = { "Glup, la proxima vez jugamos",   //mensaje 
											"en el agua, ya veras lo ",
											"rapido que puedo ir alli! " ,
											" " ,
											" " };
			return message;
		}
	}

	if (npc == 26) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Me encanta jugar al  ",  //mensaje niña verde
											"escondite con vosotros",
											"en cada partida descubro un" ,
											"lugar nuevo de la isla!" ,
											"" };
			return message;
		}
		else {
			static string message[5] = { "Creo que ya se a donde voy a",   //mensaje 
											"ir a esconderme esta vez, a ",
											"ver que me encuentro por el" ,
											"camino!" ,
											" " };
			return message;
		}
	}

	if (npc == 28) {
		if (functions::inHouse(myGame)) {
			static string message[5] = {	"Oh, yo ya estal en casa, tu ",  //mensaje niño chino 
											"ya no podel decil nada a mi ",
											"yo ahora jugal con mis    " ,
											"helmanos a saltal en cama." ,
											"^(-.-)>" };
			return message;
		}
		else {
			static string message[5] = {	"Oh no tu pillal a mi, ahora", 
											"yo que voy a hacel, has",
											"traido deshonra sobre mi" ,
											"cabra, me tendre que ir a" ,
											"casa rapido (-.-')" };
			return message;
		}
	}


	if (npc == 30) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Uff.. uff.. con lo que me ha",  //mensaje niño gordo
											"costado llegar aqui, ahora ",
											"tengo que volver a casa.. " ,
											"uff.. uff.. hare un descanso" ,
											"en la plaza..." };
			return message;
		}
		else {
			static string message[5] = { "Uff.. aun estoy cansado.. ",   //mensaje 
											"creo que tendria que hacer ",
											"mas deporte y dejar la   " ,
											"mantequilla.. " ,
											" " };
			return message;
		}
	}

	if (npc == 32) {
		if (!functions::inHouse(myGame)) {
			static string message[5] = { "Ya habiamos empezado?",  //mensaje niña roja
											"es que me he entretenido",
											"mirando el paisaje, a que es" ,
											"muy bonito todo? me pregunto" ,
											"quien lo habra creado :D" };
			return message;
		}
		else {
			static string message[5] = { "Para la proxima intentare no",   //mensaje 
											"distraerme tanto con el",
											"mundo y me escondere mejor!" ,
											"rapido vamos a jugar juntos " ,
											"otra vez!" };
			return message;
		}
	}

	else {
		static string message[5] = { "", "Si, soy un npc", "y con un texto generico!", "...", "" }; // mensaje generico
		return message;
	}

}