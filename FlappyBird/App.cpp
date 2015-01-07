#pragma warning( disable : 4505 )

#include "App.h"
#include "Logger.h"
#include "Scene.h"
#include "SceneError.h"
#include "SceneObject.h"
#include <assimp/DefaultLogger.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <sstream>
#include <GL/freeglut.h>

using namespace std;

const std::string App::windowTitle = "FlappyBird";
const std::string App::appIconPath = "icon.png";

App::App():
	win(NULL),
	ren(NULL),
	glContext(0),
	frameTime(0.0f),
	nFrames(0)
{
}

void App::setScene(Scene *scene)
{
	currentScene = scene;
}

Scene *App::getScene()
{
	return currentScene;
}

void App::initAssimpLog()
{
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
	Assimp::DefaultLogger::create("assimp_log.txt",severity, aiDefaultLogStream_FILE);
}

void App::cleanAssimpLog()
{
	// Kill it after the work is done
	Assimp::DefaultLogger::kill();
}

int App::initWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		Logger::logSDLError("SDL_Init Error");
		return 1;
	}

	// Settings for antialiasing
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    glEnable(GL_MULTISAMPLE);

	/* Turn on double buffering with a 24bit Z buffer.
     * You may need to change this to 16 or 32 for your system */
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Creat window, renderer and GL context
	win = SDL_CreateWindow(
		windowTitle.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE 
	);
	if (win == NULL){
		Logger::logSDLError("SDL_CreateWindow Error");
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL){
		Logger::logSDLError("SDL_CreateRenderer Error");
		return 1;
	}
	glContext = SDL_GL_CreateContext(win);
	if(glContext == NULL)
	{
		Logger::logSDLError("SDL_GL_CreateContext Error");
		return 1;
	}

	// load app icon
	SDL_Surface *image;
	image = IMG_Load(appIconPath.c_str());
	if(!image) {
		stringstream ss;
		ss << "Loading App icon Error: IMG_Load: " << IMG_GetError();
		Logger::log(ss.str());
	} else {
		SDL_SetWindowIcon(win, image); 
		SDL_FreeSurface(image);
	}

	setVSync(true);
	
	if(TTF_Init() == -1) {
		Logger::logSDLError("TTF_Init Error");
		return 1;
	}

	return 0;
}

bool App::pollEvent()
{
	while(SDL_PollEvent(NULL) > 0)
	{
		SDL_Event e;
		SDL_PollEvent(&e);

		std::vector<SceneObject *> objects = currentScene->getObjects();
		for(std::vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if((*it)->isEnabled())
				(*it)->handleEvent(*currentScene, e);
		}

		switch (e.type)  {
			case SDL_WINDOWEVENT:
				switch (e.window.event)  {   
					case SDL_WINDOWEVENT_SIZE_CHANGED: 
						currentScene->reshape( e.window.data1, e.window.data2);
						return true;
					default:
						return currentScene->handleEvent(e);
				}
			case SDL_QUIT:
				return false;
			default:
				return currentScene->handleEvent(e);
		}
	}
	return true;
}

void App::cleanSceneAndChild()
{
	std::vector<SceneObject *> objects = currentScene->getObjects();
	for(std::vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		(*it)->clean(*currentScene);
		checkOpenGLError("Scene child clean");
	}
	currentScene->clean();
	checkOpenGLError("Scene clean");
	cleanAssimpLog();
}

void App::renderSceneAndChild()
{
	std::vector<SceneObject *> objects = currentScene->getObjects();
	++nFrames;
	Uint32 start = SDL_GetTicks();
	currentScene->render();
	checkOpenGLError("Scene render");
	for(std::vector<SceneObject *>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if((*it)->isEnabled())
		{
			(*it)->render(*currentScene);
			checkOpenGLError("Scene child render");
		}
	}
	SDL_GL_SwapWindow(win);
	frameTime = (SDL_GetTicks() - start) / 1000.0f;
}

int App::run(int argc, char **argv)
{
	int retcode;
	glutInit(&argc, argv);
	
	retcode = initWindow();
	if(retcode != 0)
		return retcode;
	initAssimpLog();
		
	try {
		currentScene->init();
		checkOpenGLError("Scene init");
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		currentScene->reshape(w, h);
		checkOpenGLError("Initial Scene reshape");
	
		while (pollEvent()){
			unsigned int windowFlags = SDL_GetWindowFlags(win);
			if(windowFlags & (SDL_WINDOW_HIDDEN | SDL_WINDOW_MINIMIZED)){
				continue;
			}
			renderSceneAndChild();
		}

		cleanSceneAndChild();
		retcode = 0;
	} catch(exception& ex)
	{
		Logger::logError(ex.what());
		retcode = 1;
	}

	SDL_DestroyWindow(win);
	SDL_Quit();
	return retcode;
}

SDL_Window *App::getWindow() const
{
	return win;
}

SDL_Renderer *App::getRenderer() const
{
	return ren;
}

SDL_GLContext App::getGlContext() const
{
	return glContext;
}

float App::getFrameTime() const
{
	return frameTime;
}

unsigned int App::getRenderedFrames() const
{
	return nFrames;
}

Options *App::getOptions()
{
	return &options;
}

void App::setVSync(bool enabled)
{
	//Use Vsync
	if(SDL_GL_SetSwapInterval(enabled ? 1: 0) < 0 ){
		Logger::logSDLError("SDL_GL_SetSwapInterval Error");
	}
}

void App::checkOpenGLError(const string message)
{
	GLenum error = glGetError();
	if(error != GL_NO_ERROR)
		throw SceneError::fromGLError(error, message);
}
