#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

const float CAM_SPEED = 6.0f;

struct Camera {
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float cLeft;
	float cRight;
	float cBottom;
	float cTop;
	
	glm::vec3 cPosition;
	glm::vec3 cVelocity;
	float cZoom;

	Camera(float left, float right, float bottom, float top);

	void recalculateView();
};

#endif