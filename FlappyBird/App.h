#pragma once

#include <SDL.h>
#include <vector>
#include <string>
#include "Options.h"

class Scene;

class App
{
private:
	Scene *currentScene;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_GLContext glContext;
	float frameTime;
	unsigned long nFrames;

	// options
	Options options;

	// constants
	static const std::string windowTitle;
	static const std::string appIconPath;
public:
	App();
	int run(int argc, char **argv);

	/* scene management */
	void setScene(Scene *scene);
	Scene *getScene();

	/* getters */
	SDL_Window *getWindow() const;
	SDL_Renderer *getRenderer() const;
	SDL_GLContext getGlContext() const;
	float getFrameTime() const;
	unsigned int getRenderedFrames() const;
	Options *getOptions();

	/* manejo de vsync */
	void setVSync(bool enabled);

	/* error checking/handling */
	static void checkOpenGLError(std::string messageIfError);

private:
	int initWindow();
	void initAssimpLog();
	void cleanAssimpLog();
	bool pollEvent();
	void renderSceneAndChild();
	void cleanSceneAndChild();
};