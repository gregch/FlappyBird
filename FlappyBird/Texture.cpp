#include "Texture.h"
#include <SDL_image.h>
#include <GL/glu.h>
#include <stdexcept>

Texture::Texture():
	textureId(0), w(0), h(0)
{
}

Texture::Texture(int id, GLuint width, GLuint height):
	textureId(id), w(width), h(height)
{
}

Texture::Texture(string path, bool lineal, bool repeat, int mipmaps)
{
	SDL_Surface *surface = IMG_Load(path.c_str());
	if(surface == NULL)
	{
		throw invalid_argument(("Couldn't load texture " + path).c_str());
	}
	initFromSurface(surface, lineal, repeat, mipmaps);
	SDL_FreeSurface(surface);
}


Texture::Texture(SDL_Surface *surface, bool lineal, bool repeat, int mipmaps)
{
	initFromSurface(surface, lineal, repeat, mipmaps);
}

void Texture::initFromSurface(SDL_Surface *surface, bool lineal, bool repeat, int mipmaps)
{
	GLenum texture_format;
	
	// get the number of channels in the SDL surface
	GLint nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4)     // contains an alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
		else
				texture_format = GL_BGRA;
	} else if (nOfColors == 3)     // no alpha channel
	{
		if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
		else
				texture_format = GL_BGR;
	} else {
		throw invalid_argument("Couldn't load sprite: couldn't detect texture format");
	}

	w = surface->w;
	h = surface->h;
 
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &textureId );
 
	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, textureId );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters
 
	// Set the texture's stretching properties
	if(mipmaps == 0){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lineal ? GL_LINEAR : GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lineal ? GL_LINEAR : GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  lineal ?  GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  lineal ? GL_LINEAR : GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, mipmaps, surface->w, surface->h, texture_format, GL_UNSIGNED_BYTE, surface->pixels); 
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP);
 
	
}

GLuint Texture::id() const
{
	return textureId;
}

GLuint Texture::width() const
{
	return w;
}

GLuint Texture::height() const
{
	return h;
}

void Texture::render() const
{
	GLfloat offsetX, offsetY;
	if(w > h)
	{
		offsetX = 1;
		offsetY = h/ (GLfloat) w;
	} else {
		offsetY = 1;
		offsetX = w / (GLfloat)h;
	}
	glBindTexture(*this);
	glBegin(GL_QUADS);
	{
		glColor4f(1,1,1,1);
		glTexCoord2f(0, 0);
		glVertex2f(-offsetX, offsetY);
		glTexCoord2f(0, 1);
		glVertex2f(-offsetX, -offsetY);
		glTexCoord2f(1, 1);
		glVertex2f(offsetX, -offsetY);
		glTexCoord2f(1, 0);
		glVertex2f(offsetX, offsetY);
	}
	glEnd();
}

void Texture::glTexCoord(int x, int y) const
{
	glTexCoord2f(x/(GLfloat)w, y/(GLfloat)h);
}

void glBindTexture(const Texture& t)
{
	glBindTexture( GL_TEXTURE_2D, t.id());
}

void glDeleteTextures(const Texture& t)
{
	GLuint textureId = t.id();
	glDeleteTextures(1, &textureId);
}
