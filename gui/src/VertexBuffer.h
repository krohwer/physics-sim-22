#pragma once

class VertexBuffer {
	/// Identifies the vertex buffer with a unique ID
	unsigned int m_RendererID;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	~VertexBuffer();

	void bind() const;
	void unbind() const;

	// eventually might want the ability to lock, modify, and unlock for streaming data to the buffer
};