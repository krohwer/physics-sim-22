#pragma once

#ifndef EXPERIMENT_MANAGER_H
#define EXPERIMENT_MANAGER_H

#include <string>

#include "pch.h"
#include "Environment.h"
#include "Shape.h"
#include "PhysicsObject.h"
#include "Camera.h"

namespace Experiment {

	std::string saveFile(const char* filter);
	std::string openFile(const char* filter);
	
	void save(Environment& env, Camera& cam, std::string fileName);
	void load(Environment& env, Camera& cam, std::string fileName);

}

#endif