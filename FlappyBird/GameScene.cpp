#include "Logger.h"
#include "GameScene.h"
#include "SceneError.h"
#include <sstream>

const float GameScene::gameOverCoolDown = GameOverPanel::animTotalTime + 0.2f;

GameScene::GameScene(MainMenu& mainMenu):
	Scene(mainMenu.app()),
	mainMenu(mainMenu),
	inited(false),
	flappy(NULL),
	scoreText(NULL),
	state(GAME_SCENE_GET_READY),
	score(0),
	timeSinceGameOver(0)
{
}
void GameScene::init()
{
	if(!inited)
	{
		addObject(mainMenu.getCamera());
		addObject(mainMenu.getSkybox());
		addObject(mainMenu.getBridge());
		addObject(mainMenu.getFloor());
		flappy = new Flappy(mainMenu.getBridge());
		addObject(flappy);
		level = new PipeLevel(*mainMenu.getBridge());
		addObject(level);
		level->disable();
		dieParticleSystem = new FlappyFeatherParticleSystem(*flappy);
		addObject(dieParticleSystem);
		dieParticleSystem->disable();
		hint = new Hint();
		addObject(hint);
		gameOverPanel = new GameOverPanel();
		addObject(gameOverPanel);
		gameOverPanel->disable();
		initFonts();
		addObject(mainMenu.getFadeInOut());
		dieEffect = new FadeInOut(0.25f, 0.70f, Vector3::one);
		addObject(dieEffect);
		dieEffect->disable();
		pointSound = Mix_LoadWAV("point.wav");
	}
	inited = true;
}


void GameScene::reshape(int width, int height)
{
	mainMenu.getCamera()->reshape(width, height);
}
void GameScene::initFonts()
{
	TextOptions options;
	options.fontPath = "Flappy.ttf";
	options.fontSize = 72;
	options.color =  Vector3::one;
	options.placement = TOP_CENTER;
	options.borderSize = 3;
	options.borderColor = Vector3::fromRGB(40, 100, 30);
	options.offsetY = 10;
	options.text = "0";
	scoreText = new Text(options);
	addObject(scoreText);
	scoreTextOptions = options;

	scoreFont = TTF_OpenFont(options.fontPath.c_str(), options.fontSize);
	if (scoreFont == NULL){
			throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
	}

	options.color = Vector3::fromRGB(236, 218, 19);
	options.fontSize = 70;
	options.placement = CENTER;
	options.offsetY = -140;
	options.text = "Preparate";
	getReadyText = new Text(options);
	addObject(getReadyText);
	options.text = "Game Over";
	gameOverText = new Text(options);
	addObject(gameOverText);
	gameOverText->disable();
}


bool GameScene::handleEvent(const SDL_Event& ev)
{

	if(state != GAME_SCENE_FADING_OUT && ev.type == SDL_KEYDOWN)
	{
		switch(ev.key.keysym.scancode){
		case SDL_SCANCODE_P:
		case SDL_SCANCODE_ESCAPE:
			app().setScene(&mainMenu);
			mainMenu.getBridge()->stop();
			mainMenu.getCamera()->disableMove();
			return true;
		case SDL_SCANCODE_Q:
			return false;
		default:
			return true;
		}
	}

	if(state == GAME_SCENE_GAME_OVER &&
		timeSinceGameOver > gameOverCoolDown &&
		(
			ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT ||
			ev.type == SDL_KEYDOWN
		)
	)
	{
		state = GAME_SCENE_FADING_OUT;
		mainMenu.getFadeInOut()->enable();
	}

	return true;
}
void GameScene::render()
{
	switch(state)
	{
	case GAME_SCENE_GET_READY:
		if(flappy->heJumpedFirstTime())
		{
			state = GAME_SCENE_PLAYING;
			getReadyText->fadeOut();
			hint->fadeOut();
			level->reset();
			level->enable();
		}
		break;
	case GAME_SCENE_PLAYING:
		if(flappy->isDead() || level->testCollition(*flappy)){
			flappy->kill();
			state = GAME_SCENE_GAME_OVER;
			timeSinceGameOver = 0;
			level->stop();
			mainMenu.getBridge()->stop();
			dieEffect->enable();
			dieParticleSystem->enable();
			dieParticleSystem->reset();
			gameOverText->enable();
			gameOverText->fadeIn();
			gameOverPanel->enable(score);
			mainMenu.getCamera()->tremble(0.25f);
		} else if(level->wasPointAwarded()) {
			Mix_PlayChannel(-1, pointSound, 0);
			++score;
			std::stringstream ss;
			ss << score;
			scoreText->resetText(scoreFont, scoreTextOptions, ss.str());
		}
		break;
	case GAME_SCENE_FADING_OUT:
		if(mainMenu.getFadeInOut()->fadedOut())
		{
			state = GAME_SCENE_GET_READY;
			level->disable();
			gameOverText->disable();
			gameOverPanel->disable();
			dieParticleSystem->disable();
			getReadyText->enable();
			getReadyText->fadeIn();
			hint->enable();
			mainMenu.getBridge()->reset();
			mainMenu.getBridge()->resume();
			mainMenu.getCamera()->tremble(0);
			flappy->respawn();
			score = 0;
			scoreText->resetText(scoreFont, scoreTextOptions, "0");
		}
		break;
	case GAME_SCENE_GAME_OVER:
		timeSinceGameOver += app().getFrameTime();
		break;
	} 
}

void GameScene::resume()
{
	mainMenu.getCamera()->enableMove();
	if(state == GAME_SCENE_PLAYING || state == GAME_SCENE_GET_READY)
	{
		mainMenu.getBridge()->resume();
	}
}