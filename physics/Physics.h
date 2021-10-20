#pragma once

#include <glm/glm.hpp>

#include "PhysicsObject.h"
#include "Environment.h"

#ifndef PHYSICS_H
#define PHYSICS_H

/**
 * @brief Namespace for physics-related functions and calculations
 */
namespace physics {

	/**
	 * Applies the environment gravity to an object by modifying its downward acceleration.
	 * 
	 * @param object - the object to be modified by gravity
	 */
	void applyGravity(PhysicsObject& object, Environment env);

	/**
	 * Applies a force to an object by adding to its acceleration.
	 * 
	 * @param object - the object to be modified by the force
	 * @param force - a vector defining the strength and direction of the force, measured in Newtons (N)
	 */
	void applyForce(PhysicsObject& object, glm::vec3 force);

	/**
	 * Stops the application of a force to an object by subtracting from its acceleration.
	 *
	 * @param object - the object to be modified by the force
	 * @param force - a vector defining the strength and direction of the force, measured in Newtons (N)
	 */
	void removeForce(PhysicsObject& object, glm::vec3 force);

	/**
	 * Modifies the velocity vector of an object based on the value of its acceleration vector and the change in time.
	 *
	 * @param object - the object whose velocity will be recalculated
	 * @param deltaTime - the length of time to apply the acceleration over, measured in seconds (s)
	 */
	void updateVelocity(PhysicsObject& object, double deltaTime);

	/**
	 * Modifies the position vector of an object based on the value of its velocity vector and the change in time.
	 * 
	 * @param object - the object whose position will be recalculated
	 * @param deltaTime - the length of time to apply the velocity over, measured in seconds (s)
	 */
	void updatePosition(PhysicsObject& object, double deltaTime);
}

#endif