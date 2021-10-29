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
	// s = s0 + v0t +1/2at^2 for x, y, and z
	object.position.x = object.position.x + (object.velocity.x * deltaTime) + (0.5 * object.acceleration.x * (deltaTime * deltaTime));
	object.position.y = object.position.y + (object.velocity.y * deltaTime) + (0.5 * object.acceleration.y * (deltaTime * deltaTime));
	object.position.z = object.position.z + (object.velocity.z * deltaTime) + (0.5 * object.acceleration.z * (deltaTime * deltaTime));
}

void physics::updateObject(PhysicsObject& object, Environment env, double deltaTime) {
	
	if (object.position.x < 0.5f)
	{
		object.acceleration.x = 0.0f;
		object.velocity.x = 0.0f;
		object.position.x = 0.5f;
	}
	if (object.position.x > (env.width - 0.5f))
	{
		object.acceleration.x = 0.0f;
		object.velocity.x = 0.0f;
		object.position.x = env.width - 0.5f;
	}
	if (object.position.y < 0.5f)
	{
		object.acceleration.y = 0.0f;
		object.velocity.y = 0.0f;
		object.position.y = 0.5f;
	}
// 	if (object.position.y > (env.height - 0.5f))
// 	{
// 		object.acceleration.y = 0.0f;
// 		object.velocity.y = 0.0f;
// 		object.position.y = env.height - 0.5f;
// 	}
	
	// update the position of the object using the velocity of the previous timestep as the initial velocity
	updatePosition(object, deltaTime);
	// update the object's velocity to the current velocity
	updateVelocity(object, deltaTime);

	// TODO: might force compliance with terminal velocity and drag, who knows
}

void physics::resetObject(PhysicsObject& object) {
	object.velocity.x = 0.0f;
	object.velocity.y = 0.0f;
	object.velocity.z = 0.0f;

	object.acceleration.x = 0.0f;
	object.acceleration.y = 0.0f;
	object.acceleration.z = 0.0f;
}

