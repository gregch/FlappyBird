#include "SceneError.h"

#include <sstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <cstring>

SceneError::SceneError(const char *msg):
	logic_error(msg)
{
}

SceneError fromString(const string &s)
{
	char *message = new char[s.length() + 1];
	strncpy(message, s.c_str(), s.length()+1);
	return SceneError(message);
}

SceneError SceneError::fromSDLError(const string &msg)
{
	std::stringstream ss;
	ss << msg << ": " << SDL_GetError();
	return fromString(ss.str());
}

SceneError SceneError::fromGLError(GLenum errorEnum, const string &prefix)
{
	string message;

	switch(errorEnum)
	{
		case GL_INVALID_ENUM:
			message = "OpenGL GL_INVALID_ENUM: An unacceptable value was specified for an enumerated argument";
			break;
		case GL_INVALID_VALUE:
			message = "OpenGL GL_INVALID_VALUE: A numeric argument was out of range";
			break;
		case GL_INVALID_OPERATION:
			message = "OpenGL GL_INVALID_OPERATION: An operation was not allowed in the current state";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "OpenGL GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete";
		case GL_STACK_OVERFLOW:
			message = "OpenGL GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow"; 
			break;
		case GL_STACK_UNDERFLOW:
			message = "OpenGL GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow";
			break;
		case GL_OUT_OF_MEMORY:
			message = "OpenGL GL_OUT_OF_MEMORY: There is not enough memory left to execute the command";
			break;
		case GL_NO_ERROR:
			message = "OpenGL GL_NO_ERROR: No error recorded";
			break;
		default:
			message = "OpenGL Unknown error";
			break;
	}
	std::stringstream ss;
	ss << prefix << ": " << message;
	return fromString(ss.str());
}
