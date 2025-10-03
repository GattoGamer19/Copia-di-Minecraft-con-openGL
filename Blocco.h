#ifndef BLOCCO_H
#define BLOCCO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.h"
#include "VBO.h"

class Blocco
{

public:

	bool RenderFront = true;
	bool RenderBack = true;
	bool RenderRight = true;
	bool RenderLeft = true;
	bool RenderTop = true;
	bool RenderBottom = true;

	GLuint textureID = 1;

	char chunkReference = 0;

	char id = 0;

	Blocco();
	Blocco(int chunkReference);
	void Render(VAO vao, VBO vbo);

};

#endif