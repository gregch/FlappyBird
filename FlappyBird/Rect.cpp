#include "Rect.h"
#include <SDL.h>

Rect::Rect()
{
	borders[0] = borders[1] = borders[2] = borders[3] = 0;
}

Rect::Rect(float top, float left, float bottom, float right)
{
	borders[0] = top;
	borders[1] = left;
	borders[2] = bottom;
	borders[3] = right;
}

Rect::Rect(int top, int left, int bottom, int right)
{
	borders[0] = (float) top;
	borders[1] = (float) left;
	borders[2] = (float) bottom;
	borders[3] = (float) right;
}

float Rect::top() const
{
	return borders[0];
}

float Rect::left() const
{
	return borders[1];
}

float Rect::bottom() const
{
	return borders[2];
}

float Rect::right() const
{
	return borders[3];
}

bool Rect::contains(float x, float y) const
{
	return x >= left() && x <= right() && y >= top() && y <= bottom();
}

bool Rect::contains(int x, int y) const
{
	return x >= left() && x <= right() && y >= top() && y <= bottom();
}

Rect Rect::createBoundingRect(SDL_Window *win, Placement placement, int offsetX, int offsetY, int objectWidth, int objectHeight)
{
	int width, height;
	SDL_GetWindowSize(win, &width, &height);

	int left = offsetX;
	int right = left + objectWidth;
	int top = offsetY;
	int bottom = top + objectHeight;

	if(placement == CENTER)
	{
		int offsetCenterX = width / 2 - objectWidth / 2;
		left += offsetCenterX;
		right += offsetCenterX;
		int offsetCenterY = height / 2 - objectHeight / 2;
		top += offsetCenterY;
		bottom += offsetCenterY;
	} else if(placement == TOP_CENTER) {
		int offsetCenterX = width / 2 - objectWidth / 2;
		left += offsetCenterX;
		right += offsetCenterX;
	} else if(placement == BOTTOM_LEFT) {
		bottom = height - offsetY;
		top = bottom - objectHeight;
	} else if(placement == BOTTOM_RIGHT) {
		bottom = height - offsetY;
		top = bottom - objectHeight;
		right = width - offsetY;
		left = right - objectWidth;
	} else if(placement == TOP_RIGHT) {
		right = width - offsetY;
		left = right - objectWidth;
	}

	return Rect(top, left, bottom, right);
}