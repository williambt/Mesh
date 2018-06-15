#include "VertexArray.h"

#include <glad/glad.h>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &_vao);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &_vao);
}

void VertexArray::Bind()
{
	glBindVertexArray(_vao);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

void VertexArray::AddBuffers(VertexBuffer & vb, const BufferLayout & layout)
{
	Bind();
	vb.Bind();

	const std::vector<LayoutElement>& elements = layout.GetElements();

	unsigned int offset = 0;

	unsigned int size = elements.size();
	for (int i = 0; i < size; ++i)
	{
		const LayoutElement& element = elements[i];
		glEnableVertexAttribArray(element.attribType);
		glVertexAttribPointer(element.attribType, element.size, element.type, GL_FALSE, layout.GetStride(), (const void*)offset);
		offset += element.size * element.GetTypeSize();
	}
}
