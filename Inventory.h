#pragma once

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "Texture.h"
#include "shaderClass.h"
#include "UVcord.h"
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "vars.h"

class Inventory
{
public:

	float x = 0.5f;
	float yC = -0.83f;
	float y = (x * 2) / 9;

	float scale = x * 0.11f;

	int blocks[9] = { 1,2,3,4,0,0,0,0,0};

	std::vector<std::vector<float>> blocksVertices;
	std::vector<float> _blocksVertices;
	std::vector<float> rotatedBlockVertices;
	std::vector<int> _blocksIndices;

	float uY = 1.0f / 46;
	float uX = 1.0f / 182;

	camera cam;
	uvCord uv;

	GLfloat hotBarVertices[36]
	{
		-x, y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 4.0f,
		x, y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 4.0f,
		-x, -y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f, uY * 22, 4.0f,
		x, -y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 1.0f, uY * 22, 4.0f


	};

	GLfloat selectedSlot[36]
	{

		-x, y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 4.0f,
		-x + (x / 4.5f), y + yC, 0.99f, 0.0f, 0.0f, 0.0f, uX * 23, 1.0f, 4.0f,
		-x, -y + yC, 0.99f, 0.0f, 0.0f, 0.0f, 0.0f, uY * 22, 4.0f,
		-x + (x / 4.5f), -y + yC, 0.99f, 0.0f, 0.0f, 0.0f,  uX * 23, uY * 22, 4.0f


	};


	GLuint hotBarIndices[6]
	{
		0,1,2,
		1,2,3,

	};

	VBO hotBarVBO = {};
	EBO hotBarEBO = {};

	VBO selectedSlotVBO = {};
	EBO selectedSlotEBO = {};

	VBO blocksVBO = {};
	EBO blocksEBO = {};

	Inventory();

	void changeSelectedBlock(int blockPos);

	void renderHotbar(VAO vao, Texture hotbarTexture);
	void renderSelectedSlot(VAO vao, Texture hotbarTexture);
	void updateBlocksHotbar(int blockPos, int blockID);
	void renderBlocksHotbar(VAO vao);

	void renderLowInv(VAO vao, Texture hotbarTexture, Texture grassBlock);

};