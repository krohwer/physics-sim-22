#include "pch.h"
#include "ExperimentManager.h"

#include <iostream>
#include <fstream>
#include <sstream>

void Experiment::load(Environment& env, Camera& cam, std::string fileName) {
	// attempt to open the file
	std::ifstream exp;
	exp.open(fileName);
	if (exp.is_open()) {
		std::string line;
		char del = '|';

		// read environment data
		if (getline(exp, line)) {
			// parse environment data
			std::stringstream sstream(line);
			std::string value;
			// get width
			if (getline(sstream, value, del)) {
				env.width = std::stof(value);
			}
			// get height
			if (getline(sstream, value, del)) {
				env.height = std::stof(value);
			}
			// get gravity
			if (getline(sstream, value, del)) {
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
			if (getline(sstream, value, del)) {
				cam.cPosition.x = std::stof(value);
			}
			// get y position
			if (getline(sstream, value, del)) {
				cam.cPosition.y = std::stof(value);
			}
			// get zoom amount
			if (getline(sstream, value, del)) {
				cam.cZoom = std::stof(value);
			}
		}

		// read object data
		while (getline(exp, line)) {
			// parse object data
			float attributes[8] = {};
			std::stringstream sstream(line);
			std::string value;
			int i = 0;
			while (getline(sstream, value, del)) {
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
