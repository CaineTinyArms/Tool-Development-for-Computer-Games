#include "NavSet.h"

void NavSet::init()
{
	navModel = new Model("../../Resources/Models/Sphere.obj", "Sphere"); // Sets the navModel as a new Model class with the Sphere model.
}

void NavSet::draw(glm::mat4 viewProjectionMatrix)
{
	for (NavPoint navPoint : NavPoints) // Goes through every NavPoint.
	{
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), navPoint.pos) * glm::scale(glm::mat4(1.0), navPoint.scale); // Creates a model matrix from the position and scale.
		// Adds the rotation to the model matrix.
		modelMatrix = modelMatrix * glm::rotate(glm::mat4(1.0), navPoint.rot.z, glm::vec3(0.0, 0.0, 1.0)) * glm::rotate(glm::mat4(1.0), navPoint.rot.y, glm::vec3(0.0, 1.0, 0.0)) * glm::rotate(glm::mat4(1.0), navPoint.rot.x,glm::vec3(1.0, 0.0, 0.0));
		glStencilFunc(GL_ALWAYS, (GLint)navPoint.ID, 0xFF); // Sets the GL Stencil to the ID of the  current NavPoint.

		navModel->draw(modelMatrix, viewProjectionMatrix); // Draws the sphere model with the newly created modelMatrix.
	}
}