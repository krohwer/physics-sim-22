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

/// detect a collision between two bounding boxes
void BoxvsBox(Manifold *manifold);

/// detect a collision between two balls
void BallvsBall(Manifold* manifold);

/// detect a collision between a box and a ball (both ways)
void BoxvsBall(Manifold* manifold);
void BallvsBox(Manifold* manifold);

/// collision callback to ensure appropriate detection code is used
typedef void (*collisionCallback)(Manifold* man);

extern collisionCallback Dispatch[2][2];

#endif