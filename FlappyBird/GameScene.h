#pragma once

#include "Scene.h"
#include "Flappy.h"
#include "Pipe.h"
#include "MainMenu.h"
#include "FlappyFeatherParticleSystem.h"
#include "PipeLevel.h"
#include "Hint.h"
#include "GameOverPanel.h"

enum GameSceneStatus { GAME_SCENE_GET_READY, GAME_SCENE_PLAYING, GAME_SCENE_GAME_OVER, GAME_SCENE_FADING_OUT };

class GameScene: public Scene
{
private:
	// tiempo en el cual el usuario tiene que bancarse ver la pantalla de gameover
	static const float			gameOverCoolDown;

	MainMenu					&mainMenu;
	TTF_Font					*scoreFont;
	TextOptions					scoreTextOptions;
	Text						*scoreText, *getReadyText, *gameOverText;
	FadeInOut					*dieEffect;
	Flappy						*flappy;
	FlappyFeatherParticleSystem	*dieParticleSystem;
	PipeLevel					*level;
	Mix_Chunk					*pointSound;
	Hint						*hint;
	GameOverPanel				*gameOverPanel;

	float						timeSinceGameOver;
	bool						inited;
	int							score;
	GameSceneStatus				state;
public:
	GameScene(MainMenu& mainMenu);
	void init();
	void reshape(int, int);
	bool handleEvent(const SDL_Event& ev);
	void render();
	void resume();
private:
	void initFonts();
};