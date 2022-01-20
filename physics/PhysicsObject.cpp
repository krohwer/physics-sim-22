#include "PhysicsObject.h"

#include "Shape.h"

// constructor
Body::Body(Shape* s, float x, float y) {
	shape = s->Clone();
	shape->body = this;
	position = glm::vec3(x, y, 0.0f);
	velocity = glm::vec3(0.0f);
	vDirection = 0.0f;
	vSpeed = 0.0f;

	force = glm::vec3(0.0f);

	rotation = 0.0f;
	angularVelocity = 0.0f;
	torque = 0.0f;

	scale = glm::vec3(1.0f);
	color = glm::vec4(1.0f);

	mass = 10.0f;
	momentOfInertia = 0.0f;
	restitution = 0.2f;

	computeInverseMass();
	computeInertia();
}

void Body::applyForce(glm::vec3 f) {
	if (!(inverseMass == 0.0f))
		force += f;
}

void Body::applyImpulse(glm::vec3 impulse, glm::vec3 contactVector) {
	velocity += inverseMass * impulse;
	angularVelocity += (1.0f / momentOfInertia) * glm::cross(contactVector, impulse).z;
}

void Body::step(float deltaTime) {
	float dtSquared = deltaTime * deltaTime;

	// infinite mass check
	if (!(inverseMass == 0.0f)) {
	
		// update the current position
		// s = s0 + v0t +1/2at^2 for x, y, and z
		// a = f/m
		position.x += (velocity.x * deltaTime) + (0.5f * (inverseMass * force.x) * dtSquared);
		position.y += (velocity.y * deltaTime) + (0.5f * (inverseMass * force.y) * dtSquared);
		position.z += (velocity.z * deltaTime) + (0.5f * (inverseMass * force.z) * dtSquared);

		// update the rotation
		// theta = theta0 + w0t + 1/2alphat^2
		// alpha = torque * inverseInertia
		rotation += angularVelocity * deltaTime + (0.5f * (torque * inverseInertia) * dtSquared);

		// update the current velocity
		// v = v0 + at for x, y, and z
		// a = f/m
		velocity.x += (inverseMass * force.x) * deltaTime;
		velocity.y += (inverseMass * force.y) * deltaTime;
		velocity.z += (inverseMass * force.z) * deltaTime;

		// update the angular velocity
		angularVelocity += torque * inverseInertia * deltaTime;

		computeVelocityVector();

		// TODO: might force compliance with terminal velocity and drag, who knows
	}
}

AABB Body::getAABB() {
	AABB box;
	// calculate the max and min x of the AABB by getting half of the scaled box and adding/subbing to the position
	box.max.x = position.x + 0.5f * scale.x;
	box.min.x = position.x - 0.5f * scale.x;

	// same for y
	box.max.y = position.y + 0.5f * scale.y;
	box.min.y = position.y - 0.5f * scale.y;

	return box;
}

void Body::computeInverseMass() {
	if (mass == 0.0f)
		inverseMass = 0.0f;
	else
		inverseMass = 1 / mass;
}

void Body::computeInertia() {
	inverseInertia = 0.0f;
}

void Body::init() {
	computeInverseMass();
	computeInertia();
	computeVelocityComponents();
	shape->scaleX(scale);
	shape->scaleY(scale);
}

void Body::computeVelocityVector() {
	vSpeed = length(velocity);
	if (vSpeed > 0) {
		vDirection = acos(glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), velocity) / vSpeed);
		if (velocity.y < 0)
			vDirection *= -1;
		vDirection *= toDegrees;
	}
	else {
		vDirection = 0.0f;
	}
}

void Body::computeVelocityComponents() {
	velocity.x = vSpeed * cos(vDirection * toRadians);
	velocity.y = vSpeed * sin(vDirection * toRadians);
}
