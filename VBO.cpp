#include "VBO.h"

void VBO::Gen(GLfloat* vertex, GLsizeiptr size)
{

	glGenBuffers(2, ids);


		glBindBuffer(GL_ARRAY_BUFFER, ids[0]);
		glBufferData(GL_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW);
	

}

void VBO::Bind(GLuint id)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(GLuint id)
{
	glDeleteBuffers(1, &id);
}