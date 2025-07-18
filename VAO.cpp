#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &id);
	
}

void VAO::Bind()
{
	glBindVertexArray(id);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &id);
}

void VAO::LinkVBO(VBO VBO, int nDati, GLuint layout, GLuint id, GLuint stride, int offset)
{
	VBO.Bind(id);
    glVertexAttribPointer(layout, nDati, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::LinkEBO(EBO EBO, GLuint layout)
{
	EBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(layout);
	EBO.Unbind();
}
