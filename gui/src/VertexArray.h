#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	// since the vertex array stores the state further, we just have to say here's a buffer and here's a layout.  make it work
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};