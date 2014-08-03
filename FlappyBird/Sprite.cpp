#include "Sprite.h"
#include <pugixml.hpp>

using namespace pugi;

Sprite::Sprite():
	origX(0), origY(0), sizeX(0), sizeY(0)
{
}

Sprite::Sprite(const Texture&t, int origX, int origY, int sizeX, int sizeY):
	Texture(t), 
	origX(origX), origY(origY), sizeX(sizeX), sizeY(sizeY)
{
}

Sprite::Sprite(string path, int origX, int origY, int sizeX, int sizeY):
	Texture(path),
	origX(origX), origY(origY), sizeX(sizeX), sizeY(sizeY)
{
}

void Sprite::glTexCoordBottomLeft() const
{
	glTexCoord(origX, origY + sizeY);
}

void Sprite::glTexCoordTopLeft() const
{
	glTexCoord(origX, origY);
}

void Sprite::glTexCoordBottomRight() const
{
	glTexCoord(origX + sizeX, origY + sizeY);
}

void Sprite::glTexCoordTopRight() const
{
	glTexCoord(origX + sizeX, origY);
}


vector<Sprite> Sprite::loadFromXML(const Texture &texture, const string& xml, int border)
{
	xml_document doc;
	xml_parse_result result = doc.load_file(xml.c_str());
	if(!result){
		throw exception(("Couldn't load file " + xml + ": " + result.description()).c_str());
	}

	xpath_query queryFrames("//frame");
	xpath_node_set frames = queryFrames.evaluate_node_set(doc);
		
	vector<Sprite> res;
	for(xpath_node_set::const_iterator it = frames.begin(); it != frames.end(); ++it){
		int origX = it->node().attribute("x").as_int() + border;
		int origY = it->node().attribute("y").as_int() + border;
		int sizeX = it->node().attribute("w").as_int() - 2*border;
		int sizeY = it->node().attribute("h").as_int() - 2*border;

		res.push_back(Sprite(texture, origX, origY, sizeX, sizeY));
	}

	return res;
}

vector<Sprite> Sprite::loadFromImageAndXML(const string& texturePath, const string& xml, int border)
{
	Texture texture(texturePath);
	return Sprite::loadFromXML(texture, xml, border);
}

