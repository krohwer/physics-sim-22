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

	void load(Environment& env, Camera& cam, std::string fileName);

}

#endif