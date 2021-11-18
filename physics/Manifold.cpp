#include "Manifold.h"

#include <algorithm>

#include "PhysicsObject.h"

void Manifold::PositionalCorrection() {
	const float percent = 1.0; // usually 20% to 80%
	const float slop = 0.01; // usually 0.01 to 0.1
	glm::vec3 correction = (std::max(penetration - slop, 0.0f) / (A->inverseMass + B->inverseMass)) * percent * normal;
	A->position -= A->inverseMass * correction;
	B->position += B->inverseMass * correction;
}
