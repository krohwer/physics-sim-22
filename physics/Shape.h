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
		type = Type::POLYGON;
		//SetBox();
	}

	Shape(float shapeInt) {
		if (Equal(shapeInt, 0.0))
			type = Type::POLYGON;
		else
			type = Type::BALL;
	}

// 	Shape(glm::vec3 vertexArray[]) {
// 		if (sizeof(vertexArray) <= MaxVertexCount)
// 			m_vertexCount = sizeof(vertexArray);
// 		else
// 			m_vertexCount = MaxVertexCount;
// 
// 		for (int i = 0; i < m_vertexCount; i++) {
// 			m_vertices[i] = vertexArray[i];
// 		}
// 		type = Type::POLYGON;
// 	}

	/// Enumerated type for the shape type
	enum class Type { POLYGON = 0, BALL = 1 };

	// pointer to the body associated with this shape instance
	Body* body;

	// enum to identify the shape easily
	Type type;

// 	int m_vertexCount;
// 	glm::vec3 m_vertices[MaxVertexCount];
// 	glm::vec3 m_normals[MaxVertexCount];

// 	void SetBox() {
// 		type = Type::POLYGON;
// 
// 		m_vertexCount = 4;
// 		m_vertices[0] = glm::vec3(-0.5, -0.5, 0.0f);
// 		m_vertices[1] = glm::vec3(0.5, -0.5, 0.0f);
// 		m_vertices[2] = glm::vec3(0.5, 0.5, 0.0f);
// 		m_vertices[3] = glm::vec3(-0.5, 0.5, 0.0f);
// 		m_normals[0] = glm::vec3(0.0f, -1.0f, 0.0f);
// 		m_normals[1] = glm::vec3(1.0f, 0.0f, 0.0f);
// 		m_normals[2] = glm::vec3(0.0f, 1.0f, 0.0f);
// 		m_normals[3] = glm::vec3(-1.0f, 0.0f, 0.0f);
// 	}
// 
// 	void scaleX(glm::vec3 scale) {
// 		for (int i = 0; i < m_vertexCount; i++) {
// 			m_vertices[i].x *= scale.x;
// 		}
// 	}
// 
// 	void scaleY(glm::vec3 scale) {
// 		for (int i = 0; i < m_vertexCount; i++) {
// 			m_vertices[i].y *= scale.y;
// 		}
// 	}

	/**
	 * duplicates the shape and returns that duplicate
	 */
	Shape* Clone() {
		Shape* poly = new Shape();
		poly->type = type;
// 		for (int i = 0; i < m_vertexCount; i++) {
// 			poly->m_vertices[i] = m_vertices[i];
// 			poly->m_normals[i] = m_normals[i];
// 		}
// 		poly->m_vertexCount = m_vertexCount;
		return poly;
	}
};
#endif