#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
public:
	int x = 0;
	int z = 0;
	GLuint ids[2];

	VBO() = default; 

	void Gen(GLfloat* vertex, GLsizeiptr size);

	void Unbind();
	void Bind(GLuint id);
	void Delete(GLuint id);

};

#endif
