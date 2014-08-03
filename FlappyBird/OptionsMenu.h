#pragma once

#include "Scene.h"
#include "MainMenu.h"
#include "TextSwitchHover.h"

class OptionsMenu: public Scene
{
private:
	MainMenu		&mainMenu;
	TextSwitchHover *interpolatedText, *wireframeText, *texturedText, *useVSyncText,
					*gameSpeedText, *lightColorText, *lightDirectionText;
	TextHover		*backText;
	Cursor			*cursor;
	bool			inited;
public:
	OptionsMenu(MainMenu& mainMenu);
	void init();
	void reshape(int, int);
	bool handleEvent(const SDL_Event& ev);
	void render();
private:
	void initFonts();
	void initCursor();
};