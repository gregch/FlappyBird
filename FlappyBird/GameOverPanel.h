#pragma once

#include "SceneObject.h"
#include "Texture.h"
#include "Rect.h"
#include "Text.h"
#include <SDL_ttf.h>

class GameOverPanel: public SceneObject
{
private:
	static const float coolDownTime;
	static const int translationYLength; // translation length from bottom to top
public:
	static const float animTotalTime;
private:
	Texture		texture;
	float		animTime;
	int			highScore;
	bool		newHighScore;
	int			score;
	TTF_Font	*scoreFont;
public:
	GameOverPanel();
	void render(Scene &parent);
	void enable(int currentScore);
private:
	Rect getBoundingRect(SDL_Window *win);
};
