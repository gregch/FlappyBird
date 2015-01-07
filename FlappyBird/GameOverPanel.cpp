#include "GameOverPanel.h"
#include "Vector3.h"
#include "SceneError.h"
#include <sstream>
#include <stdexcept>
#include <SDL_image.h>

const float GameOverPanel::coolDownTime = 1.0f;
const int GameOverPanel::translationYLength = 100;
const float GameOverPanel::animTotalTime = 0.5f + GameOverPanel::coolDownTime;


GameOverPanel::GameOverPanel():
	texture(),
	animTime(0),
	highScore(0),
	score(0),
	newHighScore(false)
{
	scoreFont = TTF_OpenFont("Flappy.ttf", 25);
	if (scoreFont == NULL){
			throw SceneError::fromSDLError("Couldn't load font: TTF_OpenFont");
	}
}

void GameOverPanel::render(Scene& parent)
{
	SDL_Window * win = parent.app().getWindow();
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	Rect rect = getBoundingRect(win);
	
	glPushAttrib(GL_ENABLE_BIT);
	{
		glMatrixMode(GL_PROJECTION);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, width, 0, height, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			{
				glLoadIdentity();
				glBindTexture(texture);
				
				float alpha = 0;
				float translateY = 0;
				if(animTime > coolDownTime)
				{
					float diff = (animTime - coolDownTime) / (animTotalTime - coolDownTime);
					if(diff > 1)
						diff = 1;
					alpha = diff;
					translateY = translationYLength * (1 - diff) * (1 - diff);
				}
				glTranslate(Vector3::down * translateY);
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

	if(animTime < animTotalTime)
	{
		animTime += parent.app().getFrameTime();
	}
}

static std::string intToString(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

void GameOverPanel::enable(int currentScore)
{
	SceneObject::enable();
	animTime = 0;
	score = currentScore;
	newHighScore = currentScore > highScore;
	if(newHighScore)
	{
		highScore = currentScore;
	}

	// copy texts to texture
	SDL_Surface *panelSurface = IMG_Load("panel.png");
	if(panelSurface == NULL)
	{
		throw std::logic_error("Couldn't load texture panel.png");
	}
	SDL_Surface *scoreSurface = Text::getSurface(scoreFont, intToString(score), Vector3::one, 2, Vector3::zero);
	SDL_Surface *highScoreSurface = Text::getSurface(scoreFont,  intToString(highScore), Vector3::one, 2, Vector3::zero);

	SDL_Rect scoreDstRect = {
		panelSurface->w - scoreSurface->w - 25,
		panelSurface->h / 2 - 50,
		scoreSurface->w,
		scoreSurface->h
	};
	SDL_Rect highScoreDstRect = {
		panelSurface->w - highScoreSurface->w - 25,
		panelSurface->h / 2 + 30,
		highScoreSurface->w,
		highScoreSurface->h
	};
	
	if(SDL_BlitSurface(scoreSurface, NULL, panelSurface, &scoreDstRect) < 0)
	{
		throw SceneError::fromSDLError("Couldn't copy surface");
	}
	if(SDL_BlitSurface(highScoreSurface, NULL, panelSurface, &highScoreDstRect) < 0)
	{
		throw SceneError::fromSDLError("Couldn't copy surface");
	}

	if(newHighScore)
	{
		SDL_Surface *newSurface = IMG_Load("new.png");
		if(newSurface == NULL)
		{
			throw std::logic_error("Couldn't load texture new.png");
		}
		SDL_Rect newDstRect = {
			panelSurface->w / 2 - 35,
			panelSurface->h / 2 - 5,
			highScoreSurface->w,
			highScoreSurface->h
		};
		if(SDL_BlitSurface(newSurface, NULL, panelSurface, &newDstRect) < 0)
		{
			throw SceneError::fromSDLError("Couldn't copy surface");
		}
		SDL_FreeSurface(newSurface);
	}

	SDL_FreeSurface(scoreSurface);
	SDL_FreeSurface(highScoreSurface);

	glDeleteTextures(texture);
	texture = Texture(panelSurface);
}


Rect GameOverPanel::getBoundingRect(SDL_Window *win)
{
	return Rect::createBoundingRect(win, CENTER, 0, 50, texture.width(), texture.height());
}
