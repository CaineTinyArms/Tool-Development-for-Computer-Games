#pragma once
#include <Timer.h>
#include "Camera.h"
#include "Model.h"
#include "GUIMgr.h"



class InputMgr
{
public:
	Camera_settings *camera_settings;
	float lastX = -1;
	float lastY = -1;
	enum moveMode { MoveCam = 1, MoveObj = 2 };
	int MOVEMODE = MoveCam;

//	Timer
	Timer timer;
	Camera_Type currentCam;
	GUIMgr* gui;
	Camera* camera;

	InputMgr(GUIMgr* _gui, Camera_settings* _camera_settings, Camera* _camera, Camera_Type _currentCam= Camera_Type::FRONT) {
		gui=_gui;
		camera_settings = _camera_settings; camera = _camera;
		currentCam = _currentCam;

	};
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

};
//#endif
