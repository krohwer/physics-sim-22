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

	activateEnvironmentWindow = false;
	activateHelpWindow = false;
	activateErrorAlert = false;
	errorMessage = "ERROR OCCURRED";
	errorAlertHeight = 0.0f;

	deleteObject = 0;
	highlight = -1;

	// setting up and appending all help items for help menu
	HelpItem camControls = HelpItem("resources/help-images/controls.png", "Camera", "\tUse the WASD or arrow keys to move the environment field of view.\n\tUse the mouse wheel to zoom in and out.");
	HelpItem controlPanel = HelpItem("resources/help-images/cp_buttons.png", "Control Panel", "\tPress the 'Create Object' button to create an object at the center of the screen.\n\tPress the 'Delete All Objects' button to clear all objects on the screen.");
	HelpItem objManager = HelpItem("resources/help-images/obj_menu.png", "Object Manager", "\tX Position: changes the horizontal position of the object in meters.\n\tY Position: changes the vertical position of the object in meters.\n\tSpeed: changes the initial speed of the objects in meters per second.\n\tAngle: changes the angle of the object in degrees.\n\tMass: changes the mass of the object in kilograms.\n\tWidth: changes the width of the object in meters.\n\tHeight: changes the height of the object in meters.");
	HelpItem envSettings = HelpItem("resources/help-images/env_window.png", "Environment Settings", "\tAdjust the width and or height of the Environment in meters.\n\tAdjust the rate of gravity of the Environment.");
	HelpItem expManager = HelpItem("resources/help-images/exp_options.png", "Experiments", "\tOpen any previously made Kinetics Lab experiments.\n\tSave the currently set up experiment on screen.\n\tOpen a sample experiment provided by the developers of Kinetics Lab.");
	HelpItem simManager = HelpItem("resources/help-images/sim_buttons.png", "Simulator Manager", "\tClick 'Play' to begin the simulation.\n\tClick 'Pause' to pause the simulation.\n\tClick 'Reset' to set the simulation back to its starting setup.");

	helpItems.push_back(camControls);
	helpItems.push_back(controlPanel);
	helpItems.push_back(objManager);
	helpItems.push_back(envSettings);
	helpItems.push_back(expManager);
	helpItems.push_back(simManager);

	itemDisplayed = helpItems.begin();
	index = 0;
}

void Menu::initializeStyle() {	
	ImVec4* colors = ImGui::GetStyle().Colors;
	
	colors[ImGuiCol_MenuBarBg]            = DARK_GRAY_SOLID;
	
	colors[ImGuiCol_TitleBg]              = DARK_GRAY_SOLID;
	colors[ImGuiCol_TitleBgActive]        = LIGHT_BLUE;
	colors[ImGuiCol_TitleBgCollapsed]     = DARK_GRAY_SOLID;
	colors[ImGuiCol_WindowBg]             = MID_GRAY_T;

	colors[ImGuiCol_PopupBg]              = DARK_GRAY_T;
	colors[ImGuiCol_ModalWindowDimBg]     = WHITE_T;
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
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 8.0f));
	if (ImGui::BeginMainMenuBar()) {

		disableCameraIfFocused();

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
					errorAlertHeight = MIN_ALERTMESSAGE_HEIGHT;
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
					errorAlertHeight = MIN_ALERTMESSAGE_HEIGHT;
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
						errorAlertHeight = MIN_ALERTMESSAGE_HEIGHT;
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
					errorAlertHeight = MIN_ALERTMESSAGE_HEIGHT;
				}
			}
			ImGui::EndMenu();
		} // end of Environment Window in Menu Bar


		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Getting Started"))
				if(!*isPhysicsActive && !*hasSimStarted)
					activateHelpWindow = true;
				else {
					activateErrorAlert = true;
					errorMessage = "Cannot access at this time.";
					errorAlertHeight = MIN_ALERTMESSAGE_HEIGHT;
				}
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

void Menu::createSimulatorManager() {
	ImGui::PushFont(fontLarge);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 150.0f));
	ImGui::SetNextWindowPos(ImVec2((1600 - 200.0f) / 2, 31.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 3.5f));
	ImGui::Begin("Simulator Manager", NULL, ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoBackground);

	if (!*isPhysicsActive) {
		if (ImGui::Button("Play", ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 5.0f, 35.0f))) {
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
		if (ImGui::Button("Pause", ImVec2(ImGui::GetContentRegionAvailWidth() / 2 - 5.0f, 35.0f))) {
			// Pauses simulation if it's running
			*isPhysicsActive = false;
		}
	} // end of Play / Pause menu item

	ImGui::SameLine();

	// Stops and resets objects to their initial state at the start of the simulation
	if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionAvailWidth(), 35.0f))) {
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
	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}

void Menu::createControlPanel() {
	ImGui::SetNextWindowSize(ImVec2(MIN_CONTROLPANEL_WIDTH, MIN_CONTROLPANEL_HEIGHT), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(1600 - MIN_CONTROLPANEL_WIDTH, 31.0f), ImGuiCond_FirstUseEver);

	ImGui::Begin("Control Panel");

	disableCameraIfFocused();

	// Creates an object at the center of the screen
	if (ImGui::Button("Create Object", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
		if (!*isPhysicsActive && !*hasSimStarted) {
			float xPosition = camera->cPosition.x / PIXEL_RATIO;
			float yPosition = camera->cPosition.y / PIXEL_RATIO;
			env->addBody(xPosition, yPosition);
		}
		else {
			activateErrorAlert = true;
			errorMessage = CONTROLPANEL_ERRORMESSAGE;
			errorAlertHeight = MAX_ALERTMESSAGE_HEIGHT;
		}
	} // end of Create Object button

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
			errorAlertHeight = MAX_ALERTMESSAGE_HEIGHT;
		}
	} // end of Delete All Objects button

	ImGui::PushStyleColor(ImGuiCol_Text, BLACK);
	createAllObjectMenus();
	popMultipleStyleColors(4);

	ImGui::End();
} // end of createControlPanel

void Menu::createAllObjectMenus() {
	int objectNumber = 1;
	for (Body& body : env->bodyList) {
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

	makeAlert("ERROR", errorMessage, errorAlertHeight);

	if (deleteObject > 0) {
		std::list<Body>::iterator i = env->bodyList.begin();
		std::advance(i, deleteObject - 1);
		env->bodyList.erase(i);
		deleteObject = 0;
	}
}

// PRIVATE HELPER FUNCTIONS

void Menu::makeAlert(std::string windowName, std::string alertMessage, float alertWindowHeight) {
	ImGui::SetNextWindowSize(ImVec2(ALERTMESSAGE_WIDTH, alertWindowHeight));
	ImGui::SetNextWindowPos(ImVec2((1600 - ALERTMESSAGE_WIDTH) / 2, (900 - alertWindowHeight) / 2));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, MID_GRAY_SOLID);

	if (ImGui::BeginPopupModal(windowName.c_str(), NULL, ImGuiWindowFlags_NoResize)) {

		disableCameraIfFocused();

		ImGui::PushStyleColor(ImGuiCol_Text, BLACK);
		ImGui::TextWrapped("Simulator is active.");
		ImGui::TextWrapped(alertMessage.c_str());
		ImGui::PopStyleColor();

		ImGui::Dummy(ITEM_SPACING);
		if (ImGui::Button("OK", ImVec2(ImGui::GetContentRegionAvailWidth(), 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
}

void Menu::environmentMenu() {
	ImGui::SetNextWindowSize(ENVSETTINGS_DIMENSIONS);
	ImGui::SetNextWindowPos(ENVSETTINGS_POSITION);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, MID_GRAY_SOLID);
	ImGui::PushFont(fontLarge);

	if (ImGui::BeginPopupModal("Environment Settings", NULL, ImGuiWindowFlags_NoResize)) {
		ImGui::PushStyleColor(ImGuiCol_Text, BLACK);
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
	ImGui::SetNextWindowSize(HELP_DIMENSIONS);
	ImGui::SetNextWindowPos(HELP_POSITION);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, MID_GRAY_SOLID);
	ImGui::PushFont(fontLarge);

	if (ImGui::BeginPopupModal("Getting Started", NULL, ImGuiWindowFlags_NoResize)) {
		ImGui::PushStyleColor(ImGuiCol_Text, BLACK);

		int my_image_width = 0;
		int my_image_height = 0;
		GLuint my_image_texture = 0;
		bool ret = itemDisplayed->LoadTextureFromFile(itemDisplayed->imgPath.c_str(), &my_image_texture, &my_image_width, &my_image_height);

		ImGui::Dummy(ITEM_SPACING);
		ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));

		ImGui::Dummy(ITEM_SPACING);
		ImGui::Text(itemDisplayed->title.c_str());

		ImGui::Dummy(ITEM_SPACING);
		ImGui::PushFont(fontMedium);
		ImGui::TextWrapped(itemDisplayed->description.c_str());

		ImGui::PopFont();
		ImGui::PopStyleColor();

		if (index == 0 || index == 1 || index == 3)
			ImGui::Dummy(ImVec2(0.0f, 77.0f));
		else if (index == 4 || index == 5)
			ImGui::Dummy(ImVec2(0.0f, 62.0f));
		else
			ImGui::Dummy(ITEM_SPACING);

		if (index > 0) {
			float width = ImGui::GetContentRegionAvailWidth();

			if (index != helpItems.size() - 1)
				width = width / 2 - 5.0f;

			if (ImGui::Button("PREVIOUS", ImVec2(width, 0.0f))) {
				index--;
				itemDisplayed = helpItems.begin();
				std::advance(itemDisplayed, index);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			if (index != helpItems.size() - 1)
				ImGui::SameLine();
		}

		if (index < helpItems.size() - 1) {
			float width = ImGui::GetContentRegionAvailWidth();

			if (ImGui::Button("NEXT", ImVec2(width, 0.0f))) {
				index++;
				itemDisplayed = helpItems.begin();
				std::advance(itemDisplayed, index);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		ImGui::Dummy(ITEM_SPACING);
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
		// Scales to be a third of the size of the window, but no less than 100 pixels
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
				errorAlertHeight = MAX_ALERTMESSAGE_HEIGHT;
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
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, LIGHT_RED);
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, LIGHT_RED);
}

void Menu::popMultipleStyleColors(int amountToPop) {
	while (amountToPop > 0) {
		ImGui::PopStyleColor();
		amountToPop--;
	}
}
