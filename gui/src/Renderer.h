#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

/**
 * The job of a renderer is to take in some stuff and make it appear on the screen.
 */
class Renderer {
public:
	void clear() const;
	void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};