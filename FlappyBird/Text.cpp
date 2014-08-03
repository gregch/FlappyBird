#include "Text.h"
#include "SceneError.h"
#include <GL/GLU.h>

const float Text::fadeTime = 0.75f;

Text::Text():
	offsetX(0),
	offsetY(0),
	placement(TOP_LEFT),
	animTime(0),
	status(TEXT_SOLID)
{
}

Text::Text(const Text& other):
	textTexture(other.textTexture),
	offsetX(other.offsetX),
	offsetY(other.offsetY),
	placement(other.placement),
	animTime(0),
	status(TEXT_SOLID)
{
}

Text::Text(const TextOptions& options):
	offsetX(options.offsetX),
	offsetY(options.offsetY),
	placement(options.placement),
	animTime(0),
	status(TEXT_SOLID)
{
	TTF_Font *font = TTF_OpenFont(options.fontPath.c_str(), options.fontSize);
	if (font == NULL){
		throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
    }
	resetText(font, options, options.text);

	TTF_CloseFont(font);
}

void Text::resetText(TTF_Font *font, const TextOptions& options, std::string textAsString)
{
	SDL_Surface *textSurface = getSurface(font, textAsString, options.color, options.borderSize, options.borderColor);
	textTexture = Texture(textSurface, false);

	SDL_FreeSurface(textSurface);
}

SDL_Surface *Text::getSurface(TTF_Font *font, std::string textAsString, const Vector3& color, int borderSize, const Vector3& borderColor)
{
	const char *text = textAsString.c_str();
	TTF_SetFontOutline(font, 0);
	SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, color.toSDLColor());
	if(borderSize == 0)
	{
		return textSurface;
	} else {
		TTF_SetFontOutline(font, borderSize);
		SDL_Surface *borderSurface = TTF_RenderText_Blended(font, text, borderColor.toSDLColor());

		SDL_Rect destinationRect = { borderSize, borderSize, textSurface->w, textSurface->h };
		if(SDL_BlitSurface(textSurface, NULL, borderSurface, &destinationRect) < 0){
			throw SceneError::fromSDLError("Couldn't load font: SDL_BlitSurface");
		}
		SDL_FreeSurface(textSurface);
		return borderSurface;
	} 
}

void Text::render(Scene &parent)
{
	SDL_Window * win = parent.app().getWindow();
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	Rect rect = getBoundingRect(win);
	
	glPushAttrib(GL_ENABLE_BIT);
	{
		glMatrixMode(GL_PROJECTION);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, width, 0, height, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			{
				glLoadIdentity();
				glBindTexture(textTexture);

				float alpha;
				switch(status)
				{
				case TEXT_FADING_OUT:
					alpha = 1 - animTime / fadeTime;
					break;
				case TEXT_FADING_IN:
					alpha = animTime / fadeTime;
					break;
				default:
					alpha = 1;
				}
			

				glColor(Vector3::one, alpha);
				glBegin(GL_QUADS);
				{
					glTexCoord2f(0, 0);
					glVertex2f(rect.left(), height - rect.top());
					glTexCoord2f(0, 1);
					glVertex2f(rect.left(), height - rect.bottom());
					glTexCoord2f(1, 1);
					glVertex2f(rect.right(), height - rect.bottom());
					glTexCoord2f(1, 0);
					glVertex2f(rect.right(), height - rect.top());
				}
				glEnd();
			}
			glPopMatrix();
		}
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}
	glPopAttrib();

	if(status != TEXT_SOLID)
	{
		animTime += parent.app().getFrameTime();
	}
	if(animTime > 1)
	{
		if(status == TEXT_FADING_OUT)
		{
			disable();
		}
		status = TEXT_SOLID;
		animTime = 0;
	}
}


bool Text::mouseHover(SDL_Window *win) const 
{
	if(SDL_GetMouseFocus() != win)
	{
		return false;
	}
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	return getBoundingRect(win).contains(mouseX, mouseY);
}

bool Text::mouseClick(SDL_Window *win) const 
{
	if(SDL_GetMouseFocus() != win)
	{
		return false;
	}
	int mouseX, mouseY;
	int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	if((mouseState & (SDL_BUTTON(1) | SDL_BUTTON(2))) == 0)
		return false;
	return getBoundingRect(win).contains(mouseX, mouseY);
}

void Text::fadeOut()
{
	status = TEXT_FADING_OUT;
}

void Text::fadeIn()
{
	status = TEXT_FADING_IN;
}

Rect Text::getBoundingRect(SDL_Window *win) const
{
	return Rect::createBoundingRect(win, placement, offsetX, offsetY, textTexture.width(), textTexture.height());
}


TextOptions::TextOptions():
	text(""),
	fontPath(""),
	fontSize(-1),
	color(Vector3::one),
	borderColor(Vector3::zero),
	placement(TOP_LEFT),
	offsetX(0),
	offsetY(0),
	borderSize(0)
{
}