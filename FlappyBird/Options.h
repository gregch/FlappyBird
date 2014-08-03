#pragma once

#include <SDL_opengl.h>

enum GameSpeed { SLOW = 2, MEDIUM = 0, FAST = 1 };
enum RenderMode { TEXTURED_RENDER, WIREFRAME_RENDER, SOLID_RENDER };

class Options
{
private:
	bool		textures;
	bool		wireframe;
    GameSpeed	gameSpeed;
public:
	Options();

	bool		getTexturesEnabled() const;
	void		setTexturesEnabled(bool val);
	bool		getWireframeEnabled() const;
	RenderMode	renderMode();
	void		setWireframeEnabled(bool val);
	GameSpeed	getGameSpeed() const;
	float		speedMultiplier() const;
	void		setGameSpeed(GameSpeed val);
	void		setShadingModel(bool smooth);
};

void glToggle(GLenum capability, GLboolean val);