#pragma once
#include "Scene.h"
#include "Texture.h"
#include "TextHover.h"
#include "Cursor.h"
#include "Floor.h"
#include "Skybox.h"
#include "MainMenuLogo.h"
#include "Camera.h"
#include "Bridge.h"
#include "FadeConstant.h"
#include "FadeInOut.h"
#include "Lighting.h"

#include <SDL_ttf.h>
#include <SDL_mixer.h>

class OptionsMenu;
class CreditsMenu;
class GameScene;
class EditorMenu;

enum MainMenuState { MAIN_MENU_INITIAL, MAIN_MENU_FADING_OUT, MAIN_MENU_GAMEINPROGRESS };

class MainMenu: public Scene
{
private:
	MainMenuState	state;

	OptionsMenu		*optionsMenu;
	CreditsMenu		*creditsMenu;
	EditorMenu		*editorMenu;
	GameScene		*gameScene;
	Lighting		*lighting;
	MainMenuLogo	*logo;
	TextHover		*playText, *resumeText, *optionsText, *quitText,*copyrightText,*editorText;
	Text			*versionText;
	Floor			*floor;
	Skybox			*skybox;
	Camera			*camera;
	Bridge			*bridge;
	Cursor			*cursor;
	FadeConstant	*fadeConstant;
	FadeInOut		*fadeInOut;
	bool			quitClicked;
	Mix_Music		*music;
public:
	MainMenu(App &parent);
	void init();
	void reshape(int w, int h);
	bool handleEvent(const SDL_Event& ev);
	void render();

	Skybox *getSkybox() const;
	Floor *getFloor() const;
	MainMenuLogo *getLogo() const;
	Text *getVersionText() const;
	Camera *getCamera() const;
	Bridge *getBridge() const;
	FadeConstant *getFadeConstant() const;
	FadeInOut *getFadeInOut() const;
	Lighting *getLighting() const;
private:
	void initFonts();
	void initMusic();
	void initCursor();

	void gameStart();
};