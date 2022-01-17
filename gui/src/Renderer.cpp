#include "pch.h" // uses glew 2.1.0 in precompiled header

#include "Renderer.h"

Renderer::Renderer() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear() const {

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {

	// bind the shader
	shader.bind();
	// before drawing, bind the vertex array, and the index buffer
	// using a VAO in this way is helpful for rendering multiple objects
	va.bind();
	ib.bind();

	// drawing in triangles for now, then the number of INDICES (not vertices), and the data type of the indices.
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}