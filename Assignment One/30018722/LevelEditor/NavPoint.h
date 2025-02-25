#pragma once
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <string>
#include<sstream>


class NavPoint
{
public:
	glm::vec3 pos; // Vec3 to hold the position of the NavPoint.
	glm::vec3 rot; // Vec3 to hold the rotation of the NavPoint.
	glm::vec3 scale; // Vec3 to hold the scale of the NavPoint.
	std::string name; // String to hold the name of the NavPoint.

	int ID = -1; // Int to hold the ID of the NavPoint.
	bool open = false; // Bool to hold the open value.
	bool selected = false; // Bool to hold the selected value.

	NavPoint(std::string _name) // Constructor that is passed a name.
	{
		pos = glm::vec3(0.0, 0.0, 0.0); // Sets the position to default value.
		rot = glm::vec3(0.0, 0.0, 0.0); // Sets the rotation to the default value.
		scale = glm::vec3(1.0, 1.0, 1.0); // Sets the scale to the default value.
		name = _name; // Sets the name to the name that is passed through.
	}

	std::string toXML(); // Prototype for the toXML function. 
	std::string buildDataString(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale); // Prototype for the buildDataString function.
};

