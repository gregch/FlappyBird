#include "FadeInOut.h"

FadeInOut::FadeInOut(float length, float maxAlpha, const Vector3& color):
	animTime(0),
	color(color),
	maxAlpha(maxAlpha),
	length(length)
{
}

void FadeInOut::render(Scene &parent)
{
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		{
			glLoadIdentity();
			glOrtho(0, 1, 0, 1, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			{
				glLoadIdentity();
				float alpha = maxAlpha * (-4/(length*length)) * animTime * (animTime-length);
				glColor(color, alpha); // 4.5 gets a higher max alpha value
				glBegin(GL_QUADS);
				{
					glVertex2f(0, 0);
					glVertex2f(0, 1);
					glVertex2f(1, 1);
					glVertex2f(1, 0);
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

	animTime += parent.app().getFrameTime();
	if(animTime > 1){
		animTime = 0;
		disable();
	}
}

bool FadeInOut::fadedOut() const
{
	return animTime >= 0.5f;
}
