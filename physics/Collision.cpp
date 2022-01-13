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

/**
 * finds the axis of least penetration and returns the greatest penetration along that axis
 * Must be called twice, flipping A and B.
 * 
 * Positive penetration indicates no collision
 * Negative penetration indicates a collision, and no separating axis
 */
float findAxisLeastPenetration(int* faceIndex, Shape* A, Shape* B) {
	float bestDistance = -FLT_MAX;
	int bestIndex = 0;

	for (int i = 1; i < A->m_vertexCount; i++) {
		// get a face normal from A
		glm::vec3 normal = A->m_normals[i];
		glm::vec3 nw = A->u * normal;

		// transform face normal into B's model space
		mat22 buT = B->u.Transpose();
		normal = buT * nw;

		// get a support point from B along -normal
		glm::vec3 support = B->GetSupport(-normal);
		// retrieve vertex on face from A
		glm::vec3 vertex = A->m_vertices[i];

		// convert the vertex into B's model space
		vertex = A->u * vertex + A->body->position;
		vertex -= B->body->position;
		vertex = buT * vertex;

		// Compute penetration distance (in B's model space)
		float distance = glm::dot(normal, support - vertex);

		// Store greatest distance
		if (distance > bestDistance)
		{
			bestDistance = distance;
			bestIndex = i;
		}
	}

	*faceIndex = bestIndex;
	return bestDistance;
}

void FindIncidentFace(glm::vec3* v, Shape* RefPoly, Shape* IncPoly, int referenceIndex) {
	glm::vec3 referenceNormal = RefPoly->m_normals[referenceIndex];

	// Calculate normal in incident's frame of reference
	referenceNormal = RefPoly->u * referenceNormal; // To world space
	referenceNormal = IncPoly->u.Transpose() * referenceNormal; // To incident's model space

	// Find most anti-normal face on incident polygon
	int incidentFace = 0;
	float minDot = FLT_MAX;
	for (int i = 1; i < IncPoly->m_vertexCount; i++)
	{
		float dot = glm::dot(referenceNormal, IncPoly->m_normals[i]);
		if (dot < minDot)
		{
			minDot = dot;
			incidentFace = i;
		}
	}

	// Assign face vertices for incidentFace
	v[0] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
	incidentFace = incidentFace + 1 >= (int)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
	v[1] = IncPoly->u * IncPoly->m_vertices[incidentFace] + IncPoly->body->position;
}

// int Clip(glm::vec3 n, float c, glm::vec3* face)
// {
// 	int sp = 0;
// 	glm::vec3 out[2] = {
// 		face[0],
// 		face[1]
// 	};
// 
// 	// Retrieve distances from each endpoint to the line
// 	// d = ax + by - c
// 	float d1 = glm::dot(n, face[0]) - c;
// 	float d2 = glm::dot(n, face[1]) - c;
// 
// 	// If negative (behind plane) clip
// 	if (d1 <= 0.0f) out[sp++] = face[0];
// 	if (d2 <= 0.0f) out[sp++] = face[1];
// 
// 	// If the points are on different sides of the plane
// 	if (d1 * d2 < 0.0f) // less than to ignore -0.0f
// 	{
// 		// Push intersection point
// 		float alpha = d1 / (d1 - d2);
// 		out[sp] = face[0] + alpha * (face[1] - face[0]);
// 		++sp;
// 	}
// 
// 	// Assign our new converted values
// 	face[0] = out[0];
// 	face[1] = out[1];
// 
// 	assert(sp != 3);
// 
// 	return sp;
// }
// 
// void PolygonvsPolygon(Manifold* m)
// {
// 	Shape* A = m->A->shape;
// 	Shape* B = m->B->shape;
// 	m->contactCount = 0;
// 
// 	// Check for a separating axis with A's face planes
// 	int faceA;
// 	float penetrationA = findAxisLeastPenetration(&faceA, A, B);
// 	if (penetrationA >= 0.0f)
// 		return;
// 
// 	// Check for a separating axis with B's face planes
// 	int faceB;
// 	float penetrationB = findAxisLeastPenetration(&faceB, B, A);
// 	if (penetrationB >= 0.0f)
// 		return;
// 
// 	int referenceIndex;
// 	bool flip; // Always point from a to b
// 
// 	Shape* RefPoly; // Reference
// 	Shape* IncPoly; // Incident
// 
// 	// Determine which shape contains reference face
// 	if (BiasGreaterThan(penetrationA, penetrationB))
// 	{
// 		RefPoly = A;
// 		IncPoly = B;
// 		referenceIndex = faceA;
// 		flip = false;
// 	}
// 
// 	else
// 	{
// 		RefPoly = B;
// 		IncPoly = A;
// 		referenceIndex = faceB;
// 		flip = true;
// 	}
// 
// 	// World space incident face
// 	glm::vec3 incidentFace[2];
// 	FindIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);
// 
// 	//        y
// 	//        ^  ->n       ^
// 	//      +---c ------posPlane--
// 	//  x < | i |\
//     //      +---+ c-----negPlane--
// 	//             \       v
// 	//              r
// 	//
// 	//  r : reference face
// 	//  i : incident poly
// 	//  c : clipped point
// 	//  n : incident normal
// 
// 	// Setup reference face vertices
// 	glm::vec3 v1 = RefPoly->m_vertices[referenceIndex];
// 	referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
// 	glm::vec3 v2 = RefPoly->m_vertices[referenceIndex];
// 
// 	// Transform vertices to world space
// 	v1 = RefPoly->u * v1 + RefPoly->body->position;
// 	v2 = RefPoly->u * v2 + RefPoly->body->position;
// 
// 	// Calculate reference face side normal in world space
// 	glm::vec3 sidePlaneNormal = (v2 - v1);
// 	sidePlaneNormal = glm::normalize(sidePlaneNormal);
// 
// 	// Orthogonalize
// 	glm::vec3 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x, 0.0f);
// 
// 	// ax + by = c
// 	// c is distance from origin
// 	float refC = glm::dot(refFaceNormal, v1);
// 	float negSide = -glm::dot(sidePlaneNormal, v1);
// 	float posSide = glm::dot(sidePlaneNormal, v2);
// 
// 	// Clip incident face to reference face side planes
// 	if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
// 		return; // Due to floating point error, possible to not have required points
// 
// 	if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
// 		return; // Due to floating point error, possible to not have required points
// 
// 	  // Flip
// 	m->normal = flip ? -refFaceNormal : refFaceNormal;
// 
// 	// Keep points behind reference face
// 	int cp = 0; // clipped points behind reference face
// 	float separation = glm::dot(refFaceNormal, incidentFace[0]) - refC;
// 	if (separation <= 0.0f)
// 	{
// 		m->contacts[cp] = incidentFace[0];
// 		m->penetration = -separation;
// 		++cp;
// 	}
// 	else
// 		m->penetration = 0;
// 
// 	separation = glm::dot(refFaceNormal, incidentFace[1]) - refC;
// 	if (separation <= 0.0f)
// 	{
// 		m->contacts[cp] = incidentFace[1];
// 
// 		m->penetration += -separation;
// 		++cp;
// 
// 		// Average penetration
// 		m->penetration /= (float)cp;
// 	}
// 
// 	m->contactCount = cp;
// }

collisionCallback Dispatch[2][2] = { {BoxvsBox, BoxvsBall}, 
									{BallvsBox, BallvsBall} };
