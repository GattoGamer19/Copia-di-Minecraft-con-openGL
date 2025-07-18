#include "VBO.h"

VBO::VBO(GLfloat* vertex, GLsizeiptr size)
{

	glGenBuffers(1000, ids);

	for (int i = 0; i < 1000; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, ids[i]);
		glBufferData(GL_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW);
	}
	
	
	

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