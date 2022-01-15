#pragma once

#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const float camSpeed = 6.0f;

glm::vec3 cameraPos(0.0f);
glm::vec3 cameraZoom(1.0f);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cameraPos.x = -camSpeed;
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cameraPos.x = camSpeed;
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cameraPos.y = -camSpeed;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		cameraPos.y = camSpeed;

	if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
		cameraPos.x = 0.0f;
	if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
		cameraPos.x = 0.0f;
	if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
		cameraPos.y = 0.0f;
	if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
		cameraPos.y = 0.0f;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cameraZoom.x += 0.25 * yoffset;
	cameraZoom.y += 0.25 * yoffset;
}

#endif
