#include "Vector.h"
#include <math.h>
#include <iostream>

using namespace std;

int main() {
	Vector v1 = Vector::Vector(2, 3);
	Vector v2 = Vector::Vector(5, 1);
	
	cout << "Your Vectors:" << endl;
	cout << v1.x << ", " << v1.y << " mag: " << v1.Vector::magnitude() << endl;
	cout << v2.x << ", " << v2.y << " mag: " << v2.Vector::magnitude() << endl;

	Vector v3 = add(v1, v2);
	cout << v3.x << ", " << v3.y << " mag: " << v3.Vector::magnitude() << endl;
	getchar();


	return 0;
}