#include "GroundQuad.h"

GroundQuad::GroundQuad() {

}
void GroundQuad::Init() {


	GLSL_ERROR glsl_error = ShaderLoader::createShaderProgram(
		string("..\\..\\Resources\\Shaders\\Object_shader.vert"),
		string("..\\..\\Resources\\Shaders\\Object_shader.frag"), &quadShaderProgram);
	cout << glsl_error << endl;


	quadTexture = TextureLoader::loadTexture("..\\..\\Resources\\Models\\bumblebee.png");

	setupQuadVBO();
}

void GroundQuad::setupQuadVBO(void) {

	static GLfloat boxVertices[] = {

	-0.4f, -0.7f,0,
	-0.4f, 0.7f,0,
	0.4f, -0.7f,0,
	0.4f, 0.7f,0

	};
	static GLfloat boxColors[] = {

		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 255
	};

	static GLfloat boxTextureCoords[] = {

	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
	};

	static GLubyte boxIndices[] = { 0,1,2,3 };

	// setup VBO for the star object position data
	glGenBuffers(1, &quadPosVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	// setup VBO for the star object colour data
	glGenBuffers(1, &quadColourVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxColors), boxColors, GL_STATIC_DRAW);

	// setup VBO for the star object texture coord data
	glGenBuffers(1, &quadTexCoordVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxTextureCoords), boxTextureCoords, GL_STATIC_DRAW);

	// setup star vertex index array
	glGenBuffers(1, &quadIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);
}
void GroundQuad::drawQuad_VBO(glm::mat4 model, glm::mat4 viewProjectionMatrix) {


	glUseProgram(quadShaderProgram);
	//glm::mat4 model = glm::scale(glm::mat4(1.0), glm::vec3(1, 1, 1)) * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
	//glm::mat4 viewProjectionMatrix =
	glUniformMatrix4fv(glGetUniformLocation(quadShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(quadShaderProgram, "viewProjection"), 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));



	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, quadTexture);
	glUniform1i(glGetUniformLocation(quadShaderProgram, "beeTexture"), 0);
	glEnable(GL_TEXTURE_2D);

	// Bind each vertex buffer and enable
	// The data is still stored in the GPU but we need to set it up (which also includes validation of the VBOs behind-the-scenes)
	glBindBuffer(GL_ARRAY_BUFFER, quadPosVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, quadColourVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);
	glEnableVertexAttribArray(2);

	// Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIndicesVBO);



	// "Plug in" our shader into the GPU pipeline - we're now in the driving seat!!!!!  Our shaders now intercept and process our vertices as part of the GPU rendering pipeline!
	//glUseProgram(myShaderProgram);


	// Draw the object - same function call as used for vertex arrays but the last parameter is interpreted as an offset into the currently bound index buffer (set to 0 so we start drawing from the beginning of the buffer).
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, (GLvoid*)0);
}