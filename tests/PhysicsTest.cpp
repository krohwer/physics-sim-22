#include <glm/glm.hpp>

#include <math.h>
#include <iostream>

#include "Environment.h"
#include "Physics.h"
#include "PhysicsObject.h"

using namespace std;

void printOneVector(glm::vec3 vector) {
	cout << "{" << vector.x << ", " << vector.y << ", " << vector.z << "}" << endl;
}

void printVectors(PhysicsObject object) {
	cout << "Position:";
	printOneVector(object.position);
	cout << "Velocity:";
	printOneVector(object.velocity);
	cout << "Acceleration:";
	printOneVector(object.acceleration);
}

int main() {
	double totalTime = 0;
	double deltaTime = 1;

	Environment env;
	env.gravity = 9.81f;

	PhysicsObject object;
	object.mass = 10.0f;
	object.position = glm::vec3(0.0f, 50.0f, 0);

	// initial time
	cout << "Time Elapsed: " << totalTime << endl;
	cout << "Mass: " << object.mass << endl;
	printVectors(object);
	// apply gravity only once
	physics::applyGravity(object, env);
	cout << endl;

	glm::vec3 force(10.0f, 0, 0);
	// apply the force once
	physics::applyForce(object, force);

	// run a test for 5 seconds and print the object vectors at each step
	for (int i = 0; i < 5; i++)
	{
		totalTime += deltaTime;
		physics::updateObject(object, deltaTime);
		cout << "Time Elapsed: " << totalTime << endl;
		
		printVectors(object);
		
		// apply the force for 2 seconds by removing it after 2 seconds has passed
		if (totalTime == 2) {
			physics::removeForce(object, force);
		}

		cout << endl;
	}

	getchar();


	return 0;
}