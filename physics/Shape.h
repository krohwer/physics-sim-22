#pragma once

#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>
#include "PhysicsMath.h"
#include "PhysicsObject.h"

constexpr auto MaxVertexCount = 4;

struct Shape {
	// constructors
	Shape() {
		SetBox();
	}

//	Shape(glm::vec3 vertexArray[]) {
//		if (sizeof(vertexArray) <= MaxVertexCount)
//			vertexCount = sizeof(vertexArray);
//		else
//			vertexCount = MaxVertexCount;
//
//		for (int i = 0; i < vertexCount; i++) {
//			vertices[i] = vertexArray[i];
//		}
//		type = Type::POLYGON;
//	}

	/// Enumerated type for the shape type
	enum class Type { POLYGON = 0, BALL = 1 };

	// pointer to the body associated with this shape instance
	Body* body;

	// enum to identify the shape easily
	Type type;

//	int vertexCount;
//	glm::vec3 vertices[MaxVertexCount];
//	glm::vec3 normals[MaxVertexCount];

	void SetBox() {
		type = Type::POLYGON;

//		vertexCount = 4;
//		vertices[0] = glm::vec3(-0.5, -0.5, 0.0f);
//		vertices[1] = glm::vec3(0.5, -0.5, 0.0f);
//		vertices[2] = glm::vec3(0.5, 0.5, 0.0f);
//		vertices[3] = glm::vec3(-0.5, 0.5, 0.0f);
//		normals[0] = glm::vec3(0.0f, -1.0f, 0.0f);
//		normals[1] = glm::vec3(1.0f, 0.0f, 0.0f);
//		normals[2] = glm::vec3(0.0f, 1.0f, 0.0f);
//		normals[3] = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

//	void scaleX(glm::vec3 scale) {
//		for (int i = 0; i < vertexCount; i++) {
//			vertices[i].x *= scale.x;
//		}
//	}
//
//	void scaleY(glm::vec3 scale) {
//		for (int i = 0; i < vertexCount; i++) {
//			vertices[i].y *= scale.y;
//		}
//	}

	/**
	 * duplicates the shape and returns that duplicate
	 */
	Shape* Clone() {
		Shape* poly = new Shape();
//		for (int i = 0; i < vertexCount; i++) {
//			poly->vertices[i] = vertices[i];
//			poly->normals[i] = normals[i];
//		}
//		poly->vertexCount = vertexCount;
		poly->type = type;
		return poly;
	}
};

#endif