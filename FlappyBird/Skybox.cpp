#include "Skybox.h"
#include "Vector3.h"
#include <GL/GLU.h>

Skybox::Skybox(Camera *camera):
	texture("skybox.png", true, true),
	camera(camera)
{
	texturedDisplayList = glGenLists(3);
	glNewList (texturedDisplayList, GL_COMPILE);
	{
		render(TEXTURED_RENDER);
	}
	glEndList ();
	solidDisplayList = texturedDisplayList+1;
	glNewList (solidDisplayList, GL_COMPILE);
	{
		render(SOLID_RENDER);
	}
	glEndList ();

	wireframeDisplayList = texturedDisplayList+2;
	glNewList (wireframeDisplayList, GL_COMPILE);
	{
		render(WIREFRAME_RENDER);
	}
	glEndList ();
}

void Skybox::render(Scene &parent)
{
   glPushMatrix();
   glLoadIdentity();
   gluLookAt(Vector3::zero, camera->getPosition() - camera->getCenter(), Vector3::down);
   switch(parent.app().getOptions()->renderMode())
   {
   case TEXTURED_RENDER:
	   glCallList(texturedDisplayList);
	   break;
   case SOLID_RENDER:
	   glCallList(solidDisplayList);
	   break;
   case WIREFRAME_RENDER:
	   glCallList(wireframeDisplayList);
	   break;
   }
}

void Skybox::render(RenderMode mode)
{
	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	{
		glToggle(GL_TEXTURE_2D, mode == TEXTURED_RENDER);
		if(mode == WIREFRAME_RENDER)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_BLEND);

		// Just in case we set all vertices to white.
		if(mode == TEXTURED_RENDER)
		{
			glColor4f(1,1,1,1);
		} else {
			glColor(Vector3::fromRGB(119, 196, 242));
		}

		const float deltaX = 0.25f;

		// Render the front quad
		glBindTexture(texture);
		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*3, 0); } glVertex3f(  0.5f, -0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(1,		  0); } glVertex3f( -0.5f, -0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(1,		  1); } glVertex3f( -0.5f,  0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*3, 1); } glVertex3f(  0.5f,  0.5f, -0.5f );
		}
		glEnd();

		// Render the left quad
		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*2, 0); } glVertex3f(  0.5f, -0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*3, 0); } glVertex3f(  0.5f, -0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*3, 1); } glVertex3f(  0.5f,  0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*2, 1); } glVertex3f(  0.5f,  0.5f,  0.5f );
		}
		glEnd();

		// Render the back quad
		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX, 0); } glVertex3f( -0.5f, -0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*2, 0); } glVertex3f(  0.5f, -0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX*2, 1); } glVertex3f(  0.5f,  0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX, 1); } glVertex3f( -0.5f,  0.5f,  0.5f );
		}
		glEnd();

		// Render the right quad
		glBegin(GL_QUADS);
		{
			if(mode == TEXTURED_RENDER){ glTexCoord2f(0,		0); }		glVertex3f( -0.5f, -0.5f, -0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX, 0); } glVertex3f( -0.5f, -0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(deltaX, 1); } glVertex3f( -0.5f,  0.5f,  0.5f );
			if(mode == TEXTURED_RENDER){ glTexCoord2f(0,		1); } glVertex3f( -0.5f,  0.5f, -0.5f );
		}
		glEnd();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Restore enable bits and matrix
		glPopAttrib();
	}
	glPopMatrix();
}