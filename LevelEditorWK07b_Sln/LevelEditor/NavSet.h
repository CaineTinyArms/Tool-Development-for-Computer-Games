#pragma once
#include<glm/glm.hpp>
#include "Model.h"
#include "NavPoint.h"
#include <vector>
#include <glfw3.h>
class NavSet
{
public:
	vector<NavPoint> navPoints;
	Model* navModel = nullptr;

	void init();
	void draw(glm::mat4 viewProjectionMatrix);
};

