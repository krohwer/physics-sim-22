#pragma once

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <glm/glm.hpp>


// AXIS ALIGNED BOUNDING BOX

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

struct Shape;

/**
 * Contains the material attributes of a physics body
 */
// struct Material {
// 	/// Density is used to calculate mass, where mass = density * volume
// 	float density;
// 	/// Restitution is the coefficient of "bounciness" of an object.  Used to calculate velocity after a collision.
// 	float restitution;
// };

/**
 * Stores the *unique* object data required for both physics calculations and OpenGL rendering.
 */
struct Body {
	// constructor
	Body(Shape* s, float x, float y);

	/// position stores where the object is in relation to the origin. Each component measured in meters (m)
	glm::vec3 position;
	/// velocity stores the speed of an object and in what direction. Each component measured in meters/second (m/s)
	glm::vec3 velocity;

	/// rotation stores the angle of counter-clockwise rotation about the Z AXIS measured in radians
	float rotation;
	/// the rotational speed of the object
	float angularVelocity;
	/// torque is rotational force, or the magnitude of r X F where r is a vector from the CoM to the point of contact, and F is the force of the collision
	float torque;

	/// Mass is simply the mass of the object measured in kilograms (kg)
	float mass;
	/// Inverse mass is 1/mass, which is very commonly used in calculations
	float inverseMass;

	/// Moment of inertia, mr^2 where m is the mass and r is the distance from the center of rotation (CoM)
	float momentOfInertia;

	/// Inverse moment of inertia, similar to inverse mass
	float inverseInertia;

	/// Restitution is the coefficient of "bounciness" of an object.  Used to calculate velocity after a collision. (Wood by default)
	float restitution;

	/// force stores the sum of forces acting on the object in each direction. Each component measured in Newtons (N)
	glm::vec3 force;

	/// material stores the density and restitution of the object
	//Material material;

	/// shape determines how the object is intended to be rendered
	Shape *shape;

	/// scale stores the amount to scale the object along each axis. Z should remain 1.0f
	glm::vec3 scale;

	/**
	 * Applies a force vector to the object
	 */
	void applyForce(glm::vec3 f);

	/**
	 * Applies the impulse from a collision to an object at a certain contact point
	 * 
	 * @param impulse - 
	 * @param contactVector - 
	 */
	void applyImpulse(glm::vec3 impulse, glm::vec3 contactVector);

	void step(float deltaTime);

	AABB getAABB();

	void computeInverseMass();
	void computeInertia();
};

inline bool operator==(const Body& lhs, const Body& rhs)
{
	return (lhs.position == rhs.position) && (lhs.scale == rhs.scale);
}

#endif