#pragma once

#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include <glm/glm.hpp>

// AXIS ALIGNED BOUNDING BOX

struct AABB {
	glm::vec3 min;
	glm::vec3 max;
};

/// Enumerated type for the object shape
enum class Shape {BOX, TRIANGLE, BALL};

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

	/// position stores where the object is in relation to the origin. Each component measured in meters (m)
	glm::vec3 position = glm::vec3(0, 0, 0);
	/// velocity stores the speed of an object and in what direction. Each component measured in meters/second (m/s)
	glm::vec3 velocity = glm::vec3(0, 0, 0);

	/// rotation stores the angle of rotation about the Z AXIS measured in radians
	float rotation = 0.0f;
	/// the rotational speed of the object
	float angularVelocity = 0.0f;
	/// torque is rotational force, or the magnitude of r X F where r is a vector from the CoM to the point of contact, and F is the force of the collision
	float torque = 0.0f;

	/// Mass is simply the mass of the object measured in kilograms (kg)
	float mass = 10.0f;
	/// Inverse mass is 1/mass, which is very commonly used in calculations
	float inverseMass;

	/// Moment of inertia, mr^2 where m is the mass and r is the distance from the center of rotation (CoM)
	float momentOfInertia;

	/// Restitution is the coefficient of "bounciness" of an object.  Used to calculate velocity after a collision. (Wood by default)
	float restitution = 0.2f;

	/// force stores the sum of forces acting on the object in each direction. Each component measured in Newtons (N)
	glm::vec3 force = glm::vec3(0, 0, 0);

	/// material stores the density and restitution of the object
	//Material material;

	/// shape determines how the object is intended to be rendered
	Shape shape = Shape::BOX;

	/// scale stores the amount to scale the object along each axis. Z should remain 1.0f
	glm::vec3 scale = glm::vec3(1, 1, 1);

	/**
	 * Applies a force vector to the object
	 */
	void applyForce(glm::vec3 f) {
		force += f;
	}

	/**
	 * Applies the impulse from a collision to an object at a certain contact point
	 * 
	 * @param impulse - 
	 * @param contactVector - 
	 */
	void applyImpulse(glm::vec3 impulse, glm::vec3 contactVector) {
		velocity += inverseMass * impulse;
		angularVelocity += (1.0f / momentOfInertia) * glm::cross(contactVector, impulse).z;
	}

	void step(float deltaTime) {
		float dtSquared = deltaTime * deltaTime;

		// update the current position
		// s = s0 + v0t +1/2at^2 for x, y, and z
		// a = f/m
		position.x += (velocity.x * deltaTime) + (0.5f * (inverseMass * force.x) * dtSquared);
		position.y += (velocity.y * deltaTime) + (0.5f * (inverseMass * force.y) * dtSquared);
		position.z += (velocity.z * deltaTime) + (0.5f * (inverseMass * force.z) * dtSquared);

		// update the rotation
		// theta = theta0 + w0t + 1/2alphat^2
		// alpha = torque * (1 / momentOfInertia)
		//object.rotation += object.angularVelocity * deltaTime + (0.5f * (object.torque / object.momentOfInertia) * dtSquared);



		// update the current velocity
		// v = v0 + at for x, y, and z
		// a = f/m
		velocity.x += (inverseMass * force.x) * deltaTime;
		velocity.y += (inverseMass * force.y) * deltaTime;
		velocity.z += (inverseMass * force.z) * deltaTime;

		// TODO: might force compliance with terminal velocity and drag, who knows
	}

	AABB getAABB() {
		AABB box;
		// calculate the max and min x of the AABB by getting half of the scaled box and adding/subbing to the position
		box.max.x = position.x + 0.5f * scale.x;
		box.min.x = position.x - 0.5f * scale.x;

		// same for y
		box.max.y = position.y + 0.5f * scale.y;
		box.min.y = position.y + 0.5f * scale.y;

		return box;
	}
};

inline
bool operator==(const Body& lhs, const Body& rhs)
{
	return (lhs.position == rhs.position) && (lhs.shape == rhs.shape);
}

#endif