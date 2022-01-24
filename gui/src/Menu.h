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

#define MIN_CONTROLPANEL_WIDTH 300
#define MIN_CONTROLPANEL_HEIGHT 400

#define ALERTMESSAGE_WIDTH 300
#define ALERTMESSAGE_HEIGHT 110

namespace Menu {

	std::string errorMessage;

	void makeAlert(std::string windowName, std::string alertMessage) {
		ImVec2 menuDimensions = { ALERTMESSAGE_WIDTH , ALERTMESSAGE_HEIGHT};
		ImVec2 menuPosition = { (1600 - ALERTMESSAGE_WIDTH) / 2 , (900 - ALERTMESSAGE_HEIGHT) / 2 };

		ImGui::SetNextWindowSize(menuDimensions);
		ImGui::SetNextWindowPos(menuPosition);

		if (ImGui::BeginPopupModal(windowName.c_str(), NULL, ImGuiWindowFlags_NoResize)) {
			ImGui::TextWrapped("Simulator is active.");
			ImGui::TextWrapped(alertMessage.c_str());
			ImGui::Dummy(ITEM_SPACING);
			if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void createMenuBar(Environment& env, bool isPhysicsActive, bool hasSimStarted) {
		bool activateEnvironmentWindow = false;
		/*bool activateExperimentLoader = false;*/
		bool activateHelpWindow = false;
		bool failedAccess = false;

		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("Environment")) {
				if (ImGui::MenuItem("Configure Environment Settings")) {
					if (!isPhysicsActive && !hasSimStarted)
						activateEnvironmentWindow = true;
					else {
						failedAccess = true;
						errorMessage = "Cannot edit Environment at this time.";
					}
				}
				ImGui::EndMenu();
			} // end of Environment Menu

			//if (ImGui::BeginMenu("Experiments")) {
			//	if (ImGui::MenuItem("Load Experiment")) {
			//		if (!isPhysicsActive && !hasSimStarted)
			//			activateExperimentLoader = true;
			//		else {
			//			failedAccess = true;
			//			errorMessage = "Cannot load experiments at this time.";
			//		}
			//	}
			//	ImGui::EndMenu();
			//} // end of Experiments Menu

			if (ImGui::MenuItem("Help")) {
				// TODO: Implement actual Help window
				activateHelpWindow = true;
			}

			ImGui::EndMainMenuBar();
		}

		if (activateEnvironmentWindow)
			ImGui::OpenPopup("Environment Settings");
		if (activateHelpWindow)
			ImGui::OpenPopup("Help");
		if (failedAccess)
			ImGui::OpenPopup("ERROR");

		GuiUtils::environmentMenu(env);
		GuiUtils::helpWindow();
		makeAlert("ERROR", errorMessage);
	}

	void createAllObjectMenus(Environment& env) {
		int objectNumber = 1;
		for (Body& body : env.bodyList) {
			// Dummy is used for selective line padding
			ImGui::Dummy(ITEM_SPACING);
			GuiUtils::createSingleObjectMenu(env, body, objectNumber);
			objectNumber++;
		}
	}
};

#endif