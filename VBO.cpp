#include "VBO.h"

void VBO::Gen(GLfloat* vertex, GLsizeiptr size)
{

	glGenBuffers(2, ids);

	allVertices.resize(9);

		glBindBuffer(GL_ARRAY_BUFFER, ids[0]);
		glBufferData(GL_ARRAY_BUFFER, size, vertex, GL_STATIC_DRAW);
	

}

void VBO::mergeMeshes()
{

	int count = 0;
	int totSize = 0;

	for (int i = 0; i < 8; i++)
	{
		totSize += allVertices[i].size();
	}

	allVertices[8].resize(totSize);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < allVertices[i].size(); j++)
		{
			allVertices[8][count] = allVertices[i][j];
			count++;
		}

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