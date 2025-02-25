#pragma once
#ifndef MODEL_H
#define MODEL_H
#include <TextureLoader.h>
#include <Mesh.h>
#include <ShaderLoader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <tinyxml2.h>
#include <vector>
using namespace std;

/*Vertex Shader layouts:
 vertex data			- layout (location=0) in vec3
 normal data			- layout (location=1) in vec3
 texture coordinates	- layout (location=2) in vec2
 tangent				- layout (location=3) in vec3
 bi-tangent				- layout (location=4) in vec3

 */
class Model
{
public:
	/*  Model Data */
	vector <Model> children;
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh> meshes;
	string directory;
	GLuint objectShader;
	bool gammaCorrection;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	string name;
	string path;
	int ID = -1;
	bool open = false;
	bool selected = false;


	/*  Functions   */
// constructor, expects a filepath to a 3D model.

	Model() {};
	Model(string _name)
	{
		name = _name;
		pos = glm::vec3(0.0, 0.0, 0.0);
		rot = glm::vec3(0.0, 0.0, 0.0);
		scale = glm::vec3(1.0, 1.0, 1.0);
	}
	Model(string _name, glm::vec3 _pos, glm::vec3 _rot, glm::vec3 _scale)
	{
		name = _name;
		pos = _pos;
		rot = _rot;
		scale = _scale;
	}

	Model(string const& _path, string const& _name )
	{
			name = _name;
			path = _path;
			pos = glm::vec3(0.0, 0.0, 0.0);
			rot = glm::vec3(0.0, 0.0, 0.0);
			scale = glm::vec3(1.0, 1.0, 1.0);
			load(path);
	}
	//Model(string const& path, string const& _name, string& data);
	// constructor, expects a filepath to a 3D model.
Model(string const& _path, string const& _name, string& data)
	{
		name = _name;
		path = _path;
		setData(data);
		load(path);
	}
	// draws the model, and thus all its meshes
	void draw(glm::mat4 viewProjection);
	void draw(glm::mat4 parent, glm::mat4 viewProjectionMatrix);
	void draw(GLuint shader, glm::mat4 viewProjectionMatrix = glm::mat4(1));
	void attachTexture(GLuint textureID, string type = "texture_diffuse");
	void setShaders(string const& pathVS, string const& pathFS);

	void setData(string& data)
	{
		pos = vecFromStr(data);
		rot = vecFromStr(data);
		scale = vecFromStr(data);
	}
	glm::vec3 vecFromStr(string& str)
	{
		glm::vec3 vec3out(0);
		string vec = str.substr(str.find("(") + 1, str.find(")"));
		str = str.substr(str.find(")") + 1);
		for (int j = 0; j < 3; j++)
		{
			vec3out[j] = std::stof(vec.substr(0, vec.find(",")));
			vec = vec.substr(vec.find(",") + 1);
		}
		return vec3out;
	}
	std::string toXML() {
		std::stringstream ss;
		ss << "<PathName>" << path << "</PathName>";
		ss << "<Data>" << glm::to_string(pos) << glm::to_string(rot) << glm::to_string(scale) << "</Data>";
		return ss.str();
	}

private:
	/*  Functions   */
	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void load(string const& path);
	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif