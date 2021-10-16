#include "pch.h"

#include "VertexArray.h"

#include "GLErrorManager.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {

		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		// Once the buffer is bound, we need to tell OpenGL what the layout of the buffer is
		// Need one of these for each attribute of a vertex (position, texture, color, normal, etc)
		// Parameters: index is the beginning position of the attribute in the vertex
		//			   size is the the component count of the attribute
		//			   the type is the type of data
		//			   do you want them normalized?
		//			   stride amount you need to go forward to get to the next vertex
		//			   pointer is the amount to the next attribute.  If you have a struct for your vertex, you can use offsetof()
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);	// with this line, we say index 0 of this vertex array is bound to the currently bound array buffer.  This links the buffer to the VAO.  This can be used to bind multiple vertex buffers to a single VAO.
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(m_RendererID);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}
