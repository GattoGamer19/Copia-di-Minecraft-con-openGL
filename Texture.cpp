#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(GLuint shaderID, const char* imageName)
{
	bytes = stbi_load(imageName, &larghezza, &altezza, &colCh, 4);

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, larghezza, altezza, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	tex0uni = glGetUniformLocation(shaderID, "tex0");

	glUniform1i(tex0uni, 0);

	std::cout << id<< " ";
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}