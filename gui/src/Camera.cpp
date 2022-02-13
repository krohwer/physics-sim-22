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
	if (!disabled) {
		if (cPosition.x > 0 || cVelocity.x > 0)
			cPosition.x += cVelocity.x * cZoom;
		else
			cPosition.x = 0.0f;
		if (cPosition.y > 0 || cVelocity.y > 0)
			cPosition.y += cVelocity.y * cZoom;
		else
			cPosition.y = 0.0f;
		projectionMatrix = glm::ortho(cLeft * cZoom, cRight * cZoom, cBottom * cZoom, cTop * cZoom, -1.0f, 1.0f);
	}
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), cPosition);
	viewMatrix = glm::inverse(transform);
}

