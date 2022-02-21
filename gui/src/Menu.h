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

// Dimensions
#define ENVSETTINGS_WIDTH 300
#define ENVSETTINGS_HEIGHT 145
#define ENVSETTINGS_DIMENSIONS ImVec2(ENVSETTINGS_WIDTH, ENVSETTINGS_HEIGHT)
#define ENVSETTINGS_POSITION ImVec2((1600 - ENVSETTINGS_WIDTH) / 2, (900 - ENVSETTINGS_HEIGHT) / 2)

#define HELP_WIDTH 620
#define HELP_HEIGHT 470
#define HELP_DIMENSIONS ImVec2(HELP_WIDTH, HELP_HEIGHT)
#define HELP_POSITION ImVec2((1600 - HELP_WIDTH) / 2, (900 - HELP_HEIGHT) / 2)

#define ALERTMESSAGE_WIDTH 300
#define MIN_ALERTMESSAGE_HEIGHT 103
#define MAX_ALERTMESSAGE_HEIGHT 118

#define MIN_CONTROLPANEL_WIDTH 265
#define MIN_CONTROLPANEL_HEIGHT 400

// Colors
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

// Other
#define ITEM_SPACING ImVec2(0.0f, 2.5f)
#define CONTROLPANEL_ERRORMESSAGE "Cannot use Control Panel when simulator is running."

struct Menu {
public:
	// TODO: COMMENTS
	Environment *env;
	StorageManager *storage;
	Camera *camera;

	bool *isPhysicsActive;
	bool *hasSimStarted;

	float *frameStart;
	float *startTime;

	ImFont* fontLarge;
	ImFont* fontMedium;
	ImFont* fontSmall;

	bool activateEnvironmentWindow;
	bool activateHelpWindow;
	bool activateErrorAlert;
	std::string errorMessage;
	float errorAlertHeight;

	unsigned short deleteObject;
	int highlight;

	Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime);

	void initializeStyle();
	
	void createSimulatorManager();

	void createMenuBar();

	void createControlPanel();

	void cleanUp();

private:
	// TODO: COMMENTS
	std::list <HelpItem> helpItems;
	std::list<HelpItem>::iterator itemDisplayed;
	int index;

	void createAllObjectMenus();

	void makeAlert(std::string windowName, std::string alertMessage, float alertWindowHeight);
		
	void environmentMenu();

	void helpWindow();

	void exitPopupButton(std::string buttonLabel);

	void createSingleObjectMenu(Body& object, int objectNumber);

	void disableCameraIfFocused();

	void deleteButtonStyling();

	void popMultipleStyleColors(int amountToPop);

};

#endif