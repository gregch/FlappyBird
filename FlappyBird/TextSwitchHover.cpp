#include "TextSwitchHover.h"
#include "Logger.h"


TextSwitchHover::TextSwitchHover():
	currentIndex(0),
	indexChanged(false),
	clickSound(NULL)
{
}

TextSwitchHover::TextSwitchHover(TextOptions normalStyle, TextOptions hoverStyle, Mix_Chunk *hoverSound, ...):
	currentIndex(0),
	indexChanged(false),
	clickSound(hoverSound)
{
	va_list vl;
	va_start(vl, hoverSound);
	const char *text;
	while((text = va_arg(vl, const char *)) != NULL)
	{
		TextOptions normalOptions = normalStyle;
		normalOptions.text = text;
		TextOptions hoverOptions = hoverStyle;
		hoverOptions.text = text;
		TextHover hoverText = TextHover(normalOptions, hoverOptions, hoverSound);
		texts.push_back(hoverText);
	}
	va_end(vl);
}

void TextSwitchHover::handleEvent(Scene &parent, const SDL_Event& ev)
{
	TextHover& current = texts.at(currentIndex);
	current.handleEvent(parent, ev);

	if(ev.type != SDL_MOUSEBUTTONDOWN)
		return;
	if(current.isClicked(parent))
	{
		currentIndex = (currentIndex + 1) % texts.size();
		indexChanged = true;
		if(clickSound && Mix_PlayChannel(-1, clickSound, 0) == -1) {
			Logger::logSDLError("Unable to play WAV file");
		}
	}
}

void TextSwitchHover::render(Scene &parent)
{
	if(texts.empty())
		return;
	if(currentIndex > texts.size())
	{
		currentIndex = 0;
	}
	TextHover& current = texts.at(currentIndex);
	current.render(parent);
}

int TextSwitchHover::getCurrentIndex() const
{
	return currentIndex;
}

bool TextSwitchHover::wasIndexChanged()
{
	bool res = indexChanged;
	indexChanged = false;
	return res;
}