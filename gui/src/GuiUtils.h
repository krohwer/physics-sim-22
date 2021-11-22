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
	void createSingleObjectMenu(Environment& env, Body& object, int objectNumber) {
		// This may seem excessive, but ImGUI uses the title of the menu as its ID
		// So we need to create unique titles for each object and each attribute
		// Users would likely want to differentiate between the objects as well
		std::string title = "Control Object " + std::to_string(objectNumber);
		std::string positionText = "Object " + std::to_string(objectNumber) + " Position";
		std::string xPositionText = "Object " + std::to_string(objectNumber) + " X Position";
		std::string yPositionText = "Object " + std::to_string(objectNumber) + " Y Position";
		std::string velocityText = "Object " + std::to_string(objectNumber) + " Velocity";
		std::string xVelocityText = "Object " + std::to_string(objectNumber) + " X Velocity";
		std::string yVelocityText = "Object " + std::to_string(objectNumber) + " Y Velocity";
		std::string massText = "Object " + std::to_string(objectNumber) + " Mass";
		std::string scaleText = "Object " + std::to_string(objectNumber) + " Size";
		std::string xScaleText = "Object " + std::to_string(objectNumber) + " Width";
		std::string yScaleText = "Object " + std::to_string(objectNumber) + " Height";
		std::string deleteText = "Delete Object " + std::to_string(objectNumber);

		// Allows us to make the width smaller for the input fields
		// Scales based off the width of the window
		ImGui::PushItemWidth(ImGui::GetWindowWidth() / 3.0f);

		ImGui::Text(positionText.c_str());
		ImGui::InputFloat(xPositionText.c_str(), &object.position.x);
		ImGui::InputFloat(yPositionText.c_str(), &object.position.y);

		// Boundary checking for x position input
		if (object.position.x > env.width - 0.5f * object.scale.x)
			object.position.x = env.width - 0.5f * object.scale.x;
		if (object.position.x < 0.5f * object.scale.x)
			object.position.x = 0.5f * object.scale.x;
		// Boundary checking for y position input
// 		if (object.position.y > env.height - 0.5f * object.scale.y) // Limits ceiling, but since that's nonexistent, let it be
// 			object.position.y = env.height - 0.5f * object.scale.y;
		if (object.position.y < 0.5f * object.scale.y)
			object.position.y = 0.5f * object.scale.y;

		ImGui::Text(velocityText.c_str());
		ImGui::InputFloat(xVelocityText.c_str(), &object.velocity.x);
		ImGui::InputFloat(yVelocityText.c_str(), &object.velocity.y);

		ImGui::Text(massText.c_str());
		ImGui::InputFloat(massText.c_str(), &object.mass);

		ImGui::Text(scaleText.c_str());
		ImGui::InputFloat(xScaleText.c_str(), &object.scale.x);
		ImGui::InputFloat(yScaleText.c_str(), &object.scale.y);

// 		if (ImGui::Button(deleteText.c_str())) {
// 			env.removeBody(&object);
// 		}
	}
	
	void createAllObjectMenus(Environment& env) {
		int objectNumber = 1;
		for (Body& body : env.bodyList) {
			ImGui::Text(" ");
			createSingleObjectMenu(env, body, objectNumber);
			objectNumber++;
		}
	}
};

#endif