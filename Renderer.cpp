#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Renderer.h"
#include <glad/glad.h>

void RenderObjects(VAO vao, VBO vbo, int vboIND, EBO ebo, int indexNumbers)
{
	ebo.Bind();
	vao.LinkVBO(vbo, 3, 0, vbo.ids[vboIND], 9, 0, GL_FLOAT);
	vao.LinkVBO(vbo, 3, 1, vbo.ids[vboIND], 9, 3, GL_FLOAT);
	vao.LinkVBO(vbo, 2, 2, vbo.ids[vboIND], 9, 6, GL_FLOAT);
	vao.LinkVBO(vbo, 1, 3, vbo.ids[vboIND], 9, 8, GL_FLOAT);
	glDrawElements(GL_TRIANGLES, indexNumbers, GL_UNSIGNED_INT, 0);
	ebo.Unbind();
}

void RenderLines(VAO vao, VBO vbo, int vboIND, EBO ebo, int indexNumbers)
{
	ebo.Bind();
	vao.LinkVBO(vbo, 3, 0, vbo.ids[vboIND], 9, 0, GL_FLOAT);
	vao.LinkVBO(vbo, 3, 1, vbo.ids[vboIND], 9, 3, GL_FLOAT);
	vao.LinkVBO(vbo, 2, 2, vbo.ids[vboIND], 9, 6, GL_FLOAT);
	vao.LinkVBO(vbo, 1, 3, vbo.ids[vboIND], 9, 8, GL_FLOAT);
	glDrawElements(GL_LINES, indexNumbers, GL_UNSIGNED_INT, 0);
	ebo.Unbind();
}