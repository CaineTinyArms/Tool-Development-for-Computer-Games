#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Defines several enum possible options for camera movement to easier understand the direction
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum class Camera_Type
{
	SIDE, PERSPECTIVE, TOP, FRONT
};

// Struct to store and pass camera settings
struct Camera_settings
{
	unsigned int screenWidth;
	unsigned int screenHeight;
	double nearPlane;
	double farPlane;
};

// Default camera values
const double YAW = -90.0f;
const double PITCH = 0.0f;
const double SPEED = 10.5f;
const double SENSITIVITY = 0.1f;
const double ZOOM = 45.0f;

// Camera class
class Camera
{
private:
	// Camera Attributes
	Camera_Type Type;
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	double Yaw;
	double Pitch;

	// Camera options
	double MovementSpeed;
	double MouseSensitivity;

	double NearPlane;
	double FarPlane;
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();

public:
	// Screen options
	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
	double Zoom;
	// Default Constructor
	Camera(Camera_settings camera_settings);

	// Constructor with vectors
	Camera(Camera_settings camera_settings, glm::vec3 position);

	// Constructor with vectors
	Camera(Camera_settings camera_settings, glm::vec3 position, glm::vec3 target, glm::vec3 up, Camera_Type type = Camera_Type::PERSPECTIVE);
	// Constructor with vectors
	Camera(Camera_settings camera_settings, glm::vec3 position, glm::vec3 target, glm::vec3 up, double yaw, double pitch);

	// Constructor with scalar values
	Camera(Camera_settings camera_settings, double posX, double posY, double posZ, double upX, double upY, double upZ, double tarX, double tarY, double tarZ, double yaw, double pitch);

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 getViewMatrix();

	// Returns the projection matrix calculated using screen size, zoom, near and far planes
	glm::mat4 getProjectionMatrix();

	// Returns the camera position
	glm::vec3 getCameraPosition();

	glm::vec3 unProject(double xndc, double yndc, glm::mat4* model)
	{
		glm::mat4 mvMatrix = getProjectionMatrix() * getViewMatrix() * (*model);
		//glm::mat4 invMvMatrix = glm::inverse(camera[(int)CAMERA].getViewMatrix());
		glm::mat4 invMvMatrix = glm::inverse(mvMatrix);
		//glm::mat4 invProjMatrix = glm::inverse(camera[(int)CAMERA].getProjectionMatrix());
		glm::vec4 clipCoords = glm::vec4(xndc, yndc, -1.0, 1.0);
		//glm::vec4 eyeCoords = invProjMatrix * clipCoords;
		glm::vec4 worldCoords = invMvMatrix * clipCoords;
		glm::vec3 finalPos = glm::vec3(worldCoords) / worldCoords.w;
		return finalPos;
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, double deltaTime);

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(double xoffset, double yoffset, double zoffset);

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(double yoffset);

	// Updates the screen width and height data to return correct view matrix if screen size has changed
	void updateScreenSize(double width, double height);

};

#endif

