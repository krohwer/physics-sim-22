#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <glm/glm.hpp>
#include "PhysicsObject.h"
#include "Shape.h"
#include "Manifold.h"

struct Pair {
	Body* A;
	Body* B;
};

//float findAxisLeastPenetration(int* faceIndex, Shape* A, Shape* B);

//void FindIncidentFace(glm::vec3* v, Shape* RefPoly, Shape* IncPoly, int referenceIndex);

//int Clip(glm::vec3 n, float c, glm::vec3* face);

/// detect a collision between two polygons
//void PolygonvsPolygon(Manifold* m);

/// detect a collision between two bounding boxes
void BoxvsBox(Manifold *manifold);

/// detect a collision between two balls
void BallvsBall(Manifold* manifold);

void BoxvsBall(Manifold* manifold);
void BallvsBox(Manifold* manifold);

typedef void (*collisionCallback)(Manifold* man);

extern collisionCallback Dispatch[2][2];

#endif