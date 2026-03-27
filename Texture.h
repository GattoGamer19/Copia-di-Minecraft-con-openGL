#ifndef TEXTURE_H
#define TEXTURE_H
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE

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