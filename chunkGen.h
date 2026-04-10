#ifndef CHUNKGEN_H
#define CHUNKGEN_H

#include <vector>
#include "Blocco.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include <unordered_map>

#define chunkSize 32

class Chunk
{

private:
	int sizeX = chunkSize;
	int sizeZ = chunkSize;
	int offsetX = 8;
	int offsetZ = 8;

	int minHeight = 40;

	float contrasto = 4.0f;
	

	float xU = 16.0f / 256.0f;
	float yU = 16.0f / 256.0f;

	

	int indexNumbers = 0;
	float shadowValue = 30;
	
	int verticesSize = 0;

	float shift = 0.003f;

public:

	

	GLfloat TriangleVertices[216] =
	{
		-0.5f,-0.5f ,  0.5f,		(-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto,	 (xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  0.5f,		(-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto,	 (xU / 16) + xU * 3, yU * 16, 1.0f,
		 0.5f, -0.5f,  0.5f,		(-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto,	 xU * 4,(yU / 16) + yU * 15, 2.0f,
		 0.5f, 0.5f,   0.5f,		(-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto, (-0.03f + shift) * contrasto,	 xU * 4, yU * 16, 3.0f,
									
		 -0.5f,-0.5f ,  -0.5f,		(-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  -0.5f,		(-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		(-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		(-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto, (-0.075f + shift) * contrasto,	xU * 4, yU * 16,3.0f,
									
		 0.5f,-0.5f ,  0.5f,		(-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.0f,
		0.5f, 0.5f ,  0.5f,			(-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		(-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		(-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto, (-0.04f + shift) * contrasto,	xU * 4, yU * 16,3.0f,
									
		 -0.5f,-0.5f ,  0.5f,		(-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.f,
		-0.5f, 0.5f ,  0.5f,		(-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 -0.5f, -0.5f,  -0.5f,		(-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 -0.5f, 0.5f,   -0.5f,		(-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto, (-0.05f + shift) * contrasto,	xU * 4, yU * 16,3.0f,
									
		  -0.5f, 0.5f ,  0.5f,		(0.0f + shift)* contrasto, (0.0f  + shift) * contrasto, (0.0f + shift) * contrasto,		0, (yU / 16.0f) + yU * 15,0.0f,
		-0.5f, 0.5f ,  -0.5f,		(0.0f + shift)* contrasto, (0.0f  + shift) * contrasto, (0.0f + shift) * contrasto,	0, yU * 16,1.0f,
		 0.5f, 0.5f,   0.5f,		(0.0f + shift)* contrasto, (0.0f  + shift) * contrasto, (0.0f + shift) * contrasto,	  xU,(yU / 16.0f) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		(0.0f + shift)* contrasto, (0.0f  + shift) * contrasto, (0.0f + shift) * contrasto,	  xU, yU * 16,3.0f,
									
		 -0.5f,-0.5f ,  0.5f,		(-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto,		(xU / 16) + xU * 2,(yU / 16) + yU * 15,0.0f,
		-0.5f, -0.5f ,  -0.5f,		(-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto,		(xU / 16) + xU * 2, yU * 16,1.0f,
		 0.5f, -0.5f,  0.5f,		(-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto,		xU * 3, (yU / 16) + yU * 15,2.0f,
		 0.5f, -0.5f,   -0.5f,		(-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto, (-0.08f + shift) * contrasto,		xU * 3, yU * 16,3.0f
	};

	long long seed = 3284157443;

	int size = 1;
	float scale = 1;

	int offsetC = 0;

	bool created = false;
	bool ready = false;

	int x = 0;
	int z = 0;
	int id = 1;

	int highLightedBlock[4] = { 0,0,0,0};
	int indexNumber = 0;

	std::vector<Blocco> blocco;
	std::vector<GLuint> indices0;
	std::vector<float> squareVertices;
	
	std::vector<unsigned char> posIsBlock;

	Chunk() = default;
	int getBlock(int x, int z, int y);
	void placeTree(std::vector<unsigned char>& posIsBlock, std::vector<Blocco>& blocco, int x, int z, int y);
	void Create(int x, int y, VBO& vbo, EBO& ebo, std::vector<float>& allVertices, bool hasBeenModified, int offset);
	void preRender(GLuint vbo, std::vector<float>& allVertices, EBO& ebo, int _verticesSize);
	void Update(int x, int z, int y, bool build, GLuint ebo, GLuint vbo, std::vector<float>& allVertices, int id, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);
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