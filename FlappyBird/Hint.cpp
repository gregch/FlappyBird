#include "Hint.h"
#include "Vector3.h"

Hint::Hint():
	texture("hint.png", true),
	animTime(0),
	fadingOut(false)
{
}

void Hint::render(Scene& parent)
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
				glColor(Vector3::one, 1-animTime); 
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

	if(fadingOut)
	{
		animTime += parent.app().getFrameTime();
		if(animTime > 1){
			animTime = 0;
			fadingOut = false;
			disable();
		}
	}
}

void Hint::fadeOut()
{
	fadingOut = true;
}

Rect Hint::getBoundingRect(SDL_Window *win)
{
	return Rect::createBoundingRect(win, CENTER, 0, 50, 300, 300);
}
