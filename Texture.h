#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb/stb_image.h"	
#include <glad/glad.h>

class Texture
{
	int larghezza, altezza, colCh;
	unsigned char* bytes;
	GLuint tex0uni;

	public:

		GLuint id;

		Texture(GLuint shaderID, const char* imageName);

		void Bind();
		void UnBind();


};

#endif