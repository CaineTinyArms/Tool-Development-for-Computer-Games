#include "InputMgr.h"
// process all isnput : query GLFW whether relevant keys are pressed / released this frame and react accordingly
void InputMgr::processInput(GLFWwindow * window)
{
	timer.tick();
	timer.updateDeltaTime();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera[(int)currentCam].processKeyboard(FORWARD, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera[(int)currentCam].processKeyboard(BACKWARD, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera[(int)currentCam].processKeyboard(LEFT, timer.getDeltaTimeSeconds());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera[(int)currentCam].processKeyboard(RIGHT, timer.getDeltaTimeSeconds());
}

void InputMgr::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		if (MOVEMODE == MoveCam)
			MOVEMODE = MoveObj;
		else
			MOVEMODE = MoveCam;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		currentCam = Camera_Type::SIDE;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		currentCam = Camera_Type::PERSPECTIVE;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		currentCam = Camera_Type::TOP;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		currentCam = Camera_Type::FRONT;

}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
void InputMgr::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

//The callback function receives the mouse button, button actionand modifier bits.
void InputMgr::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if ((button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT) && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		lastX = xpos;
		lastY = ypos;

		double width = camera_settings->screenWidth;
		double height = camera_settings->screenHeight;

		GLubyte  stencil;
		glReadPixels((GLint)xpos, (GLint)(height - ypos), 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &stencil);
		cout << "Stencil = " << (int)stencil << endl;
		if (stencil > 2 && stencil < 128) {
			if (!gui->scene.selectModelByID(&gui->scene.rootModel, (int)stencil))
				cout << "Model ID " << (int)stencil << " not found" << endl;
		}
		else if (stencil > 127)
		{
			if (!gui->scene.selectNodeByID((int)stencil))
			{
				cout << "Node ID " << (int)stencil << "not found" << endl;
			}
		}
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
}

// glfw: whenever the mouse moves, this callback is called
void InputMgr::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//In model move mode so update model
	if (MOVEMODE == MoveObj)
	{
		//if (CurrentObject >= 0 && (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) || (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))

		if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) || (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS))
		{
			double width = camera_settings->screenWidth;
			double height = camera_settings->screenHeight;
	
			//Left mouse button down to move
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			{
				if(!gui->scene.selected_model || gui->scene.selected_navPoint)
					return;
				//Convert mouse move distance on screen into distance in world space 
				double aspect = (double)width / (double)height;
				double movx = (xoffset / width * 2.0) * aspect * camera[(int)currentCam].Zoom;
				double movy = (yoffset / height * 2.0) * camera[(int)currentCam].Zoom;

				if (currentCam == Camera_Type::SIDE)
				{
					//in side view (looking along z axis) move x and y position
					gui->scene.selected_model->pos += glm::vec3(movx, movy, 0.0);
				}
				if (currentCam == Camera_Type::PERSPECTIVE)
				{
					//we dont move objects in 3d perspective view yet as its too complex
					//we just move camera instead
					camera[(int)currentCam].processMouseMovement(xoffset, yoffset, 0.0);
				}
				if (currentCam == Camera_Type::TOP)
				{
					//in top view (looking down along y axis) move x and z position
					gui->scene.selected_model->pos += glm::vec3(movx, 0.0, -movy);
				}
				if (currentCam == Camera_Type::FRONT)
				{
					//in front view (looking along x axis) move z and z position
					gui->scene.selected_model->pos += glm::vec3(0.0, movy, -movx);
				}
			}
			//Right mouse button down to rotate
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			{
				if (currentCam == Camera_Type::SIDE)
					//in side view (looking along z axis) rotate around z
					gui->scene.selected_model->rot += glm::vec3(0.0, 0.0, xoffset / 1000);
				if (currentCam == Camera_Type::PERSPECTIVE)
					//we dont move objects in 3d perspective view yet as its too complex
					//we just move camera instead
					camera[(int)currentCam].processMouseMovement(xoffset, yoffset, 0.0);
				if (currentCam == Camera_Type::TOP)
					//in side view (looking along y axis) rotate around y
					gui->scene.selected_model->rot += glm::vec3(0.0, xoffset / 1000, 0.0);
				if (currentCam == Camera_Type::FRONT)
					//in side view (looking along x axis) rotate around x
					gui->scene.selected_model->rot += glm::vec3(xoffset / 1000, 0.0, 0.0);
			}
		}
	}
	// In camera move mode so update camera
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (currentCam == Camera_Type::SIDE)
			camera[(int)currentCam].processMouseMovement(xoffset, yoffset, 0);
		if (currentCam == Camera_Type::PERSPECTIVE)
			camera[(int)currentCam].processMouseMovement(xoffset, yoffset, 0.0);
		if (currentCam == Camera_Type::TOP)
			camera[(int)currentCam].processMouseMovement(xoffset, 0, -yoffset);
		if (currentCam == Camera_Type::FRONT)
			camera[(int)currentCam].processMouseMovement(0, yoffset, -xoffset);
	}
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void InputMgr::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

	if (MOVEMODE == MoveObj && !(currentCam == Camera_Type::PERSPECTIVE))
	{
		if (gui->scene.selected_model)
			gui->scene.selected_model->scale += 0.1 * yoffset;
	}
	else
		camera[(int)currentCam].processMouseScroll(yoffset);
}
