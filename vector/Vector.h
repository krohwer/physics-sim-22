#pragma once
#ifndef VECTOR_H
#define VECTOR_H

class Vector {
public:
	double x, y;
	Vector(double xCoordinate, double yCoordinate);
	double magnitude();
};

Vector add(Vector vector1, Vector vector2);

Vector sub(Vector vector1, Vector vector2);

Vector scalarMult(Vector vector, double scalar);

double dotProduct(Vector vector1, Vector vector2);

#endif