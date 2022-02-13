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

#include <iostream>
#include <string>

// Window and Item Sizing

#define ITEM_SPACING ImVec2(0.0f, 2.5f)

#define ENVSETTINGS_WIDTH 300
#define ENVSETTINGS_HEIGHT 130

#define ALERTMESSAGE_WIDTH 300
#define ALERTMESSAGE_HEIGHT 110

#define MIN_CONTROLPANEL_WIDTH 300
#define MIN_CONTROLPANEL_HEIGHT 400

// Colors

#define BLUE_SOLID ImVec4(0.0f, 0.15f, 0.235f, 1.0f)
#define MID_GRAY_SOLID ImVec4(0.2f, 0.2f, 0.2f, 1.0f)
#define MID_GRAY_T ImVec4(0.2f, 0.2f, 0.2f, 0.5f)

// Other

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

	bool activateErrorAlert;
	bool activateEnvironmentWindow;
	bool activateHelpWindow;
	std::string errorMessage;

	unsigned short deleteObject;
	int highlight;

	Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime);

	void initializeStyle();

	void createMenuBar();

	void createControlPanel();

	void cleanUp();

private:
	// TODO: COMMENTS
	void createAllObjectMenus();

	void makeAlert(std::string windowName, std::string alertMessage);
		
	void environmentMenu();

	void helpWindow();

	void exitPopupButton();

	void createSingleObjectMenu(Body& object, int objectNumber);

	void disableCameraIfFocused();

};

#endif