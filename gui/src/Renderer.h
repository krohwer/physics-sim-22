#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "PhysicsObject.h"
#include "Camera.h"

/**
 * The job of a renderer is to take in some stuff and make it appear on the screen.
 */
class Renderer {
public:
	Renderer();
	void clear() const;
	void setMVP(Shader& shader, const Camera& camera, const Body& body);
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};