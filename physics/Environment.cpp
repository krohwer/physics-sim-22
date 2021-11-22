#include "Environment.h"

#include <list>

#include "PhysicsObject.h"
#include "Collision.h"
#include "Manifold.h"

Environment::Environment(float eWidth, float eHeight, float eGravity, float eTimestep) {
	width = eWidth;
	height = eHeight;
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

void Environment::calculateInverseMasses() {
	for (Body& body : bodyList) {
		body.inverseMass = 1.0f / body.mass;
	}
}

void Environment::generatePairs() {
	pairs.clear();

	for (std::list<Body>::iterator i = bodyList.begin(); i != std::prev(bodyList.end()); ++i) {
		for (std::list<Body>::iterator j = std::next(i); j != bodyList.end(); ++j) {
			Body* A = &(*i);	// 0_o thanks for being weird iterators
			Body* B = &(*j);

			// create the pair
			Pair pair;
			pair.A = A;
			pair.B = B;

			// add it to pairs
			pairs.push_back(pair);
		}
	}
}

void Environment::step() {
	for (Body& body : bodyList) {
		body.step(timestep);
		bool envCollision = collisionWithEnv(width, &body);
	}
	// create manifolds, check for collisions, and solve
	for (Pair& pair : pairs) {
		Manifold manifold(pair.A, pair.B);
		bool objectCollision = AABBvsAABB(&manifold);
		if (objectCollision) {
			resolveCollision(&manifold);
		}
	}
}
