#pragma once

#include "SceneObject.h"
#include "Text.h"
#include <SDL_mixer.h>

class TextHover: public SceneObject
{
private:
	Text		normal, hover;
	bool		isHover;
	bool		clicked;
	Mix_Chunk   *hoverSound;
public:
	TextHover();
	TextHover(const Text& normal, const Text &hover, Mix_Chunk *hoverSound);
	bool isClicked(Scene &parent) const;

	void handleEvent(Scene &parent, const SDL_Event& ev);
	void render(Scene &parent);

};