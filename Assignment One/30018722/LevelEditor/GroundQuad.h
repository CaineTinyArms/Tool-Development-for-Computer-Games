#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <TextureLoader.h>
#include <ShaderLoader.h>
#include <fstream>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL 
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
using namespace std;
class GroundQuad
{
private:
	GLuint quadTexture;
	// Shader program object
	GLuint quadShaderProgram;
	GLuint quadPosVBO, quadColourVBO, quadTexCoordVBO, quadIndicesVBO;




public:
	GroundQuad();
	void Init();
	void drawQuad_VBO(glm::mat4 model, glm::mat4 viewProjectionMatrix);
	void setupQuadVBO(void);
};

