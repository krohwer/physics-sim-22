#include "Physics.h"

#include "Environment.h"
#include "PhysicsObject.h"

void physics::stepObject(Body& object, float deltaTime) {
	float dtSquared = deltaTime * deltaTime;

	// update the current position
	// s = s0 + v0t +1/2at^2 for x, y, and z
	// a = f/m
	object.position.x += (object.velocity.x * deltaTime) + (0.5f * (object.inverseMass * object.force.x) * dtSquared);
	object.position.y += (object.velocity.y * deltaTime) + (0.5f * (object.inverseMass * object.force.y) * dtSquared);
	object.position.z += (object.velocity.z * deltaTime) + (0.5f * (object.inverseMass * object.force.z) * dtSquared);

	// update the rotation
	// theta = theta0 + w0t + 1/2alphat^2
	// alpha = torque * (1 / momentOfInertia)
	//object.rotation += object.angularVelocity * deltaTime + (0.5f * (object.torque / object.momentOfInertia) * dtSquared);

	
	
	// update the current velocity
	// v = v0 + at for x, y, and z
	// a = f/m
	object.velocity.x += (object.inverseMass * object.force.x) * deltaTime;
	object.velocity.y += (object.inverseMass * object.force.y) * deltaTime;
	object.velocity.z += (object.inverseMass * object.force.z) * deltaTime;

	// TODO: might force compliance with terminal velocity and drag, who knows
}