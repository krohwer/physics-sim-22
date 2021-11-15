#pragma once

#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Environment.h"
#include "PhysicsObject.h"

#include <iostream>
#include <string>

namespace gui_utils {
	void createAllObjectMenus(Environment& env);
	void createSingleObjectMenu(Environment env, Body& object, int objectNumber);

	void createAllObjectMenus(Environment& env) {
		int objectNumber = 1;
		for (Body& body : env.bodyList) {
			createSingleObjectMenu(env, body, objectNumber);
			objectNumber++;
		}
	}

	void createSingleObjectMenu(Environment env, Body& object, int objectNumber) {
		std::string title = "Control Object " + std::to_string(objectNumber);
		std::string positionText = "Object " + std::to_string(objectNumber) + " Position";
		std::string xPositionText = "Object " + std::to_string(objectNumber) + " X Position";
		std::string yPositionText = "Object " + std::to_string(objectNumber) + " Y Position";
		std::string velocityText = "Object " + std::to_string(objectNumber) + " Velocity";
		std::string xVelocityText = "Object " + std::to_string(objectNumber) + " X Velocity";
		std::string yVelocityText = "Object " + std::to_string(objectNumber) + " Y Velocity";

		ImGui::Begin(title.c_str());
		ImGui::Text(positionText.c_str());
		ImGui::SliderFloat(xPositionText.c_str(), &object.position.x, 0.5f, env.width - 0.5f);
		ImGui::SliderFloat(yPositionText.c_str(), &object.position.y, 0.5f, env.height - 0.5f);
		ImGui::Text(velocityText.c_str());
		ImGui::SliderFloat(xVelocityText.c_str(), &object.velocity.x, -50.0f, 50.0f);
		ImGui::SliderFloat(yVelocityText.c_str(), &object.velocity.y, -50.0f, 50.0f);
		ImGui::End();
	}
};

#endif