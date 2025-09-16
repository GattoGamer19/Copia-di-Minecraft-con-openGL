#ifndef CHUNKGEN_H
#define CHUNKGEN_H

#include <vector>
#include "Blocco.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"

class Chunk
{

	int sizeX = 16;
	int sizeY = 128;
	int sizeZ = 16;

	float contrasto = 2.0f;
	float xU = 16.0f / 256.0f;
	float yU = 16.0f / 256.0f;

	

	int indexNumbers = 0;
	float shadowValue = 15;

public:

	GLfloat TriangleVertices[216] =
	{
		-0.5f,-0.5f ,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 (xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 (xU / 16) + xU * 3, yU * 16, 1.0f,
		 0.5f, -0.5f,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 xU * 4,(yU / 16) + yU * 15, 2.0f,
		 0.5f, 0.5f,   0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 xU * 4, yU * 16, 3.0f,

		 -0.5f,-0.5f ,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4, yU * 16,3.0f,

		 0.5f,-0.5f ,  0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.0f,
		0.5f, 0.5f ,  0.5f,			-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.f,
		-0.5f, 0.5f ,  0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 -0.5f, -0.5f,  -0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 -0.5f, 0.5f,   -0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	xU * 4, yU * 16,3.0f,

		  -0.5f, 0.5f ,  0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,		0, (yU / 16.0f) + yU * 15,0.0f,
		-0.5f, 0.5f ,  -0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	0, yU * 16,1.0f,
		 0.5f, 0.5f,   0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	  xU,(yU / 16.0f) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	  xU, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		-0.08f * contrasto, -0.08f * contrasto, -0.08f * contrasto,		(xU / 16) + xU * 2,(yU / 16) + yU * 15,0.0f,
		-0.5f, -0.5f ,  -0.5f,		-0.08f * contrasto, -0.08f * contrasto, -0.08f * contrasto,		(xU / 16) + xU * 2, yU * 16,1.0f,
		 0.5f, -0.5f,  0.5f,		-0.08f * contrasto, -0.08f * contrasto, -0.08f * contrasto,		xU * 3, (yU / 16) + yU * 15,2.0f,
		 0.5f, -0.5f,   -0.5f,		-0.08f * contrasto, -0.08f * contrasto, -0.08f * contrasto,		xU * 3, yU * 16,3.0f
	};

	int seed = 85209;

	int size = 1;
	int scale = 1;

	bool created = false;
	bool ready = false;

	int x = 0;
	int z = 0;
	int id = 1;

	int highLightedBlock[4] = { 0,0,0,0};

	std::vector<Blocco> blocco;
	std::vector<GLuint> indices0;
	std::vector<float> squareVertices;
	
	int posIsBlock[16 * 16 * 128];

	Chunk() = default;
	void Create(int x, int y, GLuint vbo, GLuint ebo, std::vector<float>& allVertices);
	void preRender(GLuint vbo, std::vector<float>& allVertices, GLuint ebo);
	void Update(int x, int z, int y, bool build, GLuint ebo, GLuint vbo, std::vector<float>& allVertices, int id);
	void highLight(int x, int z, int y, GLuint vbo);
	void Render(VAO vao, VBO vbo, EBO ebo);
	void addShadowTop(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);
	void addShadowBottom(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);
	void addShadowFront(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);
	void addShadowBack(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);
	void addShadowRight(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);
	void addShadowLeft(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z);

	
};

#endif