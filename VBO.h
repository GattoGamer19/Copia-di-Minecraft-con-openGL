#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
public:
	GLuint ids[1000];

	VBO(GLfloat* vertex, GLsizeiptr size);

	void Unbind();
	void Bind(GLuint id);
	void Delete(GLuint id);

};

#endif
