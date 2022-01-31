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

#define ITEM_SPACING ImVec2(0.0f, 2.5f)

#define ENVSETTINGS_WIDTH 300
#define ENVSETTINGS_HEIGHT 130

#define ALERTMESSAGE_WIDTH 300
#define ALERTMESSAGE_HEIGHT 110

#define MIN_CONTROLPANEL_WIDTH 300
#define MIN_CONTROLPANEL_HEIGHT 400

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
	std::string errorMessage;

	unsigned short deleteObject;
	int highlight;

	Menu(Environment* env, StorageManager* storage, Camera* camera, bool* isPhysicsActive, bool* hasSimStarted, float* frameStart, float* startTime);

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

};

#endif