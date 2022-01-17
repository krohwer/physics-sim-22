#pragma once

#ifndef MENUS_H
#define MENUS_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GuiUtils.h"
#include "Environment.h"
#include "Shape.h"
#include "PhysicsObject.h"

#include <iostream>
#include <string>

namespace Menu {
	// This isn't supposed to be here, I know, but it's here for now
	void addUserObject(Environment& env, float xPosition, float yPosition) {
		// Cannot press this button if the simulation is running or paused
		Shape shape;
		Body object(&shape, xPosition, yPosition);
		env.addBody(&object);
	}

	void createAllObjectMenus(Environment& env) {
		int objectNumber = -1;
		for (Body& body : env.bodyList) {
			if (objectNumber > 0) {
				// Dummy is used for selective line padding
				ImGui::Dummy(ImVec2(0.0f, 2.5f));
				GuiUtils::createSingleObjectMenu(env, body, objectNumber);
			}
			objectNumber++;
		}
	}

	void makeAlert(std::string alertMessage) {
		if (ImGui::BeginPopupModal("ERROR")) {
			std::cout << "We hit the alert" << std::endl;
			ImGui::Text(alertMessage.c_str());

			if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
};

#endif