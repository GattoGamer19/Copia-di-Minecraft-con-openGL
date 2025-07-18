#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"
#include "EBO.h"

class VAO
{
public:
	GLuint id;

	VAO();

	void LinkVBO(VBO VBO, int nDati, GLuint layout, GLuint id, GLuint stride, int offset);
	void LinkEBO(EBO EBO, GLuint layout);

	void Unbind();
	void Bind();
	void Delete();

};

#endif

