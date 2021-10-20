#pragma once

#include <glm/glm.hpp>

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

/// Enumerated type for the object shape
enum class Shape {BOX, TRIANGLE, BALL};

/**
 * PhysicsObject stores the *unique* object data required for both physics calculations and OpenGL rendering.
 * Data can be accessed or set simply with object.data
 * I.E. box1.mass = 20;
 */
struct PhysicsObject {

	/// shape determines how the object is intended to be rendered
	Shape shape;

	/// mass is simply the mass of the object measured in kilograms (kg)
	float mass;

	/// position stores where the object is in relation to the origin. Each component measured in meters (m)
	glm::vec3 position;

	/// velocity stores the speed of an object and in what direction. Each component measured in meters/second (m/s)
	glm::vec3 velocity;

	/// acceleration stores the change in velocity over time and in what direction. Each component measured in meters/second squared (m/s^2)
	glm::vec3 acceleration;

	/// rotation stores the angle of rotation about the Z AXIS measured in degrees
	float rotation;

	/// scale stores the amount to scale the object along each axis. Z should remain 1.0f
	glm::vec3 scale;
};

#endif