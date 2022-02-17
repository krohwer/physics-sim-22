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
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// get the bounding box of the box
	AABB abox = a->getAABB();

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	// Closest point on A to center of B
	glm::vec3 closest = n;

	// Calculate half extents along each axis
	float x_extent = (abox.max.x - abox.min.x) / 2;
	float y_extent = (abox.max.y - abox.min.y) / 2;

	// Clamp point to edges of the AABB
	closest.x = clamp(-x_extent, x_extent, closest.x);
	closest.y = clamp(-y_extent, y_extent, closest.y);

	bool inside = false;

	// check if the circle is inside the bounding box
	if (Equal(n.x, closest.x) && Equal(n.y, closest.y) && Equal(n.z, closest.z)) {
		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y)) {
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		else {
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	glm::vec3 normal = n - closest;
	float distance = lenSqr(normal);
	float r = b->radius;

	// if the circle is inside the box or the distance is less than the radius, there is a collision
	if (!(distance > r * r && !inside)) {

		// Avoided sqrt and normalize until we needed
		distance = sqrt(distance);
		normal = normalize(normal);

		// Collision normal needs to be flipped to point outside if circle was
		// inside the AABB
		if (inside) {
			manifold->normal = -normal;
			manifold->penetration = r - distance;
		}
		else {
			manifold->normal = normal;
			manifold->penetration = r - distance;
		}
		manifold->contactCount = 1;
	}
}

void BallvsBox(Manifold* manifold) {
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// get the bounding box of the box
	AABB bbox = b->getAABB();

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	// Closest point on A to center of B
	glm::vec3 closest = n;

	// Calculate half extents along each axis
	float x_extent = (bbox.max.x - bbox.min.x) / 2;
	float y_extent = (bbox.max.y - bbox.min.y) / 2;

	// Clamp point to edges of the AABB
	if (closest.x < -x_extent)
		closest.x -= (-x_extent);
	else if (closest.x > x_extent)
		closest.x -= x_extent;
	if (closest.y < -y_extent)
		closest.y -= (-y_extent);
	else if (closest.y > y_extent)
		closest.y -= y_extent;

	bool inside = false;

	// check if the circle is inside the bounding box
	if (Equal(n.x, closest.x) && Equal(n.y, closest.y) && Equal(n.z, closest.z)) {
		inside = true;

		// Find closest axis
		if (abs(n.x) > abs(n.y)) {
			// Clamp to closest extent
			if (closest.x > 0)
				closest.x = x_extent;
			else
				closest.x = -x_extent;
		}
		else {
			// Clamp to closest extent
			if (closest.y > 0)
				closest.y = y_extent;
			else
				closest.y = -y_extent;
		}
	}

	glm::vec3 normal = closest;
	float distance = lenSqr(normal);
	float r = a->radius;

	// if the circle is inside the box or the distance is less than the radius, there is a collision
	if (!(distance > r * r && !inside)) {

		// Avoided sqrt and normalize until we needed
		distance = sqrt(distance);
		normal = normalize(normal);

		// Collision normal needs to be flipped to point outside if circle was
		// inside the AABB
		if (inside) {
			manifold->normal = -normal;
			manifold->penetration = r - distance;
		}
		else {
			manifold->normal = normal;
			manifold->penetration = r - distance;
		}
		manifold->contactCount = 1;
	}
}

void BallvsBall(Manifold* manifold) {
	// pointers to objects
	Body* a = manifold->A;
	Body* b = manifold->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	//float r = a->radius + b->radius;
	float r = a->radius + b->radius;
	r *= r;
	float distance = lenSqr(n);

	// check if the circles in the manifold are actually colliding
	if (r < distance) {
		// if they aren't, do nothing!
	}
	else {
		// if they are colliding

		// safely do a square root for the distance between the objects
		distance = sqrt(distance);

		if (distance != 0) {
			manifold->penetration = r - distance;
			manifold->normal = normalize(n);
		}
		// set the contactCount to 1 to indicate a collision
		manifold->contactCount = 1;
	}
}

collisionCallback Dispatch[2][2] = { {BoxvsBox, BoxvsBall}, 
									{BallvsBox, BallvsBall} };
