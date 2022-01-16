#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Camera.h"

Camera* cameraInput;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		cameraInput->cVelocity.x += CAM_SPEED;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
		cameraInput->cVelocity.x -= CAM_SPEED;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		cameraInput->cVelocity.x -= CAM_SPEED;
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
		cameraInput->cVelocity.x += CAM_SPEED;
	}

	if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS)
			cameraInput->cVelocity.y += CAM_SPEED;
		if (action == GLFW_RELEASE)
			cameraInput->cVelocity.y -= CAM_SPEED;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		cameraInput->cVelocity.y -= CAM_SPEED;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
		cameraInput->cVelocity.y += CAM_SPEED;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (yoffset > 0) {
		cameraInput->cZoom *= 0.9f;
	}
	if (yoffset < 0) {
		cameraInput->cZoom *= 1.1f;
	}
}

#endif
