#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

#include "Model.h"
#include "Scene.h"

using namespace std;

class GUIMgr
{
public:
	Scene scene;
	bool showFileBrowser = false;
	void init(GLFWwindow* window);
	void createFrame();
	void drawMenu();
	void drawPropertiesWindow(Model* model);
	void draw();//Finalise ImGUI rendering
	void shutDown();//cleanup ImGUI
	void drawConsoleWindow(std::stringstream* buffer);
	void drawAll(std::stringstream* buffer);
	void DrawModel(Model* node);
};

