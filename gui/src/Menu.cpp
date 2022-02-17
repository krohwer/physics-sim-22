#include "pch.h"
#include "Menu.h"
#include "ExperimentManager.h"
#include <GLFW/glfw3.h>

Menu::Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime) {
	this->env = env;
	this->storage = storage;
	this->camera = camera;
	this->isPhysicsActive = isPhysicsActive;
	this->hasSimStarted = hasSimStarted;
	this->frameStart = frameStart;
	this->startTime = startTime;

	ImGuiIO& io = ImGui::GetIO();
	fontLarge = io.Fonts->AddFontFromFileTTF("resources/fonts/Rubik-Medium.ttf", 20.0f);
	fontMedium = io.Fonts->AddFontFromFileTTF("resources/fonts/Rubik-Medium.ttf", 15.0f);
	fontSmall = io.Fonts->AddFontFromFileTTF("resources/fonts/Rubik-Medium.ttf", 13.0f);

	activateErrorAlert = false;
	activateEnvironmentWindow = false;
	activateHelpWindow = false;
	errorMessage = "ERROR OCCURRED";
	deleteObject = 0;
	highlight = -1;
}

void Menu::initializeStyle() {	
	ImVec4* colors = ImGui::GetStyle().Colors;
	
	colors[ImGuiCol_MenuBarBg]            = DARK_GRAY_SOLID;
	
	colors[ImGuiCol_TitleBg]              = DARK_GRAY_SOLID;
	colors[ImGuiCol_TitleBgActive]        = LIGHT_BLUE;
	colors[ImGuiCol_TitleBgCollapsed]     = DARK_GRAY_SOLID;
	colors[ImGuiCol_WindowBg]             = MID_GRAY_T;

	colors[ImGuiCol_PopupBg]              = DARK_GRAY_T;
	colors[ImGuiCol_ModalWindowDimBg]     = DARK_T;
	colors[ImGuiCol_FrameBg]              = MID_GRAY_T;
	colors[ImGuiCol_FrameBgActive]        = LIGHT_BLUE;
	colors[ImGuiCol_FrameBgHovered]       = LIGHTEST_BLUE;
	
	colors[ImGuiCol_Tab]                  = DARK_GRAY_SOLID;
	colors[ImGuiCol_TabActive]            = LIGHT_BLUE;
	colors[ImGuiCol_TabHovered]           = LIGHTEST_BLUE;

	colors[ImGuiCol_Button]               = LIGHT_BLUE;
	colors[ImGuiCol_ButtonHovered]        = LIGHTEST_BLUE;
	colors[ImGuiCol_ButtonActive]         = LIGHTEST_BLUE;

	colors[ImGuiCol_ScrollbarBg]          = MID_GRAY_T;
	colors[ImGuiCol_ScrollbarGrab]        = LIGHT_BLUE;
	colors[ImGuiCol_ScrollbarGrabHovered] = LIGHTEST_BLUE;
	colors[ImGuiCol_ScrollbarGrabActive]  = LIGHTEST_BLUE;
}

void Menu::createMenuBar() {
	// make a little taller
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 6.0f));
	if (ImGui::BeginMainMenuBar()) {

		disableCameraIfFocused();

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

		if (ImGui::BeginMenu("Experiments")) {
			if (ImGui::MenuItem("Open")) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					std::string filepath = Experiment::openFile("Kinetic Labs Experiment (*.klx)\0*.klx\0");
					if (!filepath.empty()) {
						env->bodyList.clear();
						storage->clear();
						Experiment::load(*env, *camera, filepath);
					}
				}
				else {
					activateErrorAlert = true;
					errorMessage = "Cannot load experiments at this time.";
				}
			} // end of Open Experiment Menu Item
			if (ImGui::MenuItem("Save As...")) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					std::string filepath = Experiment::saveFile("Kinetic Labs Experiment (*.klx)\0*.klx\0");
					if (!filepath.empty()) {
						Experiment::save(*env, *camera, filepath);
					}
				}
				else {
					activateErrorAlert = true;
					errorMessage = "Cannot save experiments at this time.";
				}
			} // end of Save Experiment Menu Item

			ImGui::Separator();

			if (ImGui::BeginMenu("Sample Experiments")) {
				if (ImGui::MenuItem("Classic Projectile Motion")) {
					if (!*isPhysicsActive && !*hasSimStarted) {
						env->bodyList.clear();
						storage->clear();
						std::string filepath = "Experiments/ProjectileMotionSample.klx";
						Experiment::load(*env, *camera, filepath);
					}
					else {
						activateErrorAlert = true;
						errorMessage = "Cannot load experiment at this time.";
					}
				}
				ImGui::EndMenu();
			} // end of Sample Experiments Menu Item
			ImGui::EndMenu();
		} // end of Experiments Menu

		if (ImGui::BeginMenu("Environment")) {
			if (ImGui::MenuItem("Settings...")) {
				if (!*isPhysicsActive && !*hasSimStarted)
					activateEnvironmentWindow = true;
				else {
					activateErrorAlert = true;
					errorMessage = "Cannot edit Environment at this time.";
				}
			}
			ImGui::EndMenu();
		} // end of Environment Window in Menu Bar


		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Getting Started"))
				activateHelpWindow = true;
			ImGui::EndMenu();
		} // end of Help Menu Item

		ImGui::EndMainMenuBar();
	} // end of Main Menu Bar
	ImGui::PopStyleVar();

	if (activateEnvironmentWindow)
		ImGui::OpenPopup("Environment Settings");
	if (activateHelpWindow)
		ImGui::OpenPopup("Getting Started");

	environmentMenu();
	helpWindow();

}  // end of createMenuBar()

void Menu::createControlPanel() {
	ImGui::SetNextWindowSize(ImVec2(MIN_CONTROLPANEL_WIDTH, MIN_CONTROLPANEL_HEIGHT));
	ImGui::SetNextWindowPos(ImVec2(1600 - MIN_CONTROLPANEL_WIDTH, 27));

	ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_NoResize);

	disableCameraIfFocused();

	if (ImGui::BeginTabBar("Control Panel Tabs")) {
		if (ImGui::BeginTabItem("Object Manager")) {
			// Creates an object at the center of the screen
			if (ImGui::Button("Create Box", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					float xPosition = camera->cPosition.x / PIXEL_RATIO;
					float yPosition = camera->cPosition.y / PIXEL_RATIO;
					env->addBody(0.0f, xPosition, yPosition);
				}
				else {
					activateErrorAlert = true;
					errorMessage = CONTROLPANEL_ERRORMESSAGE;
				}
			} // end of Create Box button

			// Creates an object at the center of the screen
			if (ImGui::Button("Create Ball", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					float xPosition = camera->cPosition.x / PIXEL_RATIO;
					float yPosition = camera->cPosition.y / PIXEL_RATIO;
					env->addBody(1.0f, xPosition, yPosition);
				}
				else {
					activateErrorAlert = true;
					errorMessage = CONTROLPANEL_ERRORMESSAGE;
				}
			} // end of Create Ball button

			deleteButtonStyling();
			// Removes all objects in the current environment
			if (ImGui::Button("Delete All Objects", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
				if (!*isPhysicsActive && !*hasSimStarted) {
					env->bodyList.clear();
					storage->clear();
				}
				else {
					activateErrorAlert = true;
					errorMessage = CONTROLPANEL_ERRORMESSAGE;
				}
			} // end of Delete All Objects button

			ImGui::PushStyleColor(ImGuiCol_Text, DARK_SOLID);
			createAllObjectMenus();
			popMultipleStyleColors(5);

			ImGui::EndTabItem();
		} // end of Object Manager tab item
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

		disableCameraIfFocused();

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
	ImGui::PushStyleColor(ImGuiCol_PopupBg, MID_GRAY_SOLID);
	ImGui::PushFont(fontLarge);

	if (ImGui::BeginPopupModal("Environment Settings", NULL, ImGuiWindowFlags_NoResize)) {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK_SOLID);
		ImGui::PushFont(fontMedium);
		ImGui::InputFloat("Width (m)", &env->width);
		ImGui::InputFloat("Height (m)", &env->height);
		ImGui::InputFloat("Gravity", &env->gravity);

		if (env->width < 0)
			env->width = 0;
		if (env->height < 0)
			env->height = 0;
		env->computeAxes();

		ImGui::Dummy(ITEM_SPACING);
		ImGui::PopStyleColor();
		exitPopupButton("CONFIRM");
		ImGui::PopFont();
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor();
	ImGui::PopFont();
}

void Menu::helpWindow() {
	ImVec2 menuDimensions = { HELP_WIDTH , HELP_HEIGHT };
	ImVec2 menuPosition = { (1600 - HELP_WIDTH) / 2 , (900 - HELP_HEIGHT) / 2 };

	ImGui::SetNextWindowSize(menuDimensions);
	ImGui::SetNextWindowPos(menuPosition);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, MID_GRAY_SOLID);
	ImGui::PushFont(fontLarge);

	if (ImGui::BeginPopupModal("Getting Started", NULL, ImGuiWindowFlags_NoResize)) {
		ImGui::PushStyleColor(ImGuiCol_Text, DARK_SOLID);

		ImGui::Text("Camera");
		ImGui::PushFont(fontMedium);
		ImGui::TextWrapped("	Use the WASD or arrow keys to move the environment field of view.");
		ImGui::TextWrapped("	Use the mouse wheel to zoom in and out.");
		ImGui::PopFont();

		ImGui::Dummy(ITEM_SPACING);
		ImGui::Text("Control Panel");
		ImGui::PushFont(fontMedium);
		ImGui::TextWrapped("	Press the 'Create Object' button to create an object at the center of the screen.");
		ImGui::TextWrapped("	Press the 'Delete All Objects' button to clear all objects on the screen.");
		ImGui::PopFont();
		
		ImGui::Dummy(ITEM_SPACING);
		ImGui::Text("Object Manager");
		ImGui::PushFont(fontMedium);
		ImGui::TextWrapped("	X Position: changes the horizontal position of the object in meters.");
		ImGui::TextWrapped("	Y Position: changes the vertical position of the object in meters.");
		ImGui::TextWrapped("	Speed: changes the initial speed of the objects in meters per second.");
		ImGui::TextWrapped("	Angle: changes the angle of the object in degrees.");
		ImGui::PopFont();
		
		ImGui::Dummy(ITEM_SPACING);
		ImGui::Text("Experiments");
		ImGui::PushFont(fontMedium);
		ImGui::TextWrapped("	Open any previously made Kinetics Lab experiments.");
		ImGui::TextWrapped("	Save the currently set up experiment on screen.");
		ImGui::TextWrapped("	Open a sample experiment provided by the developers of Kinetics Lab.");
		ImGui::PopFont();

		ImGui::Dummy(ITEM_SPACING);
		ImGui::PopStyleColor();
		exitPopupButton("CLOSE");
		ImGui::EndPopup();
	}
	ImGui::PopFont();
	ImGui::PopStyleColor();
}

void Menu::exitPopupButton(std::string buttonLabel) {
	if (ImGui::Button(buttonLabel.c_str(), ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
		activateEnvironmentWindow = false;
		activateHelpWindow = false;
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
		ImGui::PushFont(fontSmall);
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

		ImGui::PopFont();
		ImGui::Dummy(ITEM_SPACING);
		
		float buttonWidth = ImGui::GetContentRegionAvailWidth() * 0.9f;	
		ImGui::PushStyleColor(ImGuiCol_Text, WHITE);
		if (ImGui::Button(deleteText.c_str(), ImVec2(buttonWidth, 0.0f))) {
			if (!*isPhysicsActive && !*hasSimStarted) {
				deleteObject = objectNumber;
			}
			else {
				activateErrorAlert = true;
				errorMessage = CONTROLPANEL_ERRORMESSAGE;
			}
		}
		ImGui::PopStyleColor();

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

void Menu::disableCameraIfFocused() {
	if (ImGui::IsWindowFocused() || activateEnvironmentWindow || activateHelpWindow) {
		camera->disabled = true;
	}
	else {
		camera->disabled = false;
	}
}

void Menu::deleteButtonStyling() {
	ImGui::PushStyleColor(ImGuiCol_Text, WHITE);
	ImGui::PushStyleColor(ImGuiCol_Button, RED);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, LIGHT_RED);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, LIGHT_RED);
}

void Menu::popMultipleStyleColors(int amountToPop) {
	while (amountToPop > 0) {
		ImGui::PopStyleColor();
		amountToPop--;
	}
}
