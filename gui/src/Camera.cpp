#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top) {
	projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	viewMatrix = glm::mat4(1.0f);

	cLeft = left;
	cRight = right;
	cBottom = bottom;
	cTop = top;

	cPosition = glm::vec3(right, top, 0.0f);
	cVelocity = glm::vec3(0.0f);
	cZoom = 1.0f;

	disabled = false;
}

void Camera::recalculateView() {
	cPosition += cVelocity * cZoom;
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), cPosition);

	viewMatrix = glm::inverse(transform);
	projectionMatrix = glm::ortho(cLeft * cZoom, cRight * cZoom, cBottom * cZoom, cTop * cZoom, -1.0f, 1.0f);
}

