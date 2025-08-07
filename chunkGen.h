#ifndef CHUNKGEN_H
#define CHUNKGEN_H

#include <vector>
#include "Blocco.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

class Chunk
{

	const int sizeX = 16;
	const int sizeY = 128;
	const int sizeZ = 16;

	float contrasto = 1.25f;
	float xU = 16.0f / 256.0f;
	float yU = 16.0f / 256.0f;

	GLfloat TriangleVertices[216] =
	{
		-0.5f,-0.5f ,  0.5f,		0.025f* contrasto, 0.025f* contrasto, 0.025f* contrasto,	 (xU / 16) + xU*3, (yU / 16) + yU*15, 0.0f,
		-0.5f, 0.5f ,  0.5f,		0.025f* contrasto, 0.025f* contrasto, 0.025f* contrasto,	 (xU / 16) + xU*3, yU*16, 1.0f,
		 0.5f, -0.5f,  0.5f,		0.025f* contrasto, 0.025f* contrasto, 0.025f* contrasto,	 xU*4,(yU / 16) + yU*15, 2.0f,
		 0.5f, 0.5f,   0.5f,		0.025f* contrasto, 0.025f* contrasto, 0.025f* contrasto,	 xU*4, yU*16, 3.0f,

		 -0.5f,-0.5f ,  -0.5f,		-0.075f* contrasto, -0.075f* contrasto, -0.075f* contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4, yU * 16,3.0f,

		 0.5f,-0.5f ,  0.5f,		-0.04f* contrasto, -0.04f* contrasto, -0.04f* contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.0f,
		0.5f, 0.5f ,  0.5f,			-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		0.06f* contrasto, 0.06f* contrasto, 0.06f* contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.f,
		-0.5f, 0.5f ,  0.5f,		0.06f * contrasto, 0.06f * contrasto, 0.06f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 -0.5f, -0.5f,  -0.5f,		0.06f * contrasto, 0.06f * contrasto, 0.06f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 -0.5f, 0.5f,   -0.5f,		0.06f * contrasto, 0.06f * contrasto, 0.06f * contrasto,	xU * 4, yU * 16,3.0f,

		  -0.5f, 0.5f ,  0.5f,		0.04f* contrasto, 0.04f* contrasto, 0.04f* contrasto,		0, (yU / 16.0f) + yU * 15,0.0f,
		-0.5f, 0.5f ,  -0.5f,		0.04f * contrasto, 0.04f * contrasto, 0.04f * contrasto,	0, yU * 16,1.0f,
		 0.5f, 0.5f,   0.5f,		0.04f * contrasto, 0.04f * contrasto, 0.04f * contrasto,	  xU,(yU / 16.0f) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		0.04f * contrasto, 0.04f * contrasto, 0.04f * contrasto,	  xU, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		-0.115f* contrasto, -0.115f* contrasto, -0.115f* contrasto,		(xU / 16) + xU*2,(yU / 16) + yU*15,0.0f,
		-0.5f, -0.5f ,  -0.5f,		-0.115f* contrasto, -0.115f* contrasto, -0.115f* contrasto,		(xU / 16) + xU*2, yU*16,1.0f,
		 0.5f, -0.5f,  0.5f,		-0.115f* contrasto, -0.115f* contrasto, -0.115f* contrasto,		xU*3, (yU / 16) + yU*15,2.0f,
		 0.5f, -0.5f,   -0.5f,		-0.115f* contrasto, -0.115f* contrasto, -0.115f* contrasto,		xU*3, yU*16,3.0f
	};

	GLuint* ind = new GLuint[2];
	int indexNumbers = 0;

public:

	bool posIsBlock[16*16*128];
	bool created = false;

	int x = 0;
	int z = 0;

	int highLightedBlock[4] = { 0,0,0,0};

	std::vector<Blocco> blocco;
	std::vector<GLuint> indices0;
	std::vector<float> squareVertices;
	

	Chunk() = default;
	void Create(int x, int y, GLuint vbo, GLuint ebo);
	void Update(int x, int z, int y, bool build, GLuint ebo, GLuint vbo);
	void highLight(int x, int z, int y, GLuint vbo);
	void Render(VAO vao, VBO vbo, EBO ebo);

	
};

#endif