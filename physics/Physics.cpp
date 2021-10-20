#include "Physics.h"

#include "Environment.h"
#include "PhysicsObject.h"

void physics::applyGravity(PhysicsObject& object, Environment env) {
	// gravity value should be positive, effect needs to be negative
	object.acceleration.y -= env.gravity;
}

void physics::applyForce(PhysicsObject& object, glm::vec3 force) {
	// F = ma
	object.acceleration.x += (force.x / object.mass);
	object.acceleration.y += (force.y / object.mass);
	object.acceleration.z += (force.z / object.mass);
}

void physics::removeForce(PhysicsObject& object, glm::vec3 force) {
	// F = ma
	object.acceleration.x -= (force.x / object.mass);
	object.acceleration.y -= (force.y / object.mass);
	object.acceleration.z -= (force.z / object.mass);
}

void physics::updateVelocity(PhysicsObject& object, double deltaTime) {
	// v = v0 + at for x, y, and z
	object.velocity.x = object.velocity.x + object.acceleration.x * deltaTime;
	object.velocity.y = object.velocity.y + object.acceleration.y * deltaTime;
	object.velocity.z = object.velocity.z + object.acceleration.z * deltaTime;
}

void physics::updatePosition(PhysicsObject& object, double deltaTime) {
	// p = p0 + vt for x, y, and z
	object.position.x = object.position.x + object.velocity.x * deltaTime;
	object.position.y = object.position.y + object.velocity.y * deltaTime;
	object.position.z = object.position.z + object.velocity.z * deltaTime;
}
