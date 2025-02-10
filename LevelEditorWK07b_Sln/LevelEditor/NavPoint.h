#pragma once
#include <glm/glm.hpp>
#include <string>
class NavPoint
{
public:
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	std::string name;

	int ID = -1;
	bool open = false;
	bool selected = false;

	NavPoint(std::string _name)
	{
		name = _name;
		pos = glm::vec3(0.0, 0.0, 0.0);
		rot = glm::vec3(0.0, 0.0, 0.0);
		scale = glm::vec3(1.0, 1.0, 1.0); 
	}
};

