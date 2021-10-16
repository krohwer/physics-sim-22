#pragma once

class IndexBuffer {
	/// Identifies the index buffer with a unique ID
	unsigned int m_RendererID;
	/// Identifies how many indices the index buffer has
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_Count; };

	// eventually might want the ability to lock, modify, and unlock for streaming data to the buffer
};