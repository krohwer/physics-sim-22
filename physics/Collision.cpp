#include "Collision.h"

#include <glm/glm.hpp>

#include "Manifold.h"

bool AABBvsAABB(Manifold* man) {
	bool collision = false;
	// pointers to objects
	Body* a = man->A;
	Body* b = man->B;

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
			if (x_overlap > y_overlap) {
				// point toward b since n points from a to b
				if (n.x < 0)
					man->normal = glm::vec3(-1.0f, 0.0f, 0.0f);
				else
					man->normal = glm::vec3(1.0f, 0.0f, 0.0f);
				man->penetration = x_overlap;
				collision = true;
			}
			else {
				// y is the least overlap
				// point toward b since n points from a to b
				if (n.y < 0)
					man->normal = glm::vec3(0.0f, -1.0f, 0.0f);
				else
					man->normal = glm::vec3(0.0f, 1.0f, 0.0f);
				man->penetration = y_overlap;
				collision = true;
			}
		}
	}

	return collision;
}

bool CirclevsCircle(Manifold* man) {
	bool collision = true;
	// pointers to objects
	Body* a = man->A;
	Body* b = man->B;

	// vector from A to B
	glm::vec3 n = b->position - a->position;

	//float r = a->radius + b->radius;
	float r = 1.0f;
	r *= r;

	// check if the circles in the manifold are actually colliding
	if (r < (n.x * n.x) + (n.y * n.y) + (n.z * n.z)) 
		collision = false;
	else {
		// if they are colliding

		// safely do a square root for the distance between the objects
		float distance = n.length();

		if (distance != 0) {
			man->penetration = r - distance;
		}
	}
	return collision;
}
