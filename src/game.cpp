#include "game.h"
#include "pantallas.h"



////////////////////////////
////////////////////////////

Game* Game::instance = NULL;

Color bgcolor(130, 80, 100);

sGameData myGame;

Stage* Stage::current_stage = NULL;
std::map<std::string, Stage*> Stage::s_stages;


Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;


	//cargamos las imagenes
	font = Image::Get("data/bitmap-font-white.tga");
	font->registerAs("font");
	fontb = Image::Get("data/bitmap-font-black.tga");
	fontb->registerAs("fontb");
	minifont_b = Image::Get("data/mini-font-black-4x6.tga"); //load bitmap-font image
	minifont_b->registerAs("minifont_b");
	minifont_w = Image::Get("data/mini-font-white-4x6.tga"); //load bitmap-font image
	minifont_w->registerAs("minifont_w");
	sprite = Image::Get("data/mydata/sprites.tga"); //example to load an sprite
	sprite->registerAs("sprite");
	menu = Image::Get("data/mydata/barra_menu.tga"); //example to load an sprite
	menu->registerAs("menu");
	tileSet = Image::Get("data/mydata/tileset_mapa.tga"); //example to load an sprite
	tileSet->registerAs("tileSet");
	intro = Image::Get("data/mydata/intro.tga"); //example to load an sprite
	intro->registerAs("intro");
	intro1 = Image::Get("data/mydata/intro1.tga"); //example to load an sprite
	intro1->registerAs("intro1");
	intro2 = Image::Get("data/mydata/intro2.tga"); //example to load an sprite
	intro2->registerAs("intro2");
	intro3 = Image::Get("data/mydata/intro3.tga"); //example to load an sprite
	intro3->registerAs("intro3");
	titlescreen = Image::Get("data/mydata/titlescreen.tga"); //example to load an sprite
	titlescreen->registerAs("titlescreen");
	menuintro = Image::Get("data/mydata/menu.tga"); //example to load an sprite
	menuintro->registerAs("menuintro");




	//cargamos el mapa del mundo
	string location = "data/mydata/world_final";
	functions::loadMap(location, myGame);


	///comunicamos las puertas
	functions::comunicatemaps(myGame, 35, 25); //tutorial casa-mundo
	functions::comunicatemaps(myGame, 31, 10); //juego casa-mundo
	functions::comunicatemaps(myGame, 38, 37); //torre piso1-2
	functions::comunicatemaps(myGame, 9, 36); //mundo-torre
	functions::comunicatemaps(myGame, 24, 33); //playa-cueva monos
	functions::comunicatemaps(myGame, 7, 34); //mundo-guarida zorro
	functions::comunicatemaps(myGame, 16, 39); //mundo-guarida conejos


	//mapborders2 = readCSV("data/mydata/test_world.csv"); // cargar datos del mapa
	enableAudio(); //enable this line if you plan to add audio to your application
	//synth.playSample("data/mydata/mabe-village.wav", 1, true);
	//synth.osc1.amplitude = 0.5;
	synth.volume = 0.1;

	Stage::current_stage = new titleStage();
	new IntroStage();
	new PlayStage();
	new MenuStage();
	new talkStage();
	new talkobjectStage();
	new catchStage();
	new endStage();

	Game::instance->synth.playSample("data/mydata/intro.wav", 1, false);
}

//what to do when the image has to be draw
void Game::render(void)
{
	//Create a new Image (or we could create a global one if we want to keep the previous frame)
	Image framebuffer(160, 120); //do not change framebuffer size

	//add your code here to fill the framebuffer
	//...

	Stage::current_stage->render(framebuffer, time, myGame);


	showFramebuffer(&framebuffer);
}

void Game::update(double seconds_elapsed)
{
	//Add here your update method
	//...
	Stage::current_stage->update(seconds_elapsed, myGame);

	

	//example of 'was pressed'
	if (Input::wasKeyPressed(SDL_SCANCODE_A)) //if key A was pressed
	{
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) //if key Z was pressed
	{
	}

	//to read the gamepad state
	if (Input::gamepads[0].isButtonPressed(A_BUTTON)) //if the A button is pressed
	{
	}

	if (Input::gamepads[0].direction & PAD_UP) //left stick pointing up
	{
		bgcolor.set(0, 255, 0);
	}
}

//Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch (event.keysym.sym)
	{
	case SDLK_ESCAPE: must_exit = true; break; //ESC key, kill the app
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseMove(SDL_MouseMotionEvent event)
{
}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	window_width = width;
	window_height = height;
}

//sends the image to the framebuffer of the GPU
void Game::showFramebuffer(Image* img)
{
	static GLuint texture_id = -1;
	static GLuint shader_id = -1;
	if (!texture_id)
		glGenTextures(1, &texture_id);

	//upload as texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, img->width, img->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels);

	glDisable(GL_CULL_FACE); glDisable(GL_DEPTH_TEST); glEnable(GL_TEXTURE_2D);
	float startx = -1.0; float starty = -1.0;
	float width = 2.0; float height = 2.0;

	//center in window
	float real_aspect = window_width / (float)window_height;
	float desired_aspect = img->width / (float)img->height;
	float diff = desired_aspect / real_aspect;
	width *= diff;
	startx = -diff;

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex2f(startx, starty + height);
	glTexCoord2f(1.0, 0.0); glVertex2f(startx + width, starty + height);
	glTexCoord2f(1.0, 1.0); glVertex2f(startx + width, starty);
	glTexCoord2f(0.0, 1.0); glVertex2f(startx, starty);
	glEnd();

	/* this version resizes the image which is slower
	Image resized = *img;
	//resized.quantize(1); //change this line to have a more retro look
	resized.scale(window_width, window_height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if (1) //flip
	{
	glRasterPos2f(-1, 1);
	glPixelZoom(1, -1);
	}
	glDrawPixels( resized.width, resized.height, GL_RGBA, GL_UNSIGNED_BYTE, resized.pixels );
	*/
}

//AUDIO STUFF ********************

SDL_AudioSpec audio_spec;

void AudioCallback(void*  userdata,
	Uint8* stream,
	int    len)
{
	static double audio_time = 0;

	memset(stream, 0, len);//clear
	if (!Game::instance)
		return;

	Game::instance->onAudio((float*)stream, len / sizeof(float), audio_time, audio_spec);
	audio_time += len / (double)audio_spec.freq;
}

void Game::enableAudio()
{
	SDL_memset(&audio_spec, 0, sizeof(audio_spec)); /* or SDL_zero(want) */
	audio_spec.freq = 48000;
	audio_spec.format = AUDIO_F32;
	audio_spec.channels = 1;
	audio_spec.samples = 1024;
	audio_spec.callback = AudioCallback; /* you wrote this function elsewhere. */
	if (SDL_OpenAudio(&audio_spec, &audio_spec) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	SDL_PauseAudio(0);
}

void Game::onAudio(float *buffer, unsigned int len, double time, SDL_AudioSpec& audio_spec)
{
	//fill the audio buffer using our custom retro synth
	synth.generateAudio(buffer, len, audio_spec);
}
