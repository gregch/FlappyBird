#pragma once

#include <exception>
#include <string>
#include <SDL_opengl.h>
#include <stdexcept>

using namespace std;

class SceneError: public logic_error
{
public:
	SceneError(const char *msg);
	static SceneError fromSDLError(const string &msg);
	static SceneError fromGLError(GLenum glGetErrorRes, const string &msg);
};
