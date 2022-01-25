#pragma once

#ifndef GUI_UTILS_H
#define GUI_UTILS_H

#include "Environment.h"
#include "PhysicsObject.h"
#include "Menu.h"

#include <iostream>
#include <string>

#define ITEM_SPACING ImVec2(0.0f, 2.5f)
#define ENVSETTINGS_WIDTH 300
#define ENVSETTINGS_HEIGHT 130

namespace GuiUtils {

	int highlight;
	bool deleteObject;

	void exitPopupButton() {
		if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
	}

	void environmentMenu(Environment& env) {
		ImVec2 menuDimensions = { ENVSETTINGS_WIDTH , ENVSETTINGS_HEIGHT };
		ImVec2 menuPosition = { (1600 - ENVSETTINGS_WIDTH) / 2 , (900 - ENVSETTINGS_HEIGHT) / 2 };

		ImGui::SetNextWindowSize(menuDimensions);
		ImGui::SetNextWindowPos(menuPosition);

		if (ImGui::BeginPopupModal("Environment Settings", NULL, ImGuiWindowFlags_NoResize)) {

			ImGui::InputFloat("Width (m)", &env.width);
			ImGui::InputFloat("Height (m)", &env.height);
			ImGui::InputFloat("Gravity", &env.gravity);

			if (&env.width < 0)
				env.width = 0;
			if (&env.height < 0)
				env.height = 0;

			ImGui::Dummy(ITEM_SPACING);
			exitPopupButton();

			ImGui::EndPopup();
		}
	}

	void helpWindow() {
		ImVec2 menuDimensions = { ENVSETTINGS_WIDTH , ENVSETTINGS_HEIGHT };
		ImVec2 menuPosition = { (1600 - ENVSETTINGS_WIDTH) / 2 , (900 - ENVSETTINGS_HEIGHT) / 2 };

		ImGui::SetNextWindowSize(menuDimensions);
		ImGui::SetNextWindowPos(menuPosition);

		if (ImGui::BeginPopupModal("Help", NULL, ImGuiWindowFlags_NoResize)) {

			ImGui::TextWrapped("Stuff here");

			ImGui::Dummy(ITEM_SPACING);

			ImGui::TextWrapped("More stuff");

			exitPopupButton();

			ImGui::EndPopup();
		}
	}

	void createSingleObjectMenu(Environment& env, Body& object, int objectNumber) {
		// This may seem excessive, but ImGUI uses the title of the menu as its ID
		// So we need to create unique titles for each object and each attribute
		// Users would likely want to differentiate between the objects as well
		std::string title = "Object " + std::to_string(objectNumber);
		std::string xPositionText = "X Position (m)";
		std::string yPositionText = "Y Position (m)";
		std::string SpeedText = "Speed (m/s)";
		std::string DirectionText = "Direction (deg)";
		std::string massText = "Mass (kg)";
		std::string xScaleText = "Width (m)";
		std::string yScaleText = "Height (m)";
		std::string deleteText = "Delete Object";

		// TODO: Allow objects to be collapsible menu toggles
		if (ImGui::TreeNode(title.c_str())) {

			if (ImGui::IsItemFocused())
				highlight = objectNumber;

			// Allows us to make the width smaller for the input fields
			// Scales to be a third of the size of the window, but no less than 100 px
			if (ImGui::GetWindowWidth() / 4.0f < 75.0f)
				ImGui::PushItemWidth(75.0f);
			else
				ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4.0f);

			// Dummy is used for selective line padding

			ImGui::InputFloat(xPositionText.c_str(), &object.position.x);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;
			ImGui::InputFloat(yPositionText.c_str(), &object.position.y);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;

			ImGui::Dummy(ITEM_SPACING);
			ImGui::InputFloat(SpeedText.c_str(), &object.vSpeed);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;
			ImGui::InputFloat(DirectionText.c_str(), &object.vDirection);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;

			ImGui::Dummy(ITEM_SPACING);
			ImGui::InputFloat(massText.c_str(), &object.mass);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;

			ImGui::Dummy(ITEM_SPACING);
			ImGui::InputFloat(xScaleText.c_str(), &object.scale.x);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;
			ImGui::InputFloat(yScaleText.c_str(), &object.scale.y);
			if (ImGui::IsItemFocused())
				highlight = objectNumber;

			ImGui::Dummy(ITEM_SPACING);
			// TODO: Find a work around to "reset" the whole menu after an object is deleted
			float buttonWidth = ImGui::GetContentRegionAvailWidth() * 0.9f;
			if (ImGui::Button(deleteText.c_str(), ImVec2(buttonWidth, 0.0f))) {
				deleteObject = true;
			}

			// Boundary checking for x position input
			if (object.position.x < 0.5f * object.scale.x)
				object.position.x = 0.5f * object.scale.x;
			// Boundary checking for y position input
			if (object.position.y < 0.5f * object.scale.y)
				object.position.y = 0.5f * object.scale.y;

			ImGui::TreePop();
		}
		else if (highlight == objectNumber)
			highlight = -1;
	}
};

#endif