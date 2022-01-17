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
}

void Environment::addBody(Body* body) {
	body->shape->SetBox();
	body->shape->body = body;

	bodyList.push_back(*body);
}

void Environment::removeBody(Body* body) {
	bodyList.remove(*body);
}

void Environment::generatePairs() {
	pairs.clear();

	for (std::list<Body>::iterator i = bodyList.begin(); i != std::prev(bodyList.end()); ++i) {
		for (std::list<Body>::iterator j = std::next(i); j != bodyList.end(); ++j) {
			Body* A = &(*i);	// 0_o thanks for being weird iterators
			Body* B = &(*j);

			// only create and add the pair if at most 1 of the objects has infinite mass
			if (!(A->mass == 0.0f && B->mass == 0.0f)) {
				Pair pair;
				pair.A = A;
				pair.B = B;

				pairs.push_back(pair);
			}
		}
	}
}

void Environment::step() {
	for (Body& body : bodyList) {
		body.step(timestep);
	}
	// check pairs for broad phase
	for (Pair& pair : pairs) {
		// create manifolds, check for collisions, and solve
		Manifold manifold(pair.A, pair.B, gravity);
		Dispatch[(int)pair.A->shape->type][(int)pair.B->shape->type](&manifold);
		if (manifold.contactCount) {
			manifold.ApplyImpulse();
		}
	}
}
