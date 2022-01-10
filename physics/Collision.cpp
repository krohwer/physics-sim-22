#include "Collision.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>

#include "Environment.h"
#include "Manifold.h"
#include "PhysicsObject.h"
#include "PhysicsMath.h"

void BoxvsBox(Manifold* manifold) {
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	AABB abox = a->getAABB();
	AABB bbox = b->getAABB();

	// calculate half extents along the x axis
	float a_extent = (abox.max.x - abox.min.x) / 2;
	float b_extent = (bbox.max.x - bbox.min.x) / 2;

	// calculate overlap on x
	float x_overlap = a_extent + b_extent - abs(n.x);

	// SAT test on x
	if (x_overlap > 0) {
		// calculate half extents along the y axis
		float a_extent = (abox.max.y - abox.min.y) / 2;
		float b_extent = (bbox.max.y - bbox.min.y) / 2;

		float y_overlap = a_extent + b_extent - abs(n.y);

		// SAT test on y
		if (y_overlap > 0) {
			// find the axis of least penetration
			if (x_overlap < y_overlap) {
				// point toward b since n points from a to b
				if (n.x < 0)
					manifold->normal = glm::vec3(-1.0f, 0.0f, 0.0f);
				else
					manifold->normal = glm::vec3(1.0f, 0.0f, 0.0f);
				manifold->penetration = x_overlap;
				// set the contactCount to 1 to indicate a collision
				manifold->contactCount = 1;
			}
			else {
				// y is the least overlap
				// point toward b since n points from a to b
				if (n.y < 0)
					manifold->normal = glm::vec3(0.0f, -1.0f, 0.0f);
				else
					manifold->normal = glm::vec3(0.0f, 1.0f, 0.0f);
				manifold->penetration = y_overlap;
				// set the contactCount to 1 to indicate a collision
				manifold->contactCount = 1;
			}
		}
	}
}

void BoxvsBall(Manifold* manifold) {
	//wip
}

void BallvsBox(Manifold* manifold) {
	//wip
}

void BallvsBall(Manifold* manifold) {
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	//float r = a->radius + b->radius;
	float r = 1.0f;
	r *= r;

	// check if the circles in the manifold are actually colliding
	if (r < (n.x * n.x) + (n.y * n.y) + (n.z * n.z)) {
		// if they aren't, do nothing!
	}
	else {
		// if they are colliding

		// safely do a square root for the distance between the objects
		float distance = n.length();

		if (distance != 0) {
			manifold->penetration = r - distance;
		}
		// set the contactCount to 1 to indicate a collision
		manifold->contactCount = 1;
	}
}

collisionCallback Dispatch[2][2] = { {BoxvsBox, BoxvsBall}, 
									{BallvsBox, BallvsBall} };
