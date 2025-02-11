#include "NavSet.h"

void NavSet::init()
{
	navModel = new Model("../../Resources/Models/Sphere.obj", "Sphere");
}

void NavSet::draw(glm::mat4 viewProjectionMatrix)
{
	for (int navPoint = 0; navPoint < navPoints.size(); navPoint++)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), navPoints[navPoint].pos) * glm::scale(glm::mat4(1.0), navPoints[navPoint].scale);
		modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0), navPoints[navPoint].rot.z, glm::vec3(0.0, 0.0, 1.0)) * glm::rotate(glm::mat4(1.0), navPoints[navPoint].rot.y, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), navPoints[navPoint].rot.x, glm::vec3(1.0, 0.0, 0.0));
		glStencilFunc(GL_ALWAYS, (GLint)navPoints[navPoint].ID, 0xFF);
		if (navModel)
		{
			navModel->draw(modelMatrix, viewProjectionMatrix);
		}
	}
}
