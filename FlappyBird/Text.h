#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Texture.h"
#include "Vector3.h"
#include "SceneObject.h"
#include "Rect.h"


enum TextStatus  { TEXT_SOLID, TEXT_FADING_OUT, TEXT_FADING_IN };

class TextOptions;

class Text: public SceneObject
{
private:
	static const float fadeTime;

	Texture textTexture;
	int offsetX, offsetY;
	Placement placement;
	float animTime;
	TextStatus status;
public:
	Text();
	Text(const TextOptions& options);
	Text(const Text& other);

	void resetText(TTF_Font *font, const TextOptions& options, std::string text);
	static SDL_Surface *getSurface(TTF_Font *font, std::string textAsString, const Vector3& color, int borderSize = 0, const Vector3& borderColor = Vector3::zero);
	bool mouseHover(SDL_Window *win) const; // true if the mouse is in the font bounding rect
	bool mouseClick(SDL_Window *win) const; // true if the mouse if clicked  in the font bounding rect
	void render(Scene &parent);

	void fadeOut();
	void fadeIn();
private:
	Rect getBoundingRect(SDL_Window *win) const;
};

class TextOptions // options for constructing a text object
{
public:
	std::string text;
	std::string fontPath;
	int			fontSize;
	Vector3		color;
	Vector3		borderColor;
	Placement	placement;
	int			offsetX;
	int			offsetY;
	int			borderSize;

	TextOptions();
};