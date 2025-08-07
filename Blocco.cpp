#include "Blocco.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

Blocco::Blocco() : chunkReference(0) {};
Blocco::Blocco(int n) : chunkReference(n) {};

void Blocco::Render(VAO vao, VBO vbo)
{
		
	glBindTexture(GL_TEXTURE_2D, textureID);

	int chunkReference1 = chunkReference;
	chunkReference *= 6;

	if (RenderFront)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference++;

	}

	else{ chunkReference++; }

	if (RenderBack)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference++;
	}

	else { chunkReference++; }

	if (RenderRight)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference++;
	}

	else { chunkReference++; }

	if (RenderLeft)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference++;
	}

	else { chunkReference++; }


	if (RenderTop)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference++;
	}

	else { chunkReference++; }

	if (RenderBottom)
	{
		vbo.Bind(vbo.ids[chunkReference]);
		vao.LinkVBO(vbo, 3, 0, vbo.ids[chunkReference], 8, 0);
		vao.LinkVBO(vbo, 3, 1, vbo.ids[chunkReference], 8, 3);
		vao.LinkVBO(vbo, 2, 2, vbo.ids[chunkReference], 8, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		chunkReference = chunkReference1;
	}

	else { chunkReference = chunkReference1; }
	
}

