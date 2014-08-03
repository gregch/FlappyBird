#include "CreditsMenu.h"
#include "Logger.h"

CreditsMenu::CreditsMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	cursor(NULL),
	backText(NULL),
	integrantes(NULL),
	IA(NULL),
	MV(NULL)
{
}
void CreditsMenu::init()
{
	if(!inited)
	{
		addObject(mainMenu.getCamera());
		addObject(mainMenu.getSkybox());
		addObject(mainMenu.getBridge());
		addObject(mainMenu.getFloor());
		addObject(mainMenu.getFadeConstant());
		addObject(mainMenu.getLogo());
		initFonts();
		initCursor();
	}
	inited = true;
}

void CreditsMenu::reshape(int width, int height)
{
	mainMenu.getCamera()->reshape(width, height);
}
void CreditsMenu::initFonts()
{
	TextOptions normalOptions;
	normalOptions.fontPath = "Flappy.ttf";
	normalOptions.fontSize = 32;
	normalOptions.color =  Vector3::one;
	normalOptions.placement = CENTER;
	normalOptions.borderSize = 2;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);

	// volver
	normalOptions.text = "Volver";
	normalOptions.offsetX = 0;
	normalOptions.offsetY = 220;
	Text backTextNormal = Text(normalOptions);	
	// volver -- hover
	TextOptions hoverOptions = normalOptions;
	hoverOptions.fontSize = 36;
	hoverOptions.color =  Vector3::fromRGB(236, 218, 19);
	hoverOptions.text = "Volver";
	Text backTextHover = Text(hoverOptions);
	Mix_Chunk *menuTick = Mix_LoadWAV("MenuTick.wav");
	if(!menuTick) {
		Logger::logSDLError("Mix_LoadWAV");
	}
	backText = new TextHover(backTextNormal, backTextHover, menuTick);

	// version text
	normalOptions.fontSize = 40;
	normalOptions.color =  Vector3::one;
	normalOptions.borderColor = Vector3::fromRGB(40, 100, 30);
	normalOptions.placement = CENTER;
	normalOptions.borderSize = 2;
	normalOptions.offsetX = normalOptions.offsetY = 10;
	normalOptions.text = "Dev Team:";
	integrantes = new Text(normalOptions);
	normalOptions.offsetY += 50;
	normalOptions.text = "Ignacio Avas";
	IA = new Text(normalOptions);
	normalOptions.offsetY += 50;
	normalOptions.text = "Mauricio Vignale";
	MV = new Text(normalOptions);

	addObject(backText);
	addObject(integrantes);
	addObject(IA);
	addObject(MV);

}
void CreditsMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}

bool CreditsMenu::handleEvent(const SDL_Event& ev)
{
	// handle if must go back
	if(backText->isClicked(*this))
	{
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
	return true;
	
}
void CreditsMenu::render()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
}