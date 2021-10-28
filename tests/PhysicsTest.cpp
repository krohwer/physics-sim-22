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

void printVectors(Environment env) {
	Body object = (*env.bodyList.begin());
	cout << "Position:";
	printOneVector(object.position);
	cout << "Velocity:";
	printOneVector(object.velocity);
	cout << "Acceleration:";
	printOneVector(object.massData.inverseMass * object.force);
	cout << "Force:";
	printOneVector(object.force);
}

int main() {
	double totalTime = 0;

	Environment env = Environment(50.0f, 50.0f, 9.81f, 1.0);

	double deltaTime = env.timestep;

	Body object;
	object.massData.mass = 10.0f;
	object.position = glm::vec3(0.0f, 50.0f, 0);

	// calculate the force of gravitation for the object into a vector and apply it
	glm::vec3 gravity(0, object.massData.mass * env.gravity, 0);
	object.force -= gravity;

	env.addBody(&object);

	// initial time
	cout << "Time Elapsed: " << totalTime << endl;
	cout << "Mass: " << object.massData.mass << endl;
	printVectors(env);
	cout << endl;

	glm::vec3 force(10.0f, 0, 0);

	// run a test for 5 seconds and print the object vectors at each step
	for (int i = 0; i < 5; i++)
	{
		totalTime += deltaTime;
		env.step();
		cout << "Time Elapsed: " << totalTime << endl;
		
		printVectors(env);

		cout << endl;
	}

	getchar();


	return 0;
}