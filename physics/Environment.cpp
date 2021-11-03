#include "Environment.h"

#include <list>

#include "PhysicsObject.h"

Environment::Environment(float eHeight, float eWidth, float eGravity, float eTimestep) {
	height = eHeight;
	width = eWidth;
	gravity = eGravity;
	timestep = eTimestep;
	pixelRatio = 0;
}

void Environment::addBody(Body* body) {
	// handling for 0 mass as "infinite" mass
	if (body->mass == 0)
		body->inverseMass = 0;
	else
		body->inverseMass = 1.0f / body->mass;

	bodyList.push_back(*body);
}

void Environment::removeBody(Body* body) {
	bodyList.remove(*body);
}

void Environment::step() {
	for (Body& body : bodyList) {
		body.step(timestep);
	}
}
