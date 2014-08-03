#include "EditorMenu.h"
#include "Logger.h"
EditorMenu::EditorMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	cursor(NULL),
	backText(NULL),
	tubo(NULL)
{
}
void EditorMenu::init()
{
	if(!inited)
	{
		addObject(mainMenu.getCamera());
		addObject(mainMenu.getSkybox());
		addObject(mainMenu.getBridge());
		mainMenu.getBridge()->stop();
		addObject(mainMenu.getFloor());
		addObject(mainMenu.getFadeConstant());		
		
		tubo =  new PipeEditor(*mainMenu.getBridge(),100,60);
		addObject(tubo);

		initFonts();
		initCursor();
	}
	inited = true;
}

void EditorMenu::reshape(int width, int height)
{
	mainMenu.getCamera()->reshape(width, height);
}
void EditorMenu::initFonts()
{
	TextOptions normalOptions;
	normalOptions.fontPath = "Flappy.ttf";
	normalOptions.fontSize = 32;
	normalOptions.color =  Vector3::one;
	normalOptions.placement = CENTER;
	normalOptions.borderSize = 2;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);

	// volver
	normalOptions.text = "Guardar";
	normalOptions.offsetX = 0;
	normalOptions.offsetY = 220;
	Text backTextNormal = Text(normalOptions);	
	// volver -- hover
	TextOptions hoverOptions = normalOptions;
	hoverOptions.fontSize = 36;
	hoverOptions.color =  Vector3::fromRGB(236, 218, 19);
	hoverOptions.text = "Guardar";
	Text backTextHover = Text(hoverOptions);
	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}
	backText = new TextHover(backTextNormal, backTextHover, menuTick);


	addObject(backText);

}
void EditorMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}

bool EditorMenu::handleEvent(const SDL_Event& ev)
{
	// handle if must go back
	if(backText->isClicked(*this))
	{
		tubo->writeLevelToXml();
		app().setScene(&mainMenu);
		return true;
	}
	else if(ev.type == SDL_KEYDOWN &&
			(ev.key.keysym.scancode == SDL_SCANCODE_Q  || ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	)
	{
		app().setScene(&mainMenu);
		return true;
	}
	else if(ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT){
		tubo->pushCurrentApertureHeight();
		mainMenu.getBridge()->move(35.0f);
		return true;
	}
	return true;
	
}
void EditorMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}