#include "Environment.h"

#include <list>
#include <iterator>

#include "PhysicsObject.h"
#include "Physics.h"

Environment::Environment(float eHeight, float eWidth, float eGravity, double eTimestep) {
	height = eHeight;
	width = eWidth;
	gravity = eGravity;
	timestep = eTimestep;
	pixelRatio = 0;
}

void Environment::addBody(Body* body) {
	body->massData.inverseMass = 1.0f / body->massData.mass;
	bodyList.push_back(*body);
}

void Environment::removeBody(Body* body) {
	bodyList.remove(*body);
}

void Environment::step() {
// 	for (std::list<Body>::iterator iter = bodyList.begin(); iter != bodyList.end(); ++iter) {
// 		physics::stepObject(*iter, timestep);
// 	}
	for (Body& body : bodyList) {
		physics::stepObject(body, timestep);
	}
}
