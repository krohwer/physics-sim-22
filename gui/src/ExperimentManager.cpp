#include "pch.h"
#include "ExperimentManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <windows.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define DELIMITER '|'

std::string Experiment::saveFile(const char* filter) {
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = "Experiments";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

std::string Experiment::openFile(const char* filter) {
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = "Experiments";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetOpenFileNameA(&ofn) == TRUE)
		return ofn.lpstrFile;

	return std::string();
}

void Experiment::save(Environment& env, Camera& cam, std::string fileName) {
	std::ofstream exp;
	if (fileName.substr(fileName.find_last_of(".") + 1) == "klx") {
		exp.open(fileName, std::ios::trunc);
	}
	else {
		exp.open(fileName + ".klx", std::ios::trunc);
	}
	if (exp.is_open()) {
		// write environment data
		exp << env.width << DELIMITER << env.height << DELIMITER << env.gravity << std::endl;
		// write camera data
		exp << cam.cPosition.x << DELIMITER << cam.cPosition.y << DELIMITER << cam.cZoom << std::endl;

		for (Body& body : env.bodyList) {
			// write shape
			exp << (float)body.shape->type << DELIMITER;
			// write x and y position
			exp << body.position.x << DELIMITER << body.position.y << DELIMITER;
			// write velocity data
			exp << body.vSpeed << DELIMITER << body.vDirection << DELIMITER;
			// write mass and restitution
			exp << body.mass << DELIMITER << body.restitution << DELIMITER;
			// write scale
			exp << body.scale.x << DELIMITER << body.scale.y;
			exp << std::endl;
		}
		exp.close();
	}
	else
		std::cout << "Error saving file " << fileName << std::endl;
}

void Experiment::load(Environment& env, Camera& cam, std::string fileName) {
	// attempt to open the file
	std::ifstream exp;
	exp.open(fileName);
	if (exp.is_open()) {
		std::string line;

		// read environment data
		if (getline(exp, line)) {
			// parse environment data
			std::stringstream sstream(line);
			std::string value;
			// get width
			if (getline(sstream, value, DELIMITER)) {
				env.width = std::stof(value);
			}
			// get height
			if (getline(sstream, value, DELIMITER)) {
				env.height = std::stof(value);
			}
			// get gravity
			if (getline(sstream, value, DELIMITER)) {
				env.gravity = std::stof(value);
			}
			env.computeAxes();
		}
		// read camera data
		if (getline(exp, line)) {
			// parse camera data
			std::stringstream sstream(line);
			std::string value;
			// get x position
			if (getline(sstream, value, DELIMITER)) {
				cam.cPosition.x = std::stof(value);
			}
			// get y position
			if (getline(sstream, value, DELIMITER)) {
				cam.cPosition.y = std::stof(value);
			}
			// get zoom amount
			if (getline(sstream, value, DELIMITER)) {
				cam.cZoom = std::stof(value);
			}
		}

		// read object data
		while (getline(exp, line)) {
			// parse object data
			float attributes[9] = {};
			std::stringstream sstream(line);
			std::string value;
			int i = 0;
			while (getline(sstream, value, DELIMITER)) {
				attributes[i] = std::stof(value);
				i++;
			}
			env.addBody(attributes);
		}
		exp.close();
	}
	else
		std::cout << "Error opening file " << fileName << std::endl;
}
