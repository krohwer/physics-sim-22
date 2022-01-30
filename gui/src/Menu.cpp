#include "pch.h"
#include "Menu.h"
#include <GLFW/glfw3.h>

Menu::Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime) {
	this->env = env;
	this->storage = storage;
	this->camera = camera;
	this->isPhysicsActive = isPhysicsActive;
	this->hasSimStarted = hasSimStarted;
	this->frameStart = frameStart;
	this->startTime = startTime;

	activateErrorAlert = false;
	errorMessage = "ERROR OCCURRED";
	deleteObject = 0;
}

void Menu::createMenuBar() {
	bool activateEnvironmentWindow = false;
	bool activateHelpWindow = false;

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Environment")) {
			if (ImGui::MenuItem("Configure Environment Settings")) {
				if (!*isPhysicsActive && !*hasSimStarted)
					activateEnvironmentWindow = true;
				else {
					activateErrorAlert = true;
					errorMessage = "Cannot edit Environment at this time.";
				}
			}
			ImGui::EndMenu();
		} // end of Environment Window in Menu Bar

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

		if (!*isPhysicsActive) {
			if (ImGui::MenuItem("Play")) {
				if (!(env->bodyList.empty())) {
					// initializing axes and storing starting positions if this is the first time the Simulator starts
					if (!*hasSimStarted) {

						// initialize the axes
						env->xAxis.init();
						env->yAxis.init();

						// Physics pre-calculations
						for (Body& body : env->bodyList) {
							// save object starting positions and velocities
							storage->save(body);

							// while we're looping objects, go ahead and recalculate some important values
							body.init();
							// calculate the force of gravitation for the object into a vector and apply it
							glm::vec3 gravity(0, body.mass * env->gravity, 0);
							body.force -= gravity;
						}
						env->generatePairs();
						*hasSimStarted = true;
					} // end of hasSimStarted check

					// Physics happens here
					*frameStart = glfwGetTime();
					*startTime = glfwGetTime();
					
					*isPhysicsActive = true;
				}
			}
		}
		else {
			if (ImGui::MenuItem("Pause")) {
				// Pauses simulation if it's running
				*isPhysicsActive = false;
			}
		} // end of Play / Pause menu item

		// Stops and resets objects to their initial state at the start of the simulation
		if (ImGui::MenuItem("Reset")) {
			if (*isPhysicsActive || *hasSimStarted) {
				int count = 0;
				for (Body& body : env->bodyList) {
					storage->restore(body, count);
					// clear forces!
					body.force = glm::vec3(0.0f);

					count++;
				}
				storage->clear();
				*isPhysicsActive = false;
				*hasSimStarted = false;
			}
		} // end of Reset menu item

		if (ImGui::MenuItem("Help")) {
			// TODO: Implement actual Help Window
			activateHelpWindow = true;
		} // end of Help Menu Item

		ImGui::EndMainMenuBar();
	} // end of Main Menu Bar

	if (activateEnvironmentWindow)
		ImGui::OpenPopup("Environment Settings");
	if (activateHelpWindow)
		ImGui::OpenPopup("Help");

	environmentMenu();
	helpWindow();

}  // end of createMenuBar()

void Menu::createControlPanel() {
	std::string controlPanelErrorMessage = "Cannot use Control Panel when simulator is running.";
	ImGui::SetNextWindowSize(ImVec2(MIN_CONTROLPANEL_WIDTH, MIN_CONTROLPANEL_HEIGHT), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(1600 - MIN_CONTROLPANEL_WIDTH, 50), ImGuiCond_FirstUseEver);

	ImGui::Begin("Control Panel");

	if (ImGui::BeginTabBar("Control Panel Tabs")) {
		if (ImGui::BeginTabItem("Object Manager")) {
			// Creates an object at the center of the screen
			if (ImGui::Button("Create Object", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					float xPosition = camera->cPosition.x / PIXEL_RATIO;
					float yPosition = camera->cPosition.y / PIXEL_RATIO;
					env->addBody(xPosition, yPosition);
				}
				else {
					activateErrorAlert = true;
					errorMessage = controlPanelErrorMessage;
				}
			} // end of Create Object button

			// Removes all objects in the current environment
			if (ImGui::Button("Delete All Objects", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					env->bodyList.clear();
					storage->clear();
				}
				else {
					activateErrorAlert = true;
					errorMessage = controlPanelErrorMessage;
				}
			} // end of Delete All Objects button

			createAllObjectMenus();
			ImGui::EndTabItem();
		} // end of Object Manager tab item

		if (ImGui::BeginTabItem("Experiments")) {
			if (ImGui::TreeNode("Classic Projectile Motion")) {
				ImGui::TextWrapped("There are two boxes. They're exactly the same and held at the same height.");
				ImGui::TextWrapped("One falls, the other is slightly pushed to the right when it falls. Which box touches the ground first?");

				if (ImGui::Button("Load Experiment", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
					env->bodyList.clear();
					storage->clear();

					float obj1xPosition = (800 - 100.0f) / PIXEL_RATIO;
					float obj2xPosition = (800 + 100.0f) / PIXEL_RATIO;
					float yPosition = 450 / PIXEL_RATIO;

					env->addBody(obj1xPosition, yPosition);
					Body* object2 = env->addBody(obj2xPosition, yPosition);
					object2->vSpeed = 5.0f;
				}
				ImGui::TreePop();
			} // end of hard coded Classic Projectile Motion
			ImGui::EndTabItem();
		} // end of Experiments tab item
	}
	ImGui::EndTabBar();
	ImGui::End();
} // end of createControlPanel

void Menu::createAllObjectMenus() {
	int objectNumber = 1;
	for (Body& body : env->bodyList) {
		// Dummy is used for selective line padding
		ImGui::Dummy(ITEM_SPACING);
		createSingleObjectMenu(body, objectNumber);
		objectNumber++;
	}
}

void Menu::cleanUp() {
	if (activateErrorAlert) {
		ImGui::OpenPopup("ERROR");
		activateErrorAlert = false;
	}

	makeAlert("ERROR", errorMessage);

	if (deleteObject > 0) {
		std::list<Body>::iterator i = env->bodyList.begin();
		std::advance(i, deleteObject - 1);
		env->bodyList.erase(i);
		deleteObject = 0;
	}
}

// PRIVATE HELPER FUNCTIONS

void Menu::makeAlert(std::string windowName, std::string alertMessage) {
	ImVec2 menuDimensions = { ALERTMESSAGE_WIDTH , ALERTMESSAGE_HEIGHT };
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

void Menu::environmentMenu() {
	ImVec2 menuDimensions = { ENVSETTINGS_WIDTH , ENVSETTINGS_HEIGHT };
	ImVec2 menuPosition = { (1600 - ENVSETTINGS_WIDTH) / 2 , (900 - ENVSETTINGS_HEIGHT) / 2 };

	ImGui::SetNextWindowSize(menuDimensions);
	ImGui::SetNextWindowPos(menuPosition);

	if (ImGui::BeginPopupModal("Environment Settings", NULL, ImGuiWindowFlags_NoResize)) {

		ImGui::InputFloat("Width (m)", &env->width);
		ImGui::InputFloat("Height (m)", &env->height);
		ImGui::InputFloat("Gravity", &env->gravity);

		if (env->width < 0)
			env->width = 0;
		if (env->height < 0)
			env->height = 0;

		ImGui::Dummy(ITEM_SPACING);
		exitPopupButton();

		ImGui::EndPopup();
	}
}

void Menu::helpWindow() {
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

void Menu::exitPopupButton() {
	if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
		ImGui::CloseCurrentPopup();
	}
}

void Menu::createSingleObjectMenu(Body& object, int objectNumber) {
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

		// Allows us to make the width smaller for the input fields
		// Scales to be a third of the size of the window, but no less than 100 px
		if (ImGui::GetWindowWidth() / 4.0f < 75.0f)
			ImGui::PushItemWidth(75.0f);
		else
			ImGui::PushItemWidth(ImGui::GetWindowWidth() / 4.0f);

		// Dummy is used for selective line padding

		ImGui::InputFloat(xPositionText.c_str(), &object.position.x);
		ImGui::InputFloat(yPositionText.c_str(), &object.position.y);

		ImGui::Dummy(ITEM_SPACING);
		ImGui::InputFloat(SpeedText.c_str(), &object.vSpeed);
		ImGui::InputFloat(DirectionText.c_str(), &object.vDirection);

		ImGui::Dummy(ITEM_SPACING);
		ImGui::InputFloat(massText.c_str(), &object.mass);

		ImGui::Dummy(ITEM_SPACING);
		ImGui::InputFloat(xScaleText.c_str(), &object.scale.x);
		ImGui::InputFloat(yScaleText.c_str(), &object.scale.y);

		ImGui::Dummy(ITEM_SPACING);
		// TODO: Find a work around to "reset" the whole menu after an object is deleted
		float buttonWidth = ImGui::GetContentRegionAvailWidth() * 0.9f;
		if (ImGui::Button(deleteText.c_str(), ImVec2(buttonWidth, 0.0f))) {
			deleteObject = objectNumber;
		}

		// Boundary checking for x position input
		if (object.position.x < 0.5f * object.scale.x)
			object.position.x = 0.5f * object.scale.x;
		// Boundary checking for y position input
		if (object.position.y < 0.5f * object.scale.y)
			object.position.y = 0.5f * object.scale.y;

		ImGui::TreePop();
	}
}