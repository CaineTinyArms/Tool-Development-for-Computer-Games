#include "NavSet.h"

void NavSet::init()
{
	navModel = new Model("../../Resources/Models/Sphere.obj", "Sphere");
}

void NavSet::draw(glm::mat4 viewProjectionMatrix)
{
	for (NavPoint navPoint : NavPoints)
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), navPoint.pos) * glm::scale(glm::mat4(1.0), navPoint.scale);
		modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0), navPoint.rot.z, glm::vec3(0.0, 0.0, 1.0)) * glm::rotate(glm::mat4(1.0), navPoint.rot.y, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), navPoint.rot.x,glm::vec3(1.0, 0.0, 0.0));
		glStencilFunc(GL_ALWAYS, (GLint)navPoint.ID, 0xFF);

		navModel->draw(modelMatrix, viewProjectionMatrix);
	}
}