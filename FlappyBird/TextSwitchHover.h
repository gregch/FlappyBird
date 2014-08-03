#pragma once

#include "TextHover.h"

class TextSwitchHover : public SceneObject
{
private:
	std::vector<TextHover> texts;
	Mix_Chunk *clickSound;
	int currentIndex;
	bool indexChanged;
public: 
	TextSwitchHover();
	// @param normalStyle	the normal text status style
	// @param hoverStyle	the options for the hovered text status
	// @param sound			the sound used when text is hover or clicked
	// @param ...			a variable length (va_list) of strings (char *) for the different options
	//						that are switched when the text is clicked. The list must be ended with a
	//						NULL pointer
	TextSwitchHover(TextOptions normalStyle, TextOptions hoverStyle, Mix_Chunk *sound, ...);

	int getCurrentIndex() const;
	bool wasIndexChanged(); // returns true if index changed since the last time it was called

	void handleEvent(Scene &parent, const SDL_Event& ev);
	void render(Scene &parent);
};