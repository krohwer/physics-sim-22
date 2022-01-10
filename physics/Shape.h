#pragma once

#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>

constexpr auto MaxVertexCount = 4;

struct Shape {
	/// Enumerated type for the shape type
	enum class Type { POLYGON = 0, BALL = 1 };

	Type type;

	int m_vertexCount;
	glm::vec3 m_vertices[MaxVertexCount];
	glm::vec3 m_normals[MaxVertexCount];

	void SetBox() {
		type = Type::POLYGON;

		m_vertexCount = 4;
		m_vertices[0] = glm::vec3(-0.5, -0.5, 0.0f);
		m_vertices[1] = glm::vec3(0.5, -0.5, 0.0f);
		m_vertices[2] = glm::vec3(0.5, 0.5, 0.0f);
		m_vertices[3] = glm::vec3(-0.5, 0.5, 0.0f);
		m_normals[0] = glm::vec3(0.0f, -1.0f, 0.0f);
		m_normals[1] = glm::vec3(1.0f, 0.0f, 0.0f);
		m_normals[2] = glm::vec3(0.0f, 1.0f, 0.0f);
		m_normals[3] = glm::vec3(-1.0f, 0.0f, 0.0f);
	}

	void scaleX(glm::vec3 scale) {
		for (int i = 0; i < m_vertexCount; i++) {
			m_vertices[i].x *= scale.x;
		}
	}

	void scaleY(glm::vec3 scale) {
		for (int i = 0; i < m_vertexCount; i++) {
			m_vertices[i].y *= scale.y;
		}
	}

	/**
	 * Get the extreme point along a direction within a polygon
	 * Usage: Polygonal collisions
	 * 
	 * CREDIT: Dirk Gregorius for describing this method
	 */
	glm::vec3 GetSupport(glm::vec3 direction)
	{
		float bestProjection = -FLT_MAX;
		glm::vec3 bestVertex = m_vertices[0];

		for (int i = 1; i < m_vertexCount; ++i)
		{
			glm::vec3 vertex = m_vertices[i];
			float projection = glm::dot(vertex, direction);

			if (projection > bestProjection)
			{
				bestVertex = vertex;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}
};

#endif