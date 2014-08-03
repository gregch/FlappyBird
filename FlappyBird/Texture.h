#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <string>

using namespace std;

class Texture
{
private:
	GLuint textureId;
	GLuint w;
	GLuint h;
public:
	Texture(); // dummy constructor
	// creates a texture from an OpenGL texture id, and dimensions
	Texture(int id, GLuint width, GLuint height); 
	// load a texture from a path (with lineal filtering) and mipmaps
	Texture(string path, bool lineal = true, bool repeat = false, int mipmaps = 0); 
	// load a texture from a surface (with lineal filtering)
	Texture(SDL_Surface *surface, bool lineal = true, bool repeat = false, int mipmaps = 0); 

	GLuint id() const; // texture OpenGL id
	GLuint width() const; // texture width
	GLuint height() const; // texture height
	void glTexCoord(int x, int y) const; // shortcut for glTexCoord2f(x/(GLfloat) width() , y/(GLfloat) height() );
	
	void render() const; // renders the texture entirely
private:
	void initFromSurface(SDL_Surface *surface, bool lineal, bool repeat, int mipmaps);
};

void glBindTexture(const Texture& t);
void glDeleteTextures(const Texture& t);