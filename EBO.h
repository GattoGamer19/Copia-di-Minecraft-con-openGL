#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class EBO
{
public:
	GLuint id = 0;

	std::vector<std::vector<GLuint>> indices;

	EBO() = default;

	void Gen(GLuint* Indices, GLsizeiptr size);

	void calcIndices(int sizeVertices);
	void Unbind();
	void Bind();
	void Delete();

};

#endif