#include "MainMenu.h"
#include "Vector3.h"
#include "SceneError.h"
#include "SceneObject.h"
#include "Logger.h"
#include "OptionsMenu.h"
#include "EditorMenu.h"
#include "GameScene.h"
#include "CreditsMenu.h"
#include <GL/glu.h>

MainMenu::MainMenu(App& parent):
	Scene(parent),
	state(MAIN_MENU_INITIAL),
	music(NULL),
	quitClicked(false),
	playText(NULL), optionsText(NULL), quitText(NULL),
	versionText(NULL), copyrightText(NULL),editorText(NULL),
	floor(NULL), skybox(NULL),
	fadeConstant(NULL), fadeInOut(NULL)
{
	optionsMenu = new OptionsMenu(*this);
	creditsMenu = new CreditsMenu(*this);
	gameScene	= new GameScene(*this);
	editorMenu  = new EditorMenu(*this);
}

void MainMenu::init()
{	
	glEnable(GL_DEPTH_TEST);
	// blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(Vector3::fromRGB(86, 157, 223));
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	// fog
	glEnable (GL_FOG);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogi (GL_FOG_MODE, GL_EXP2);
	glFogf (GL_FOG_DENSITY, 0.001f);
	glFog (GL_FOG_COLOR, Vector3::fromRGB(86, 157, 223));

	
	lighting = new Lighting();
	camera = new Camera(0, 1.51f, 180.0f, Vector3::up * 130.0f);
	addObject(camera);
	skybox = new Skybox(camera);
	addObject(skybox);
	bridge = new Bridge();
	addObject(bridge);
	floor = new Floor();
	addObject(floor);
	fadeConstant = new FadeConstant();
	addObject(fadeConstant);
	fadeConstant->disable();
	logo = new MainMenuLogo("logo.png");
	addObject(logo);
	initMusic();
	initFonts();
	initCursor();
	fadeInOut = new FadeInOut();
	fadeInOut->disable();
	addObject(fadeInOut);
}

void MainMenu::reshape(int width, int height)
{
	camera->reshape(width, height);
}

bool MainMenu::handleEvent(const SDL_Event& ev)
{
	if(state == MAIN_MENU_FADING_OUT)
		return !quitClicked; // no se permiten cosas raras en la transición

	// ver si el init se inicia muchas veces! (antes esto estaba en render)
	quitClicked = quitClicked || quitText->isClicked(*this);
	if(optionsText->isClicked(*this))
	{
		optionsMenu->init();
		app().setScene(optionsMenu);
	}else if(copyrightText->isClicked(*this)){
		creditsMenu->init();
		app().setScene(creditsMenu);
	}else if(editorText->isClicked(*this)){
		editorMenu->init();
		app().setScene(editorMenu);
	} 
	else if(playText->isClicked(*this)){
		gameStart();
	}

	if(ev.type == SDL_KEYDOWN &&
			(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	)
	{
		return false;
	}


	return !quitClicked;
}

void MainMenu::render()
{
	if(state == MAIN_MENU_FADING_OUT && fadeInOut->fadedOut())
	{
		state = MAIN_MENU_GAMEINPROGRESS;

		playText->disable();
		resumeText->enable();
		fadeConstant->enable();
		app().setScene(gameScene);
		gameScene->resume();
	}
}

void MainMenu::initFonts()
{
	TextOptions options;
	options.fontPath = "Flappy.ttf";
	options.fontSize = 42;
	options.color =  Vector3::one;
	options.placement = CENTER;
	options.borderSize = 2;
	options.borderColor = Vector3::fromRGB(40, 100, 30);

	// jugar
	options.text = "Jugar";
	options.offsetX = options.offsetY = 0;
	Text playTextNormal = Text(options);
	// reanudar
	options.text = "Reanudar";
	Text resumeTextNormal = Text(options);
	// opciones
	options.text = "Opciones";
	options.offsetY = 50;
	Text optionsTextNormal = Text(options);
	// editor
	options.text = "Editor de Niveles";
	options.offsetY = 100;
	Text editorTextNormal = Text(options);
	// salir
	options.text = "Salir";
	options.offsetY = 150;
	Text quitTextNormal = Text(options);
	

	options.placement = CENTER;
	// jugar -- hover
	options.fontSize = 46;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.offsetY = 0;
	options.text = "Jugar";
	Text playTextHover = Text(options);
	options.text = "Reanudar";
	Text resumeTextHover = Text(options);
	// opciones -- hover
	options.text = "Opciones";
	options.offsetY = 50;
	Text optionsTextHover = Text(options);
	// editor -- hover
	options.text = "Editor de Niveles";
	options.offsetY = 100;
	Text editorTextHover = Text(options);
	// salir -- hover
	options.text = "Salir";
	options.offsetY = 150;
	Text quitTextHover = Text(options);

	

	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}

	playText = new TextHover(playTextNormal, playTextHover, menuTick);
	resumeText = new TextHover(resumeTextNormal, resumeTextHover, menuTick);
	optionsText = new TextHover(optionsTextNormal, optionsTextHover, menuTick);
	editorText = new TextHover(editorTextNormal, editorTextHover, menuTick);
	quitText = new TextHover(quitTextNormal, quitTextHover, menuTick);
	

	addObject(playText);
	addObject(resumeText);
	resumeText->disable();
	addObject(optionsText);
	addObject(editorText);
	addObject(quitText);

	// version text
	options.fontSize = 20;
	options.color =  Vector3::one;
	options.placement = BOTTOM_LEFT;
	options.borderSize = 1;
	options.offsetX = options.offsetY = 10;
	options.text = "v1.0.0";
	versionText = new Text(options);

	// copyright text
	options.placement = BOTTOM_RIGHT;
	options.text = "2014 CompGraf";
	Text copyrightTextNormal =  Text(options);

	// copyright text -- hover
	options.fontSize = 22;
	options.color =  Vector3::fromRGB(236, 218, 19);
	options.text = "2014 CompGraf";
	Text copyrightTextHover = Text(options);

	copyrightText = new TextHover(copyrightTextNormal, copyrightTextHover, menuTick);

	addObject(versionText);
	addObject(copyrightText);
}

void MainMenu::initMusic()
{
    int flags = MIX_INIT_MP3 | MIX_INIT_OGG;
	int initted = Mix_Init(flags);

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) != 0) {
		Logger::logSDLError("Mix_OpenAudio: Unable to initialize audio");
		return;
	}

    if((initted & MIX_INIT_MP3) != 0){
        music = Mix_LoadMUS("MenuMusic.mp3");
    } else {
        music = Mix_LoadMUS("MenuMusic.ogg");
    }
	if(!music) {
		Logger::logSDLError("Mix_LoadMUS error");
		return;
	}
	if(Mix_PlayMusic(music, -1) == -1) {
		Logger::logSDLError("Mix_PlayMusic");
		return;
	}
}

void MainMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}

Floor *MainMenu::getFloor() const
{
	return floor;
}

MainMenuLogo *MainMenu::getLogo() const 
{
	return logo;
}

Camera *MainMenu::getCamera() const
{
	return camera;
}

Bridge *MainMenu::getBridge() const 
{
	return bridge;
}

FadeConstant *MainMenu::getFadeConstant() const 
{
	return fadeConstant;
}

FadeInOut *MainMenu::getFadeInOut() const 
{
	return fadeInOut;
}

Lighting *MainMenu::getLighting() const 
{
	return lighting;
}

Skybox *MainMenu::getSkybox() const
{
	return skybox;
}

Text *MainMenu::getVersionText() const
{
	return versionText;
}

void MainMenu::gameStart()
{
	if(state == MAIN_MENU_INITIAL)
	{
		gameScene->init();
		state = MAIN_MENU_FADING_OUT;
		fadeInOut->enable();
		camera->enableMove();
	} else if(state == MAIN_MENU_GAMEINPROGRESS) {
		gameScene->init();
		gameScene->resume();
		app().setScene(gameScene);
	}
}
