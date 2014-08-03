#pragma once

#include <string>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/Scene.h>
#include <SDL_opengl.h>
#include "Options.h"

class Model
{
private:
	// scene from the model is initialized
	std::string path;
	// the global Assimp scene object
	const aiScene* scene;
	// images / texture
	std::map<std::string, GLuint> textureIdMap;	// map image filenames to textureIds
	// Create an instance of the Importer class
	Assimp::Importer importer;
public:
	Model(); // an empty and non renderable model
	Model(const Model& other);
	Model(const std::string& path);
	Model& operator=(const Model& other);
	void load(const std::string& path);
	void reload(); // reload model (and textures) again from path
	void render(float scale, RenderMode mode, bool useMaterials = true);
	~Model();
private:
	void applyMaterial(const aiMaterial *mtl);
	void recursiveRender(const struct aiScene *sc, const struct aiNode* nd, float scale, RenderMode mode, bool useMaterials);
};
