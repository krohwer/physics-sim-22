#pragma once

#include <glm/glm.hpp>

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

/// Enumerated type for the object shape
enum class Shape {BOX, TRIANGLE, BALL};

/**
 * Contains the material attributes of a physics body
 */
struct Material {
	/// Density is used to calculate mass, where mass = density * volume
	float density;
	/// Restitution is the coefficient of "bounciness" of an object.  Used to calculate velocity after a collision.
	float restitution;
};

/**
 * Contains the mass-related information of a physics body
 */
struct MassData {
	/// Mass is simply the mass of the object measured in kilograms (kg)
	float mass;
	/// Inverse mass is 1/mass, which is very commonly used in calculations
	float inverseMass;

	/// Used for rotations
	float inertia;
	/// Used for rotations
	float inverseInertia;
};

/**
 * Stores the *unique* object data required for both physics calculations and OpenGL rendering.
 */
struct Body {

	/// shape determines how the object is intended to be rendered
	Shape shape = Shape::BOX;

	/// position stores where the object is in relation to the origin. Each component measured in meters (m)
	glm::vec3 position = glm::vec3(0, 0, 0);

	/// rotation stores the angle of rotation about the Z AXIS measured in degrees
	float rotation = 0.0f;

	/// scale stores the amount to scale the object along each axis. Z should remain 1.0f
	glm::vec3 scale = glm::vec3(0, 0, 0);

	/// material stores the density and restitution of the object
	Material material;

	/// massData stores the mass, inertia, and their inverses of the object
	MassData massData;

	/// velocity stores the speed of an object and in what direction. Each component measured in meters/second (m/s)
	glm::vec3 velocity = glm::vec3(0, 0, 0);

	/// force stores the sum of forces acting on the object in each direction. Each component measured in Newtons (N)
	glm::vec3 force = glm::vec3(0, 0, 0);

};

inline
bool operator==(const Body& lhs, const Body& rhs)
{
	return (lhs.position == rhs.position) && (lhs.shape == rhs.shape);
}

#endif