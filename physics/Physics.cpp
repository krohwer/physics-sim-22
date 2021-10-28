#include "Physics.h"

#include "Environment.h"
#include "PhysicsObject.h"

void physics::stepObject(Body& object, double deltaTime) {
	// update the current position
	// s = s0 + v0t +1/2at^2 for x, y, and z
	object.position.y = object.position.y + (object.velocity.y * deltaTime) + (0.5f * (object.massData.inverseMass * object.force.y) * (deltaTime * deltaTime));
	object.position.z = object.position.z + (object.velocity.z * deltaTime) + (0.5f * (object.massData.inverseMass * object.force.z) * (deltaTime * deltaTime));
	object.position.x = object.position.x + (object.velocity.x * deltaTime) + (0.5f * (object.massData.inverseMass * object.force.x) * (deltaTime * deltaTime));

	// update the current velocity
	// v = v0 + at for x, y, and z
	object.velocity.x = object.velocity.x + (object.massData.inverseMass * object.force.x) * deltaTime;
	object.velocity.y = object.velocity.y + (object.massData.inverseMass * object.force.y) * deltaTime;
	object.velocity.z = object.velocity.z + (object.massData.inverseMass * object.force.z) * deltaTime;

	// TODO: might force compliance with terminal velocity and drag, who knows
}