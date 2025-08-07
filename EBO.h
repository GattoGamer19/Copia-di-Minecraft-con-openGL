#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	GLuint id = 0;

	EBO() = default;

	void Gen(GLuint* Indices, GLsizeiptr size);

	void Unbind();
	void Bind();
	void Delete();

};

#endif