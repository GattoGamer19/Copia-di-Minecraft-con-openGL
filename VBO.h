#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class VBO
{
public:

	int x = 0;
	int z = 0;
	GLuint ids[2] = { 0,0 };

	std::vector<std::vector<float>> allVertices;
	int meshCount = 0;

	bool verticesInizialized = false;

	int boh = 0;

	bool rendered = false;


	VBO() = default; 

	void Gen(GLfloat* vertex, GLsizeiptr size);

	void mergeMeshes();
	void Unbind();
	void Bind(GLuint id);
	void Delete(GLuint id);

};

#endif
