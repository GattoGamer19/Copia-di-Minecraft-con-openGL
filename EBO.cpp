#include "EBO.h"

void EBO::Gen(GLuint* Indices, GLsizeiptr size)
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, Indices, GL_STATIC_DRAW);
	indices.resize(9);

}

void EBO::calcIndices(int sizeVertices)
{
	int iCount = 0;

	indices[8].resize(sizeVertices / 6);

	for (int i = 0; i < sizeVertices / 6; i += 6)
	{
		indices[8][i] = iCount;
		indices[8][i + 1] = 1 + iCount;
		indices[8][i + 2] = 2 + iCount;
		indices[8][i + 3] = 1 + iCount;
		indices[8][i + 4] = 2 + iCount;
		indices[8][i + 5] = 3 + iCount;
		iCount += 4;
	}
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &id);
}