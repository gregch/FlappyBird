#pragma once

enum Placement { CENTER, TOP_LEFT, TOP_CENTER, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT }; // Text placement in screen

struct SDL_Window;

class Rect
{
private:
	float borders[4]; // top, left, bottom, right
public:
	Rect();
	Rect(float top, float left, float bottom, float right);
	Rect(int top, int left, int bottom, int right);
	static Rect createBoundingRect(SDL_Window *win, Placement placement, int offsetX, int offsetY, int width, int height);

	// getters
	float top() const;
	float left() const;
	float bottom() const;
	float right() const;

	// utils
	bool contains(float x, float y) const;
	bool contains(int x, int y) const;
};
