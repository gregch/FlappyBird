#include "Model.h"
#include "Texture.h"
#include <fstream>
#include <stdexcept>
#include <assimp/aiScene.h>
#include <assimp/aiPostProcess.h>

Model::Model():
	scene(NULL)
{
}

Model::~Model()
{
}

Model::Model(const Model& other)
{
	path = other.path;
	reload();
}

Model::Model(const std::string& path):
	path(path)
{
	reload();
}

Model& Model::operator=(const Model& other)
{
	path = other.path;
	reload();
	return *this;
}

void Model::load(const string& path)
{
	this->path = path;
	reload();
}

void Model::reload()
{
	//check if file exists
	std::ifstream fin(path.c_str());
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		throw std::invalid_argument(("Couldn't open file " + path + ": " + importer.GetErrorString()).c_str());
	}
	scene = importer.ReadFile( path, aiProcessPreset_TargetRealtime_Quality);
	// If the import failed, report it
	if(!scene)
	{
		throw std::invalid_argument((std::string("Assimp couldn't load model: ") + std::string(importer.GetErrorString())).c_str());
	}

	if (scene->HasTextures()){
			throw std::invalid_argument("Couldn't load textures: Support for meshes with embedded textures is not implemented");
	}

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m=0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			texIndex++;
		}
	}


	/* get iterator */
	for(std::map<std::string, GLuint>::iterator itr = textureIdMap.begin(); itr != textureIdMap.end(); ++itr)
	{
		Texture t(itr->first);
		itr->second = t.id();
	}
}

static void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

static void color4_to_float4(const aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Model::applyMaterial(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4D diffuse;
	aiColor4D specular;
	aiColor4D ambient;
	aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;	// changed: to unsigned

	int texIndex = 0;
	aiString texPath;	//contains filename of texture

	if(AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath))
	{
		//bind texture
		unsigned int texId = textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS)){
		float shininessVal =  shininess * strength;
		if(shininessVal > 128)
			shininessVal = 128;
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininessVal);
	}else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;

	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}


void Model::recursiveRender (const struct aiScene *sc, const struct aiNode* nd, float scale, RenderMode mode, bool useMaterials)
{
	unsigned int i;
	unsigned int n=0, t;
	aiMatrix4x4 m = nd->mTransformation;

	m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n)
	{
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		if(useMaterials)
		{
			applyMaterial(sc->mMaterials[mesh->mMaterialIndex]);
		}
		
		if(mode == WIREFRAME_RENDER)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}


		if(mesh->mNormals == NULL)
		{
			glDisable(GL_LIGHTING);
		}
		else
		{
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		else
		{
			glDisable(GL_COLOR_MATERIAL);
		}



		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices)
			{
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if(mesh->mColors[0] != NULL){
					const aiColor4D *color = &mesh->mColors[0][vertexIndex];
					glColor4f(color->r, color->g, color->b, color->a);
				}if(mesh->mNormals != NULL)

					if(mode == TEXTURED_RENDER && mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

					glNormal3fv(&mesh->mNormals[vertexIndex].x);
					glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();

		}

	}


	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n)
	{
		recursiveRender(sc, nd->mChildren[n], scale, mode, useMaterials);
	}

	glPopMatrix();
}


void Model::render(float scale, RenderMode mode, bool useMaterials)
{
	recursiveRender(scene, scene->mRootNode, scale, mode, useMaterials);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}