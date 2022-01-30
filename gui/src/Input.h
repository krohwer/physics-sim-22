#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

#include "Camera.h"

Camera* cameraInput;
bool createObject = false;
bool disableCamera = false;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!disableCamera) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_A) {
			if (action == GLFW_PRESS)
				cameraInput->cVelocity.x -= CAM_SPEED;
			if (action == GLFW_RELEASE)
				cameraInput->cVelocity.x += CAM_SPEED;
		}
		if (key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) {
			if (action == GLFW_PRESS)
				cameraInput->cVelocity.x += CAM_SPEED;
			if (action == GLFW_RELEASE)
				cameraInput->cVelocity.x -= CAM_SPEED;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_W) {
			if (action == GLFW_PRESS)
				cameraInput->cVelocity.y += CAM_SPEED;
			if (action == GLFW_RELEASE)
				cameraInput->cVelocity.y -= CAM_SPEED;
		}
		if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S) {
			if (action == GLFW_PRESS)
				cameraInput->cVelocity.y -= CAM_SPEED;
			if (action == GLFW_RELEASE)
				cameraInput->cVelocity.y += CAM_SPEED;
		}
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (!disableCamera) {
		if (yoffset > 0) {
			cameraInput->cZoom *= 0.9f;
		}
		if (yoffset < 0) {
			cameraInput->cZoom *= 1.1f;
		}
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		createObject = true;
}

#endif
