#pragma once


#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm/glm.hpp>

struct Body;

/**
 * @brief Namespace for physics-related functions and calculations [DEPRECATED]
 */
namespace physics {
	/**
	 * Updates an objects position based on it's initial velocity (velocity at previous timestep), then updates its velocity to the current instantaneous velocity.
	 * 
	 * @param object - the object whose position and velocity will be recalculated
	 * @param deltaTime - the length of time to apply over, measured in seconds (s)
	 */
	void stepObject(Body& object, float deltaTime);
}

#endif