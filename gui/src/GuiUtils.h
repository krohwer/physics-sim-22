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
		std::string horizontalText = "Object " + std::to_string(objectNumber) + " Horizontal Position";
		std::string verticalText = "Object " + std::to_string(objectNumber) + " Vertical Position";
		std::string xText = "Object " + std::to_string(objectNumber) + " X Position";
		std::string yText = "Object " + std::to_string(objectNumber) + " Y Position";

		ImGui::Begin(title.c_str());
		ImGui::Text(horizontalText.c_str());
		ImGui::SliderFloat(xText.c_str(), &object.position.x, 0.5f, env.width - 0.5f);
		ImGui::Text(verticalText.c_str());
		ImGui::SliderFloat(yText.c_str(), &object.position.y, 0.5f, env.height - 0.5f);
		ImGui::End();
	}
};

#endif