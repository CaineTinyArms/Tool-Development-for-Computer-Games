#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "NavPoint.h"
#include "Model.h"

class NavSet
{
public:
	vector<NavPoint> NavPoints; // Vector of NavPoint class objects to store the NavPoints.
	Model* navModel = nullptr; // Model variable to hold the model.

	void init(); // Prototype for the init function.
	void draw(glm::mat4 viewProjectionMatrix); // Prototype for the draw function.
};

