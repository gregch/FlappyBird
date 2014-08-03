#include "TextHover.h"
#include "Logger.h"

TextHover::TextHover():
	hoverSound(NULL),
	isHover(false)
{
}

TextHover::TextHover(const Text& normal, const Text &hover, Mix_Chunk *hoverSound):
	normal(normal),
	hover(hover),
	hoverSound(hoverSound),
	isHover(false)
{
}

void TextHover::handleEvent(Scene &parent, const SDL_Event& ev)
{
	if(ev.type != SDL_MOUSEMOTION)
		return;
	bool wasHover = isHover;
	SDL_Window *win = parent.app().getWindow();
	isHover = normal.mouseHover(win);
	if(hoverSound && isHover && !wasHover) {
		if(Mix_PlayChannel(-1, hoverSound, 0) == -1) {
			Logger::logSDLError("Unable to play WAV file");
		}
	}
}

void TextHover::render(Scene& parent)
{
	(isHover ? hover : normal).render(parent);
}

bool TextHover::isClicked(Scene &parent) const
{
	return normal.mouseClick(parent.app().getWindow());
}