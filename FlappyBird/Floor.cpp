#include "Floor.h"
#include "Vector3.h"

Floor::Floor():
	texture("green-pattern.jpg", false, true, 3),
	animTime(0)
{
	texturedDisplayList = glGenLists(3);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList();
	solidDisplayList = texturedDisplayList+1;
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList();
	wireframeDisplayList = texturedDisplayList+2;
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList();
}

void Floor::render(RenderMode mode)
{
	glPushAttrib(GL_ENABLE_BIT);
	{

		if(mode == TEXTURED_RENDER)
		{
			glEnable(GL_TEXTURE_2D);
			glBindTexture(texture);
		} else {
			glDisable(GL_TEXTURE_2D);
		}
		if(mode == WIREFRAME_RENDER)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		const float floorSize = 4000;

		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER)
			{
				glColor(Vector3::one);
			} else {
				glColor(Vector3::fromRGB(96, 151, 50));
			}
			
			glTexCoord2f (0, floorSize/10); glVertex3f   (-floorSize/2, 0,  floorSize/2);
			glTexCoord2f (0, 0);  glVertex3f   (-floorSize/2, 0, -floorSize/2);
			glTexCoord2f (floorSize/10, 0); glVertex3f   ( floorSize/2, 0, -floorSize/2);
			glTexCoord2f (floorSize/10, floorSize/10); glVertex3f   ( floorSize/2, 0,  floorSize/2);
		}
		glEnd();
	}
	glPopAttrib();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Floor::render(Scene &parent)
{	
	if(parent.app().getOptions()->renderMode() != WIREFRAME_RENDER)
	{
		switch(parent.app().getOptions()->renderMode())
		{
		case TEXTURED_RENDER:
			glMatrixMode(GL_TEXTURE);
			glPushMatrix();
			glLoadIdentity();
			glTranslatef(animTime, 0, 0);
			glMatrixMode(GL_MODELVIEW);
			glCallList(texturedDisplayList);
			glMatrixMode(GL_TEXTURE);
			glPopMatrix();	
			glMatrixMode(GL_MODELVIEW);
			break;
		case SOLID_RENDER:
			glCallList(solidDisplayList);
			break;
		case WIREFRAME_RENDER:
			glCallList(wireframeDisplayList);
			break;
		default:
			break;
		}
	}

	float multiplier = parent.app().getOptions()->speedMultiplier();
	animTime += parent.app().getFrameTime() * 1.8f * multiplier;
	if(animTime > 10)
		animTime -= 10;
}