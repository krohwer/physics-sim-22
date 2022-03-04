#pragma once

#include "pch.h"

#ifndef MENUS_H
#define MENUS_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Camera.h"
#include "StorageManager.h"
#include "Environment.h"
#include "Shape.h"
#include "PhysicsObject.h"
#include "HelpItem.h"

#include <iostream>

// DIMENSIONS //
// All dimensions are in terms of pixels/floats
#define ENVSETTINGS_WIDTH 300
#define ENVSETTINGS_HEIGHT 145
#define ENVSETTINGS_DIMENSIONS ImVec2(ENVSETTINGS_WIDTH, ENVSETTINGS_HEIGHT)
// Centers the window position based on the set window size
#define ENVSETTINGS_POSITION ImVec2((1600 - ENVSETTINGS_WIDTH) / 2, (900 - ENVSETTINGS_HEIGHT) / 2)

#define HELP_WIDTH 620
#define HELP_HEIGHT 470
#define HELP_DIMENSIONS ImVec2(HELP_WIDTH, HELP_HEIGHT)
// Centers the window position based on the set window size
#define HELP_POSITION ImVec2((1600 - HELP_WIDTH) / 2, (900 - HELP_HEIGHT) / 2)

#define ALERTMESSAGE_WIDTH 300
#define MIN_ALERTMESSAGE_HEIGHT 103
#define MAX_ALERTMESSAGE_HEIGHT 118

#define MIN_CONTROLPANEL_WIDTH 265
#define MIN_CONTROLPANEL_HEIGHT 400

// COLORS //
// Colors are set based on a scale of 0.0 - 1.0
// To turn an RGA scale to this, simply divide the value of the color in RGB by 255
// _SOLID signifies a color with an alpha of 1.0
// _T signifies a color with an alpha < 1.0
// Colors without the above indicators are SOLID
#define WHITE ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
#define WHITE_T ImVec4(1.0f, 1.0f, 1.0f, 0.15f)
#define LIGHT_RED ImVec4(220.0f / 255.0f, 24.0f / 255.0f, 17.0f / 255.0f, 1.0f)

#define LIGHTEST_BLUE ImVec4(71.0f / 255.0f, 158.0f / 255.0f, 215.0f / 255.0f, 1.0f)
#define LIGHT_BLUE ImVec4(41.0f / 255.0f, 128.0f / 255.0f, 185.0f / 255.0f, 1.0f)
#define BLUE ImVec4(37.0f / 255.0f, 64.0f / 255.0f, 90.0f / 255.0f, 1.0f)

#define MID_GRAY_SOLID ImVec4(223.0f / 255.0f, 223.0f / 255.0f, 236.0f / 255.0f, 1.0f)
#define MID_GRAY_T ImVec4(0.5f, 0.5f, 0.7f, 0.25f)

#define DARK_GRAY_SOLID ImVec4(0.15f, 0.15f, 0.2f, 1.0f)
#define DARK_GRAY_T ImVec4(0.15f, 0.15f, 0.15f, 0.95f)

#define BLACK ImVec4(0.0f, 0.0f, 0.0f, 1.0f)

// OTHER //
#define ITEM_SPACING ImVec2(0.0f, 2.5f)
#define CONTROLPANEL_ERRORMESSAGE "Cannot use Control Panel when simulator is running."

struct Menu {
public:
	// Setting pointers to necessary actors that are managed by the menu
	Environment *env;
	StorageManager *storage;
	Camera *camera;

	// Pointers to necessary checks to restrict menu use
	bool *isPhysicsActive;
	bool *hasSimStarted;

	// Pointers to necessary simulator timers
	float *frameStart;
	float *startTime;

	// Pointers to the necessary font style of the menu
	ImFont* fontLarge;
	ImFont* fontMedium;
	ImFont* fontSmall;

	// Popup module managers and error variables
	bool activateEnvironmentWindow;
	bool activateHelpWindow;
	bool activateErrorAlert;
	std::string errorMessage;
	float errorAlertHeight;

	// Object management variables
	unsigned short deleteObject;
	int highlight;

	Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime);

	// Menu styling - can (and SHOULD) be called before rendering loop
	void initializeStyle();
	
	// Window with simulator manager buttons - pause/play
	void createSimulatorManager();

	// Menu Bar at the top of the application
	void createMenuBar();

	// Control Panel and object manager window
	void createControlPanel();

	// External necessary functions to update the GUI
	void cleanUp();

private:
	// Items managed within the menu
	std::list <HelpItem> helpItems;
	std::list<HelpItem>::iterator itemDisplayed;
	int index;

	// Object sub menu(s) in Control Panel
	void createAllObjectMenus();

	// Error management window
	void makeAlert(std::string windowName, std::string alertMessage, float alertWindowHeight);
	
	// Environment setting window
	void environmentMenu();

	// "Getting Started" window
	void helpWindow();

	// Reusable exit/close button for popup windows
	void exitPopupButton(std::string buttonLabel);

	// Inputs and labels of a single object
	void createSingleObjectMenu(Body& object, int objectNumber);

	// Camera management
	void disableCameraIfFocused();
	void disableCameraIfHovered();

	// Reusable delete styling for multiple delete buttons
	void deleteButtonStyling();

	// Avoids the multiple calls of "ImGui::PopStyleColor(...);" by simplifying
	//     it into one call
	void popMultipleStyleColors(int amountToPop);

};

#endif