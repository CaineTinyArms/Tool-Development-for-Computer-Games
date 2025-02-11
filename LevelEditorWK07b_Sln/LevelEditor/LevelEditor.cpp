// LevelEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <glad/glad.h>
#include <glfw3.h>
#include <vector>
#include <stack>
#include <iostream>

#include "GUIMgr.h"
#include "InputMgr.h"
#include "Model.h"
#include "Scene.h"
#include "Camera.h"
//#include "GroundQuad.h"

using namespace std;

// Camera settings
//							  width, height, near plane, far plane
Camera_settings camera_settings{ 1280, 1024, 0.1, 1000.0 };
//3d
//Camera camera = Camera(camera_settings, glm::vec3(0.0f, 5.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
// Instantiate the camera object with basic data
Camera camera[] =
{
	//side
	Camera(camera_settings, glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f, 0.0f), Camera_Type::SIDE),
	//3d
	Camera(camera_settings, glm::vec3(0.0f, 5.0f, 100.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f, 0.0f), Camera_Type::PERSPECTIVE),
	//top
	Camera(camera_settings, glm::vec3(0.0f, 100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,0.0f, -1.0f), Camera_Type::TOP),
	//front
	Camera(camera_settings, glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,1.0f, 0.0f), Camera_Type::FRONT)
};

GUIMgr GUI;
InputMgr inputMgr = InputMgr(&GUI, &camera_settings, camera);

//GroundQuad ground;

void drawModelHierarchy(Model* modelHierarchy, glm::mat4* parentMatrix, glm::mat4* viewProjectionMatrix)
{
	glm::vec3 pos = modelHierarchy->pos;
	glm::vec3 rot = modelHierarchy->rot;
	glm::vec3 scale = modelHierarchy->scale;
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), pos) * glm::scale(glm::mat4(1.0), scale);
	modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0), rot.z, glm::vec3(0.0, 0.0, 1.0)) * glm::rotate(glm::mat4(1.0), rot.y, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 newParentMatrix = (*parentMatrix) * modelMatrix;

	

	if (GUI.scene.selected_model == modelHierarchy)
		modelHierarchy->selected = true;
	else
		modelHierarchy->selected = false;
	if (modelHierarchy->meshes.size() > 0)
	{
		glStencilFunc(GL_ALWAYS, (GLint)modelHierarchy->ID , 0xFF);
		modelHierarchy->draw(*parentMatrix, *viewProjectionMatrix);
	}

	for (int i = 0; i < modelHierarchy->children.size(); i++)
		drawModelHierarchy(&modelHierarchy->children[i],&newParentMatrix, viewProjectionMatrix);
}

int main()
{
	//remove the console window ans send cout to buffer for imGUI
//	FreeConsole();
	std::stringstream buffer;
	//std::streambuf* oldcout = std::cout.rdbuf(buffer.rdbuf());
	//std::streambuf* oldcerr = std::cerr.rdbuf(buffer.rdbuf());

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(1280, 1024, "Level Editor", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the callback functions
	glfwMakeContextCurrent(window); 
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {inputMgr.framebuffer_size_callback(window, width, height);});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {inputMgr.mouse_callback(window, xpos, ypos);});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {inputMgr.scroll_callback(window, xoffset,  yoffset); });
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {inputMgr.mouse_button_callback(window,  button, action, mods); });
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {inputMgr.key_callback(window,key,scancode,action,mods); });


	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << "Initialising OpenGL done!" << std::endl;

	//Rendering settings
	glfwSwapInterval(1);		// glfw enable swap interval to match screen v-sync
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);	//Enables face culling
	//glFrontFace(GL_CCW);		//Specifies which winding order if front facing
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//only update stencil buffer if depth test passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	
	//Iinitialise IMGU
	GUI.init(window);

//	ground.Init();


	// render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		inputMgr.processInput(window);

		// Clear the screen
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, camera_settings.screenWidth, camera_settings.screenHeight);//viewport fills window
		glm::mat4 viewProjectionMatrix = camera[(int)inputMgr.currentCam].getProjectionMatrix() * camera[(int)inputMgr.currentCam].getViewMatrix();
		
		glm::mat4 I = glm::mat4(1);

		glEnable(GL_STENCIL_TEST);
		drawModelHierarchy(&GUI.scene.rootModel, &I, &viewProjectionMatrix);
		glDisable(GL_STENCIL_TEST);

	//	ground.drawQuad_VBO(I, viewProjectionMatrix);

		//Draw all GUI components with one function call
		GUI.drawAll(&buffer);

		// glfw: swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup ImGUI. After the render loop but before “glfwTerminate(); ” 
	GUI.shutDown();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
