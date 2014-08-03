#pragma once

#include "Texture.h"
#include <vector>

using namespace std;


class Sprite: public Texture
{
private:
	int origX, origY, sizeX, sizeY;
public:
	Sprite(); // empty sprite
	Sprite(const Texture&t, int origX, int origY, int sizeX, int sizeY); // create from a region of a texture
	Sprite(string path, int origX, int origY, int sizeX, int sizeY); // create from a region of a texture (loading by path)

	void glTexCoordBottomLeft() const;
	void glTexCoordTopLeft() const;
	void glTexCoordBottomRight() const;
	void glTexCoordTopRight() const;

	// loads a stripe collection from a existent texture and a XML. The XML specify stripe locations
	static vector<Sprite> loadFromXML(const Texture &texture, const string& xml, int border = 0); 
	// loads a stripe collection from a texture and a XML. The XML specify stripe locations
	static vector<Sprite> loadFromImageAndXML(const string& texturePath, const string& xml, int border = 0);
};

