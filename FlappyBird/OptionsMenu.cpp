#include "OptionsMenu.h"
#include "Logger.h"

OptionsMenu::OptionsMenu(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	cursor(NULL),
	backText(NULL),
	interpolatedText(NULL), wireframeText(NULL), texturedText(NULL), useVSyncText(NULL),
	gameSpeedText(NULL), lightColorText(NULL), lightDirectionText(NULL)
{
}

void OptionsMenu::init()
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

void OptionsMenu::reshape(int width, int height)
{
	mainMenu.getCamera()->reshape(width, height);
}

void OptionsMenu::initFonts()
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
	addObject(backText);

	// velocidad de juego
	normalOptions.offsetY = hoverOptions.offsetY = -60;
	gameSpeedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Velocidad de Juego: Media", "Velocidad de Juego: Alta", "Velocidad de Juego: Baja", NULL
		);
	addObject(gameSpeedText);

	// interpolado
	normalOptions.offsetY = hoverOptions.offsetY +=  40;
	interpolatedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Renderizado interpolado", "Renderizado liso",  NULL
		);
	addObject(interpolatedText);

	// texturas
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	texturedText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Texturas habilitadas", "Texturas deshabilitadas",  NULL
		);
	addObject(texturedText);

	// texturas
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	useVSyncText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"VSync Activado", "VSync Desactivado",  NULL
		);
	addObject(useVSyncText);

	// wireframe
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	wireframeText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Modo alambre desactivado", "Modo alambre activado",  NULL
		);
	addObject(wireframeText);

	// color de luz
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	lightColorText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Luz blanca", "Luz roja", "Luz psicodelica", "Luz desactivada",  NULL
		);
	addObject(lightColorText);

	// posición de luz
	normalOptions.offsetY = hoverOptions.offsetY += 40;
	lightDirectionText = new TextSwitchHover(
		normalOptions, hoverOptions, menuTick,
		"Luz de frente", "Luz de arriba", "Luz de derecha", "Luz de atras",  NULL
		);
	addObject(lightDirectionText);

	addObject(mainMenu.getVersionText());
	//addObject(mainMenu.getCopyrightText());
}

void OptionsMenu::render()
{
}

bool OptionsMenu::handleEvent(const SDL_Event& ev)
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

	if(texturedText->wasIndexChanged())
	{
		app().getOptions()->setTexturesEnabled(texturedText->getCurrentIndex() == 0);
	}
	if(useVSyncText->wasIndexChanged())
	{
		app().setVSync(useVSyncText->getCurrentIndex() == 0);
	}
	if(gameSpeedText->wasIndexChanged())
	{
		GameSpeed speed = (GameSpeed) gameSpeedText->getCurrentIndex();
		app().getOptions()->setGameSpeed(speed);
	}
	if(wireframeText->wasIndexChanged())
	{
		app().getOptions()->setWireframeEnabled(wireframeText->getCurrentIndex() == 1);
	}
	if(interpolatedText->wasIndexChanged())
	{
		app().getOptions()->setShadingModel(interpolatedText->getCurrentIndex() == 0);
	}
	if(lightDirectionText->wasIndexChanged())
	{
		PresetLightDirection dir = (PresetLightDirection) lightDirectionText->getCurrentIndex();
		mainMenu.getLighting()->setDirection(dir);
	}
	if(lightColorText->wasIndexChanged())
	{
		PresetLightColor color = (PresetLightColor) lightColorText->getCurrentIndex();
		mainMenu.getLighting()->setColor(color);
	}

	return true;
}

void OptionsMenu::initCursor()
{
	SDL_ShowCursor(0);
	cursor = new Cursor("cursor.png", -4, -2);
	addObject(cursor);
}