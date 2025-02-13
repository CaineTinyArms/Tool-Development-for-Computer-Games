#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "NavPoint.h"
#include "Model.h"

class NavSet
{
public:
	vector<NavPoint> NavPoints;
	Model* navModel = nullptr;

	void init();
	void draw(glm::mat4 viewProjectionMatrix);
};

