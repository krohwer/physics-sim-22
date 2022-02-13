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

	// create axes
	Shape yAxisShape;
	yAxis = Body(&yAxisShape, -0.5f, (0.5f * height));
	yAxis.mass = 0.0f;
	yAxis.scale = glm::vec3(1.0f, height, 1.0f);
	Shape xAxisShape;
	xAxis = Body(&xAxisShape, (0.5f * width), -0.5);
	xAxis.mass = 0.0f;
	xAxis.scale = glm::vec3(width, 1.0f, 1.0f);
}

Body* Environment::addBody(float xPosition, float yPosition) {
	Shape shape;
	Body body(&shape, xPosition, yPosition);
	bodyList.push_back(body);

	return &bodyList.back();
}

Body* Environment::addBody(float attr[8]) {
	Shape shape;
	Body body(&shape, attr[0], attr[1]);
	body.vSpeed = attr[2];
	body.vDirection = attr[3];
	body.mass = attr[4];
	body.restitution = attr[5];
	body.scale.x = attr[6];
	body.scale.y = attr[7];
	bodyList.push_back(body);

	return &bodyList.back();
}

void Environment::removeBody(Body* body) {
	bodyList.remove(*body);
}

void Environment::generatePairs() {
	pairs.clear();
	if (!bodyList.empty()) {
		// pair all objects with the environment
		for (Body& body : bodyList) {
			Body* X = &xAxis;
			Body* Y = &yAxis;
			Body* B = &body;
			Pair pair;
			pair.A = X;
			pair.B = B;
			pairs.push_back(pair);
			pair.A = Y;
			pairs.push_back(pair);
		}

		for (std::list<Body>::iterator i = bodyList.begin(); i != std::prev(bodyList.end()); ++i) {
			Body* A = &(*i);	// 0_o thanks for being weird iterators
			for (std::list<Body>::iterator j = std::next(i); j != bodyList.end(); ++j) {
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

void Environment::computeAxes() {
	xAxis.scale.x = width + 2.0f;
	xAxis.position.x = 0.5f * width - 1.0f;
	yAxis.scale.y = height + 2.0f;
	yAxis.position.y = 0.5f * height - 1.0f;
}
