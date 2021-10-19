#include <glm/glm.hpp>

#include <math.h>
#include <iostream>

using namespace std;

int main() {
	glm::vec3 v1(2.0, 3.0, 0.0);
	glm::vec3 v2(5.0, 1.0, 0.0);
	
	cout << "Your Vectors:" << endl;
	cout << v1.x << ", " << v1.y << " mag: " << glm::length(v1) << endl;
	cout << v2.x << ", " << v2.y << " mag: " << glm::length(v2) << endl;

	glm::vec3 v3 = v1 + v2;
	cout << v3.x << ", " << v3.y << " mag: " << glm::length(v3) << endl;
	getchar();


	return 0;
}