#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "stb/stb_image.h"	
#include "Texture.h"
#include <math.h>
#include "Comandi.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Blocco.h"
#include <vector>
#include "chunkGen.h"
#include "time.h"
#include <thread>
#include <unordered_map>
#include <map>
#include "UVcord.h"

float PI = 3.141592653589793;

bool collisionEnabled = true;

int nChunkX = 15;
int rChunkX = 15;
int nChunkZ = 15;
int rChunkZ = 15;
int offset = floor((float)nChunkX / 2);

float fallingSpeed = 0;

float altezza = 1080;
float larghezza = 1920;
float aSensZoom = 0.1;
float position[3] = { 5+(offset*16),5+(offset*16),69};
int currentChunk[2] = { 0,0 };
float chunkPosition[3] = { 0,0,0 };
float prevSelectedBlock[3] = { 0,0,0 };
float prevSelectedChunk[2] = { 0,0 };
float yRotation = 0;
float xRotation = 0;
float sensRotazione = 0;
float sensX = 0;
float sensY = 0;
int asse = 0;
int placeBlockPosition[3] = { 0,0,0 };
int selectedBlockPosition[3] = { 0,0,0 };


int nX = 1;
bool renderingX = false;
int nBX = 1;
bool renderingBX = false;
int nZ = 1;
bool renderingZ = false;
int nBZ = 1;
bool renderingBZ = false;

std::vector<int> ChunkVBOX(nChunkX);
std::vector<int> ChunkVBOZ(nChunkX);

int frameSpeedZ = 1;
int frameSpeedX = 1;

float pos0 = position[0];
float pos1 = position[1];
float pos2 = position[2];

int framePassed = 0;

int invBlock = 2;
bool change = false;

camera cam;

glm::mat4 view1;

std::vector<std::vector<VBO>> vbo(nChunkX, std::vector<VBO>(nChunkZ));
std::vector<std::vector<EBO>> ebo(nChunkX, std::vector<EBO>(nChunkZ));
std::vector<std::vector<Chunk>> chunk(nChunkX, std::vector<Chunk>(nChunkZ));

std::vector<std::vector<bool>> isVisible(nChunkX, std::vector<bool>(nChunkZ));
float ray[2] = { 0,0 };

std::unordered_map<int, std::unordered_map<int, bool>> isCreated;
std::vector<bool> base(nChunkX, false);
std::vector<Chunk> baseChunk(1);
std::vector<Chunk> baseChunkArray(nChunkX);

clock_t destroyT = clock();
clock_t before = clock();
clock_t before2 = clock();
clock_t spaceBarT = clock();
clock_t jumpT = clock();
bool isJumping = false;

bool creativeMode = true;
int click = 0;

std::vector<float> allVerticesZ(216*16*16*128);
std::vector<float> allVerticesX(216*16*16*128);

int refreshCountZ = 0;
int refreshCountX = 0;


int regChunkX = 0;
int regChunkX1 = 0;
int regChunkBX = 0;
int regChunkBX1 = 0;
int regChunkZ = 0;
int regChunkZ1 = 0;
int regChunkBZ = 0;
int regChunkBZ1 = 0;


	void moveArray(std::vector<int>& array, int size, int dir)
	{
		int n = -1;
		int n1 = -1;
		int first = 0;
		int last = 0;
		for (int i = 0; i < size; i++)
		{
			if (dir < 0)
			{
				if (i == 0)
				{
					first = array[size - 1];
				}

				if (i < size - 1)
				{
					n = array[(size - 1) - i];
					n1 = array[((size - 1) - i) - 1];
					array[((size - 1) - i) - 1] = n;
					array[(size - 1) - i] = n1;
				}
				else
				{
					array[((size - 1) - i)] = first;
				}
			}


			if (dir > 0)
			{
				if (i == 0)
				{
					last = array[0];
				}

				if (i < size - 1)
				{
					n = array[i];
					n1 = array[i + 1];
					array[i + 1] = n;
					array[i] = n1;
				}
				else
				{
					array[i] = last;
				}
			}
		}


	}


void genZ(int offsetZ, int offsetX, int x1, int z1)
{

	int z = ChunkVBOZ[0];
	int x = (x1 + (offsetX)) % nChunkX;

	if (z < 0)
	{
		z = abs(z);
		z %= nChunkX;
		z = (nChunkX)-z;

	}

	if (x < 0)
	{
		x = abs(x);
		x %= nChunkX;
		x = (nChunkX)-x;

	}

	chunk[z][x].Create((x1 + offsetX)-offset,( z1 + offsetZ)-offset, vbo[z][x].ids[1], ebo[z][x].id, allVerticesZ);


}

void genBZ(int offsetZ, int offsetX, int x1, int z1)
{

	int z = ChunkVBOZ[nChunkX-1];
	int x = (x1 + (offsetX)) % nChunkX;

	if (z < 0)
	{
		z = abs(z);
		z %= nChunkX;
		z = (nChunkX)-z;

	}

	if (x < 0)
	{
		x = abs(x);
		x %= nChunkX;
		x = (nChunkX)-x;

	}

	chunk[z][x].Create((x1 + offsetX) - offset, (z1 + offsetZ)-offset, vbo[z][x].ids[1], ebo[z][x].id, allVerticesZ);


}

void genX(int offsetZ, int offsetX, int x1, int z1)
{
	int z = (z1 + (offsetZ)) % nChunkX;
	int x = ChunkVBOX[0];

	if (z < 0)
	{
		z = abs(z);
		z %= nChunkX;
		z = (nChunkX)-z;

	}

	if (x < 0)
	{
		x = abs(x);
		x %= nChunkX;
		x = (nChunkX)-x;

	}
	chunk[z][x].Create((x1 + offsetX) - offset, (z1 + offsetZ) - offset, vbo[z][x].ids[1], ebo[z][x].id, allVerticesX);



}

void genBX(int offsetZ, int offsetX, int x1, int z1)
{
	int z = (z1 + (offsetZ)) % nChunkX;
	int x = ChunkVBOX[nChunkX-1];


	if (z < 0)
	{
		z = abs(z);
		z %= nChunkX;
		z = (nChunkX)-z;

	}

	if (x < 0)
	{
		x = abs(x);
		x %= nChunkX;
		x = (nChunkX)-x;

	}
	chunk[z][x].Create((x1 + offsetX) - offset, (z1 + offsetZ) - offset, vbo[z][x].ids[1], ebo[z][x].id, allVerticesX);

}
bool onGround()
{
	if (collisionEnabled)
	{	

		currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
		currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;

		if (currentChunk[1] < 0)
		{
			currentChunk[1] = abs(currentChunk[1]);
			currentChunk[1] %= nChunkX;
			currentChunk[1] = (nChunkX)-currentChunk[1];

		}

		if (currentChunk[0] < 0)
		{
			currentChunk[0] = abs(currentChunk[0]);
			currentChunk[0] %= nChunkX;
			currentChunk[0] = (nChunkX)-currentChunk[0];

		}

		chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
		chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
		chunkPosition[2] = position[2];
		currentChunk[0] = (((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16) % nChunkZ;
		currentChunk[1] = (((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16) % nChunkX;

		if (currentChunk[1] < 0)
		{
			currentChunk[1] = abs(currentChunk[1]);
			currentChunk[1] %= nChunkX;
			currentChunk[1] = (nChunkX)-currentChunk[1];

		}

		if (currentChunk[0] < 0)
		{
			currentChunk[0] = abs(currentChunk[0]);
			currentChunk[0] %= nChunkX;
			currentChunk[0] = (nChunkX)-currentChunk[0];

		}

		int xOffset = chunkPosition[0] + 0.25f;
		int xOffset1 = chunkPosition[0] - 0.25f;
		int zOffset = chunkPosition[1] + 0.25f;
		int zOffset1 = chunkPosition[1] - 0.25f;

		int cZ = currentChunk[1];
		int cZ1 = currentChunk[1];

		int cX = currentChunk[0];
		int cX1 = currentChunk[0];

		if (chunkPosition[0] < 0.25f)
		{
			xOffset1 = xOffset;
			xOffset = 15;
			cX = currentChunk[0] - 1;
			cX1 = currentChunk[0];
		}

		else if (chunkPosition[0] > 15.75f)
		{
			xOffset = 0;
			cX = currentChunk[0] + 1;
			cX1 = currentChunk[0];
		}

		if (chunkPosition[1] < 0.25f)
		{
			zOffset1 = zOffset;
			zOffset = 15;
			cZ = currentChunk[1] - 1;
			cZ1 = currentChunk[1];
		}

		else if (chunkPosition[1] > 15.75f)
		{
			zOffset = 0;
			cZ = currentChunk[1] + 1;
			cZ1 = currentChunk[1];
		}

		cX %= nChunkX;
		cX1 %= nChunkX;
		cZ %= nChunkX;
		cZ1 %= nChunkX;





		if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 1.8f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}




		if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 1.8f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}




		if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 1.8f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}




		if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 1.8f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}

		return false;
	}

	else return false;
}


bool checkBlock()
{
	if (collisionEnabled)
	{
		currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
		currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;

		if (currentChunk[1] < 0)
		{
			currentChunk[1] = abs(currentChunk[1]);
			currentChunk[1] %= nChunkX;
			currentChunk[1] = (nChunkX)-currentChunk[1];

		}

		if (currentChunk[0] < 0)
		{
			currentChunk[0] = abs(currentChunk[0]);
			currentChunk[0] %= nChunkX;
			currentChunk[0] = (nChunkX)-currentChunk[0];

		}

		chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
		chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
		chunkPosition[2] = position[2];
		currentChunk[0] = (((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16) % nChunkZ;
		currentChunk[1] = (((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16) % nChunkX;

		if (currentChunk[1] < 0)
		{
			currentChunk[1] = abs(currentChunk[1]);
			currentChunk[1] %= nChunkX;
			currentChunk[1] = (nChunkX)-currentChunk[1];

		}

		if (currentChunk[0] < 0)
		{
			currentChunk[0] = abs(currentChunk[0]);
			currentChunk[0] %= nChunkX;
			currentChunk[0] = (nChunkX)-currentChunk[0];

		}

		int xOffset = chunkPosition[0] + 0.25f;
		int xOffset1 = chunkPosition[0] - 0.25f;
		int zOffset = chunkPosition[1] + 0.25f;
		int zOffset1 = chunkPosition[1] - 0.25f;

		int cZ = currentChunk[1];
		int cZ1 = currentChunk[1];

		int cX = currentChunk[0];
		int cX1 = currentChunk[0];

		if (chunkPosition[0] < 0.25f)
		{
			xOffset1 = xOffset;
			xOffset = 15;
			cX = currentChunk[0] - 1;
			cX1 = currentChunk[0];
		}

		else if (chunkPosition[0] > 15.75f)
		{
			xOffset = 0;
			cX = currentChunk[0] + 1;
			cX1 = currentChunk[0];
		}

		if (chunkPosition[1] < 0.25f)
		{
			zOffset1 = zOffset;
			zOffset = 15;
			cZ = currentChunk[1] - 1;
			cZ1 = currentChunk[1];
		}

		else if (chunkPosition[1] > 15.75f)
		{
			zOffset = 0;
			cZ = currentChunk[1] + 1;
			cZ1 = currentChunk[1];
		}

		cX %= nChunkX;
		cX1 %= nChunkX;
		cZ %= nChunkX;
		cZ1 %= nChunkX;


		if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] + 0.20f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}



		if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] + 0.20f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}



		if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] + 0.20f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}



		if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] + 0.20f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}



		if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}



		if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}



		if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}



		if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}


		if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}



		if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}



		if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
		{
			return true;
		}


		if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
		{
			return true;
		}


		if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 1.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
		{
			fallingSpeed = 0;
			return true;
		}


		if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 1.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
		{
			fallingSpeed = 0;
			return true;
		}




		if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 1.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
		{
			fallingSpeed = 0;
			return true;
		}




		if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 1.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
		{
			fallingSpeed = 0;
			return true;
		}


		currentChunk[0] = abs((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
		currentChunk[1] = abs((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;

		return false;
	}

	else return false;
}


void Move(int asse, float sens)
{

	 pos0 = position[0];
	 pos1 = position[1];
	 pos2 = position[2];

	 view1 = cam.view;


	 cam.move(asse, sens/1000, position);


	 currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
	 currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
	 chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
	 chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
	 chunkPosition[2] = position[2];

	if(checkBlock())
	{
		cam.view = view1;
		position[0] = pos0;
		position[1] = pos1;
		position[2] = pos2;
	}



}


int main()
{

	std::cout << "Render Distance: " << std::endl;
	std::cin >> nChunkX;

	rChunkX = nChunkX;
	nChunkZ = nChunkX;
	rChunkZ = nChunkX;

	offset = floor((float)nChunkX / 2);

	position[0] = 5 + (offset * 16);
	position[1] = 5 + (offset * 16);

	pos0 = position[0];
	pos1 = position[1];
	pos2 = position[2];

	ChunkVBOX.resize(nChunkX);
	ChunkVBOZ.resize(nChunkX);
	
	ebo.resize(nChunkX);
	vbo.resize(nChunkX);
	chunk.resize(nChunkX);

	isVisible.resize(nChunkX);

	for (int i = 0; i < nChunkX; i++)
	{
		ebo[i].resize(nChunkX);
		vbo[i].resize(nChunkX);
		chunk[i].resize(nChunkX);

		isVisible[i].resize(nChunkX);
	}



		base.resize(nChunkX);

		baseChunkArray.resize(nChunkX);

		std::cout << "Enter '1' for collision enabled, will also disable infinite world generation and may also crash going negativly on x and z axys" << std::endl;
		std::cout << "Enter '0' to not enable" << std::endl;
		std::cin >> collisionEnabled;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(larghezza, altezza, "Minecraft", NULL, NULL);

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, larghezza, altezza);



	GLfloat TriangleVertices1[] =
	{
		-0.6f,-0.2f ,  0.5f,		1.0f, 1.0f, 1.0f,	 0.0f, 0.333f, 4.0f,
		-0.6f, 0.2f ,  0.5f,		1.0f, 1.0f, 1.0f,	 0.0f, 0.666f,4.0f,
		 0.6f, -0.2f,  0.5f,		1.0f, 1.0f, 1.0f,		0.25f, 0.333f,4.0f,
		 0.6f, 0.2f,   0.5f,		1.0f, 1.0f, 1.0f,		0.25f, 0.666f,4.0f,

		 -0.1f,-1.0f ,  -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.333f,4.0f,
		-0.1f, 1.0f ,  -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.666f,4.0f,
		 0.1f, -1.0f,  -0.5f,		1.0f, 1.0f, 1.0f,			0.25f, 0.333f,4.0f,
		 0.1f, 1.0f,   -0.5f,		1.0f, 1.0f, 1.0f,			0.25f, 0.666f,4.0f

	};

	GLfloat handBlock[216];

	float contrasto = 1;
	float outlineX = 1.0f;
	float outlineY = 1.0f;
	float outlineZ = 1.0f;

	float vx = outlineX / 2.0f;
	float vy = outlineZ / 2.0f;
	float vz = outlineY / 2.0f;

	float posX = 0;
	float posY = 0;
	float posZ = 0;

	float outlineVertices[] = {
		posX + vx, posY + vy, posZ + vz,0,0,0,0,0,0,  // 0: +x,+y,+z
		posX - vx, posY + vy, posZ + vz,0,0,0,0,0,0,  // 1: -x,+y,+z
		posX - vx, posY - vy, posZ + vz,0,0,0,0,0,0,  // 2: -x,-y,+z
		posX + vx, posY - vy, posZ + vz,0,0,0,0,0,0,  // 3: +x,-y,+z
		posX + vx, posY + vy, posZ - vz,0,0,0,0,0,0,  // 4: +x,+y,-z
		posX - vx, posY + vy, posZ - vz,0,0,0,0,0,0,  // 5: -x,+y,-z
		posX - vx, posY - vy, posZ - vz,0,0,0,0,0,0,  // 6: -x,-y,-z
		posX + vx, posY - vy, posZ - vz,0,0,0,0,0,0   // 7: +x,-y,-z
	};


	GLuint outlineIndices[] = {
		// fronte Z+
		0,1,  1,2,  2,3,  3,0,
		// retro Z-
		4,5,  5,6,  6,7,  7,4,
		// collegamenti
		0,4,  1,5,  2,6,  3,7

		//l'unica cosa per cui ho usato chatGPT insieme agli "outlineVertices", a
		// causa di un esaurimento nervoso, 
		// "mi è bastato fare i vertici e gli 
		// indici del cubo su cui si basano i blocchi una volta, non farò la stessa cosa una seconda volta" CIT 03/09/2025 by gigi.

	};

	GLuint Indices[]
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7
	};

	for (int i = 0; i < nChunkX; i++)
	{
		ChunkVBOZ[i] = i;
		ChunkVBOX[i] = i;
	}

	uvCord uv;

	for (int i = 0; i < 216; i+=9)
	{
		handBlock[i] = uv.handBlock[i] + 13.15f;
		handBlock[i+1] = uv.handBlock[i+1] + 3.55f;
		handBlock[i+2] = uv.handBlock[i+2] + 2.9f;
		handBlock[i+3] = uv.handBlock[i+3];
		handBlock[i+4] = uv.handBlock[i+4];
		handBlock[i+5] = uv.handBlock[i+5];

		handBlock[i + 6] = uv.handBlock[i + 6];
		handBlock[i + 7] = uv.handBlock[i + 7];
		handBlock[i+8] = 5.0f;

	}

	glm::mat4x4 R = cam.R;
	glm::mat4x4 R1 = cam.R1;

	cam.proj(60.0f, 0.001f, larghezza, altezza, 10000.0f);
	cam.rotate(0.0f, 1.0f, 0.0f, 15.0f);
	cam.rotate(1.0f, 0.0f, 0.0f, -25.0f);

	for (int i = 0; i < 216; i += 9)
	{
		glm::vec4 R(handBlock[i], handBlock[i + 1], handBlock[i + 2], 1.0f);
		R = cam.R * R;

		handBlock[i] = R.x;
		handBlock[i+1] = R.y;
		handBlock[i+2] = R.z;

		handBlock[i + 6] = uv.uvCords[(((i) / 9) * 2) + (48 * invBlock)];
		handBlock[i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (48 * invBlock)];

	}

	cam.R1 = R1;
	cam.R = R;
	Shader ShaderProgram("default.vert", "default.frag");


	VAO VAO1;
	VAO1.Bind();

	VBO vboCursor;
	vboCursor.Gen(TriangleVertices1, sizeof(TriangleVertices1));
	VBO vboHandBlock;
	vboHandBlock.Gen(handBlock, sizeof(handBlock));
	VBO vboOutline;
	vboOutline.Gen(outlineVertices, sizeof(outlineVertices));


	EBO eboCursor;
	eboCursor.Gen(Indices, sizeof(Indices));
	EBO eboHandBlock;
	eboHandBlock.Gen(uv.indices, sizeof(uv.indices));
	EBO eboOutline;
	eboOutline.Gen(outlineIndices, sizeof(outlineIndices));

	



	for (int j = 0; j < nChunkZ ;j++)
	{
		for (int i = 0; i < nChunkX; i++)
		{
			vbo[j][i].Gen(TriangleVertices1, sizeof(TriangleVertices1));
			isVisible[j][i] = false;
		}
	}


	for (int j = 0; j < nChunkZ; j++)
	{
		for (int i = 0; i < nChunkX; i++)
		{
			ebo[j][i].Gen(Indices, sizeof(Indices));
		}
	}






	VAO1.Unbind();
	ShaderProgram.Activate();

	Texture grassTexture(ShaderProgram.ID, "terrain.png");
	Texture steveTexture(ShaderProgram.ID, "steve.png");

	int count = 0;

	for (int i = 0; i < nChunkZ; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{

			chunk[i][j].id = j + i * nChunkX;

			chunk[i][j].Create(j - offset, i - offset, vbo[i][j].ids[1], ebo[i][j].id, allVerticesZ);
			chunk[i][j].preRender(vbo[i][j].ids[1], allVerticesZ, ebo[i][j].id);

			isCreated[i - offset][j - offset] = true;

		}

	}

	glEnable(GL_DEPTH_TEST);

	POINT P;
	POINT P1;
	bool firstTime = true;
	float changeMouseX = 0;
	float changeMouseY = 0;
	bool yes = true;



	int kl = 0;
	int km = 0;

	glm::vec3 rayPoint;
	bool lookingBlock = false;

	float	currentChunk1 = 0;
	float	currentChunk2 = 0;
	float	chunkPosition1 = 0;
	float	chunkPosition2 = 0;

	bool run = true;

	ShaderProgram.Activate();

	grassTexture.Bind();

	VAO1.Bind();

	float pos0 = 0;
	float pos1 = 0;

	glLineWidth(2.5f);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	while (!glfwWindowShouldClose(window))
	{
		pos0 = position[0];
		pos1 = position[1];


		position[0] = (pos0 - offset * 16);
		position[1] = (pos1 - offset * 16) - (nChunkX * 16);

		currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
		currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
		chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
		chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
		chunkPosition[2] = position[2];


	/*	if (!isCreated[currentChunk[1] + (nChunkX - 1)][currentChunk[0]])
		{

			int z = (currentChunk[1] + (nChunkX - 1)) % nChunkX;
			int x = (currentChunk[0]) % nChunkX;

			if (z < 0)
			{
				z = abs(z);
				z %= nChunkX;
				z = (nChunkX)-z;

			}

			if (x < 0)
			{
				x = abs(x);
				x %= nChunkX;
				x = (nChunkX)-x;

			}

			chunk[z][x] = baseChunk[0];
			genZ(nChunkX - 1, 0, currentChunk[0], currentChunk[1]);

			chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesZ, ebo[z][x].id);

			isCreated[currentChunk[1] + (nChunkX - 1)][currentChunk[0]] = true;
			isCreated[currentChunk[1] + (nChunkX - 1) - nChunkX][currentChunk[0]] = false;


			renderingZ = true;

			regChunkX = currentChunk[0];
			regChunkZ = currentChunk[1];

		}
	*/
		position[0] = pos0 - offset * 16;
		position[1] = pos1 - offset * 16;

		currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
		currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
		chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
		chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
		chunkPosition[2] = position[2];

	//	std::cout << currentChunk[0] << " " << currentChunk[1] << '\n';

		if (!isCreated[(currentChunk[1] + (nChunkX - 1))-offset][(currentChunk[0]-offset)])
		{
			if (isCreated[-1+currentChunk[1] - offset][(currentChunk[0] - offset)] && !renderingBZ && !renderingZ && !renderingBX && !renderingX)
			{
				std::cout << "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

				int z = ChunkVBOZ[0];
				int x = (currentChunk[0]) % nChunkX;

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genZ(nChunkX - 1, 0, currentChunk[0], currentChunk[1]);

				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesZ, ebo[z][x].id);

				isCreated[(currentChunk[1] - offset) + (nChunkX - 1)][-offset + currentChunk[0]] = true;
				isCreated[(currentChunk[1] - offset) + (nChunkX - 1) - nChunkX][-offset + currentChunk[0]] = false;


				renderingZ = true;

				regChunkX = currentChunk[0];
				regChunkZ = currentChunk[1];
			}

		}

		if (!isCreated[currentChunk[1] - offset][(currentChunk[0] - offset)])
		{
			if (isCreated[currentChunk[1] - offset][(currentChunk[0] - offset) + 1] && !renderingBX && !renderingX && !renderingBZ && !renderingZ)
			{
				std::cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
				int z = (currentChunk[1]) % nChunkX;
				int x = ChunkVBOX[nChunkX - 1];

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genBX(0, 0, currentChunk[0], currentChunk[1]);

				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesX, ebo[z][x].id);

				isCreated[currentChunk[1] - offset][(currentChunk[0] - offset)] = true;
				isCreated[currentChunk[1] - offset][(currentChunk[0] - offset) + nChunkX] = false;


				if (renderingBX == false)
				{
					regChunkBX1 = currentChunk[0];
					regChunkBZ1 = currentChunk[1];
				}

				renderingBX = true;
			}
			else if (isCreated[1 + currentChunk[1] - offset][(currentChunk[0] - offset)] && !renderingZ && !renderingBZ && !renderingBX && !renderingX)
			{
				std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
				int z = ChunkVBOZ[nChunkX - 1];
				int x = (currentChunk[0]) % nChunkX;

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genBZ((0), 0, currentChunk[0], currentChunk[1]);

				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesZ, ebo[z][x].id);

				isCreated[((currentChunk[1] - offset))][-offset + currentChunk[0]] = true;
				isCreated[((currentChunk[1] - offset) + nChunkX)][-offset + currentChunk[0]] = false;


				renderingBZ = true;

				regChunkBX = currentChunk[0];
				regChunkBZ = currentChunk[1];
			}
		}
		
			if (!isCreated[currentChunk[1]-offset][(currentChunk[0]-offset) + (nChunkX - 1)] && !renderingX && !renderingBX && !renderingBZ && !renderingZ)
			{
				std::cout << "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii";
				int z = (currentChunk[1]) % nChunkX;
				int x = ChunkVBOX[0];

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genX(0, nChunkX - 1, currentChunk[0], currentChunk[1]);

				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesX, ebo[z][x].id);

				isCreated[currentChunk[1]-offset][(currentChunk[0]-offset) + (nChunkX - 1)] = true;
				isCreated[currentChunk[1]-offset][(currentChunk[0]-offset) + (nChunkX - 1) - nChunkX] = false;


				if (renderingX == false)
				{
					regChunkX1 = currentChunk[0];
					regChunkZ1 = currentChunk[1];
				}

				renderingX = true;

			}

		

			if (renderingBX)
			{
				//std::cout << regChunkX1 << " " << regChunkZ1 << '\n';
				//std::cout << nX << '\n';

				int z = (regChunkBZ1 + (nBX)) % nChunkX;
				int x = ChunkVBOX[nChunkX-1];

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genBX(nBX, 0, regChunkBX1, regChunkBZ1);
				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesX, ebo[z][x].id);

				isCreated[(regChunkBZ1 - offset) + (nBX)][((regChunkBX1 - offset))] = true;
				isCreated[(regChunkBZ1 - offset) + (nBX)][((regChunkBX1 - offset)) + nChunkX] = false;
				nBX++;
				if (nBX == nChunkX)
				{
					nBX = 1;
					renderingBX = false;
					moveArray(ChunkVBOX, nChunkX, -1);


				}


				

			}
			

			if (renderingX)
			{
				//std::cout << regChunkX1 << " " << regChunkZ1 << '\n';
				//std::cout << nX << '\n';

			

				int z = (regChunkZ1 + (nX)) % nChunkX;
				int x = ChunkVBOX[0];

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

				chunk[z][x] = baseChunk[0];
				genX(nX, nChunkX - 1, regChunkX1, regChunkZ1);
				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesX, ebo[z][x].id);

				isCreated[(regChunkZ1-offset) + (nX)][((regChunkX1-offset) + (nChunkX - 1))] = true;
				isCreated[(regChunkZ1-offset) + (nX)][((regChunkX1-offset) + (nChunkX - 1)) - nChunkX] = false;
				nX++;
				if (nX == nChunkX)
				{
					nX = 1;
					renderingX = false;
					moveArray(ChunkVBOX, nChunkX, 1);
				}


				

			}
			
			

			if (renderingZ)
			{

				int x = (regChunkX + (nZ)) % nChunkX;
				int z = ChunkVBOZ[0];

				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}

			//	std::cout << z << " " << x << '\n';

				chunk[z][x] = baseChunk[0];
				genZ(nChunkX - 1, nZ, regChunkX, regChunkZ);
				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesZ, ebo[z][x].id);

				isCreated[(regChunkZ - offset) + (nChunkX - 1)][-offset + regChunkX + nZ] = true;
				isCreated[(regChunkZ - offset) + (nChunkX - 1) - nChunkX][-offset+regChunkX + nZ] = false;
			//	std::cout << nZ << '\n';
				nZ++;
				if (nZ == nChunkX)
				{
					nZ = 1;
					renderingZ = false;
					moveArray(ChunkVBOZ, nChunkX, 1);
				}

				framePassed = 0;

			}


			if (renderingBZ)
			{

				int x = (regChunkBX + (nBZ)) % nChunkX;
				int z = ChunkVBOZ[nChunkX-1];


				if (z < 0)
				{
					z = abs(z);
					z %= nChunkX;
					z = (nChunkX)-z;

				}

				if (x < 0)
				{
					x = abs(x);
					x %= nChunkX;
					x = (nChunkX)-x;

				}
				//std::cout << z << " " << x << '\n';

				chunk[z][x] = baseChunk[0];
				genBZ((0), nBZ, regChunkBX, regChunkBZ);
				chunk[z][x].preRender(vbo[z][x].ids[1], allVerticesZ, ebo[z][x].id);

				isCreated[(regChunkBZ - offset)][-offset + regChunkBX + nBZ] = true;
				isCreated[((regChunkBZ - offset)) + nChunkX][-offset+regChunkBX + nBZ] = false;
				//std::cout << nBZ << '\n';
				nBZ++;
				if (nBZ == nChunkX)
				{
					nBZ = 1;
					renderingBZ = false;
					moveArray(ChunkVBOZ, nChunkX, -1);
					
				}

				framePassed = 0;

			}

	

			position[0] = pos0;
			position[1] = pos1;

			rayPoint = glm::vec3(position[0], position[2], position[1]);
			int count = 0;

		//	std::cout << position[0] << " " << position[1]<<'\n';
			
				while (run && collisionEnabled)
				{
					rayPoint += (cam.forward * 0.05f);
					currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16);
					currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16);
					chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
					chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);
					currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16) % nChunkX;
					currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16) % nChunkZ;

					count++;
					
						if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
						{
							rayPoint -= (cam.forward * 0.05f);
							while (run)
							{


								rayPoint += (cam.forward * 0.005f);
								currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16);
								currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16);
								chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
								chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);
								currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16) % nChunkX;
								currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16) % nChunkZ;

								if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
								{
									rayPoint -= (cam.forward * 0.005f);
									while (run)
									{
										rayPoint += (cam.forward * 0.0005f);
										currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16);
										currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16);
										chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
										chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);
										currentChunk[0] = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16) % nChunkX;
										currentChunk[1] = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16) % nChunkZ;

										if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
										{
											selectedBlockPosition[0] = chunkPosition[0];
											selectedBlockPosition[1] = chunkPosition[1];
											selectedBlockPosition[2] = roundf(rayPoint.y);
											placeBlockPosition[0] = selectedBlockPosition[0];
											placeBlockPosition[1] = selectedBlockPosition[1];
											placeBlockPosition[2] = selectedBlockPosition[2];

											rayPoint -= cam.forward * 0.0005f;

											currentChunk1 = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16);
											currentChunk2 = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16);
											chunkPosition1 = roundf(rayPoint.x - currentChunk1 * 16);
											chunkPosition2 = roundf(rayPoint.z - currentChunk2 * 16);
											currentChunk1 = abs(((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16) % nChunkX;
											currentChunk2 = abs(((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16) % nChunkX;


											if (selectedBlockPosition[0] == chunkPosition1 && selectedBlockPosition[1] == chunkPosition2)
											{

												if (rayPoint.y - selectedBlockPosition[2] > 0)
												{
													placeBlockPosition[2] += 1;
												}

												else
												{
													placeBlockPosition[2] -= 1;
												}

												lookingBlock = true;
												run = false;
											}
											if (currentChunk[0] == currentChunk1)
											{
												if (selectedBlockPosition[1] == chunkPosition2 && selectedBlockPosition[2] == roundf(rayPoint.y))
												{

													if (chunkPosition1 - selectedBlockPosition[0] > 0)
													{
														placeBlockPosition[0] += 1;
													}

													else
													{
														placeBlockPosition[0] -= 1;
													}
													lookingBlock = true;
													run = false;
												}
											}
											else
											{

												if (currentChunk1 > currentChunk[0])
												{
													selectedBlockPosition[0] = 15;
													placeBlockPosition[0] = 0;
												}

												else
												{
													selectedBlockPosition[0] = 0;
													placeBlockPosition[0] = 15;
												}

												lookingBlock = true;
												run = false;
											}


											if (currentChunk[1] == currentChunk2)
											{
												if (selectedBlockPosition[0] == chunkPosition1 && selectedBlockPosition[2] == roundf(rayPoint.y))
												{


													if (chunkPosition2 - selectedBlockPosition[1] > 0)
													{
														placeBlockPosition[1] += 1;
													}

													else
													{
														placeBlockPosition[1] -= 1;
													}
													lookingBlock = true;
													run = false;
												}
											}
											else
											{
												if (currentChunk2 > currentChunk[1])
												{
													selectedBlockPosition[1] = 15;
													placeBlockPosition[1] = 0;
												}

												else
												{
													selectedBlockPosition[1] = 0;
													placeBlockPosition[1] = 15;
												}

												lookingBlock = true;
												run = false;
											}
											run = false;
										}



									}

								}

							}

						}
					
					count++;

					if (count > 275)
					{
						selectedBlockPosition[0] = 0;
						selectedBlockPosition[1] = 0;
						selectedBlockPosition[2] = 0;
						lookingBlock = false;
						run = false;
					}


				}
			

			run = true;

			if (GetAsyncKeyState('1') & 0x8000)
			{
				invBlock = 1;
				change = true;
			}

			if (GetAsyncKeyState('2') & 0x8000)
			{
				invBlock = 2;
				change = true;
			}

			if (GetAsyncKeyState('3') & 0x8000)
			{
				invBlock = 3;
				change = true;
			}

			if (GetAsyncKeyState('4') & 0x8000)
			{
				invBlock = 4;
				change = true;
			}


			if (GetAsyncKeyState(VK_LBUTTON) & 1 && lookingBlock)
			{
				
					chunk[currentChunk[1]][currentChunk[0]].Update(selectedBlockPosition[0], selectedBlockPosition[1], selectedBlockPosition[2], false, ebo[(currentChunk[1])][currentChunk[0]].id, vbo[(currentChunk[1])][currentChunk[0]].ids[1], allVerticesX,0);
					kl = 0;
				
					PlaySound(TEXT("stone.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}

			kl++;

			if (GetAsyncKeyState(VK_RBUTTON) & 1 && lookingBlock)
			{
				
					chunk[currentChunk2][currentChunk1].Update(placeBlockPosition[0], placeBlockPosition[1], placeBlockPosition[2], true, ebo[(currentChunk2)][currentChunk1].id, vbo[(currentChunk2)][currentChunk1].ids[1], allVerticesX,invBlock);
					PlaySound(TEXT("stone.wav"), NULL, SND_FILENAME | SND_ASYNC);

			}

			if (lookingBlock)
			{
				rayPoint += (cam.forward * 0.005f);
				posX = round(rayPoint.x) - offset * 16;
				posZ = round(rayPoint.z) - offset * 16;
				posY = round(rayPoint.y);
			}
			else
			{
				posX =2345235;
				posZ =245234;
				posY =-1000000;
			}
	

			float outlineVertices1[] =
			{
				posX + vx, posY + vy, posZ + vz,0,0,0,0,0,0,  // 0: +x,+y,+z
				posX - vx, posY + vy, posZ + vz,0,0,0,0,0,0,  // 1: -x,+y,+z
				posX - vx, posY - vy, posZ + vz,0,0,0,0,0,0,  // 2: -x,-y,+z
				posX + vx, posY - vy, posZ + vz,0,0,0,0,0,0,  // 3: +x,-y,+z
				posX + vx, posY + vy, posZ - vz,0,0,0,0,0,0,  // 4: +x,+y,-z
				posX - vx, posY + vy, posZ - vz,0,0,0,0,0,0,  // 5: -x,+y,-z
				posX - vx, posY - vy, posZ - vz,0,0,0,0,0,0,  // 6: -x,-y,-z
				posX + vx, posY - vy, posZ - vz,0,0,0,0,0,0   // 7: +x,-y,-z
			};

			for (int i = 0; i < 72; i ++)
			{

				outlineVertices[i] = outlineVertices1[i];

			
			}

			vboOutline.Bind(vboOutline.ids[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(outlineVertices), outlineVertices, GL_STATIC_DRAW);
			vboOutline.Unbind();

			float changeMouseY1 = changeMouseY;

			if (yes)
			{


				GetCursorPos(&P);

				if (firstTime)
				{
					P1 = P;
					firstTime = false;
				}

				changeMouseX = (float)(P.x - 960) / 1920;
				changeMouseY = (float)(P.y - 540) / 1080;


				SetCursorPos(960, 540);

			}


			GLuint aR = glGetUniformLocation(ShaderProgram.ID, "R");
			glUniformMatrix4fv(aR, 1, GL_FALSE, glm::value_ptr(cam.R));

			GLuint aView = glGetUniformLocation(ShaderProgram.ID, "view");
			glUniformMatrix4fv(aView, 1, GL_FALSE, glm::value_ptr(cam.view));

			GLuint aProj = glGetUniformLocation(ShaderProgram.ID, "proj");
			glUniformMatrix4fv(aProj, 1, GL_FALSE, glm::value_ptr(cam.pro));

			if (GetAsyncKeyState('N') & 0x8000)
			{
				yes = false;
			}

			if (GetAsyncKeyState('M') & 0x8000)
			{
				cam.rotate(0, 1, 0, 0.2f);
			}

			float xRotation1 = xRotation;
		

			cam.rotate(0, 1, 0, changeMouseX * 70);
			yRotation += changeMouseX * 70;
			xRotation += changeMouseY * 70;

			if (yRotation > 360)
			{
				yRotation -= 360;
			}

			if (yRotation < 0)
			{
				yRotation += 360;
			}

			

			if (xRotation > 88)
			{
				xRotation = xRotation1;
			
				
			}

			else if (xRotation < -88)
			{
				xRotation = xRotation1;
			
				
			}

			else
			{
				cam.rotate(cam.right.x, cam.right.y, cam.right.z, -changeMouseY * 70);
			}
			

			{



				if (GetAsyncKeyState('S') & 0x8000)
				{
					cam.forward1.x = -cam.forward.x;
					cam.forward1.z = -cam.forward.z;
					yes = true;


				}

				if (!(GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState('S') & 0x8000))
				{
					cam.forward1.x = 0;
					cam.forward1.z = 0;
					cam.forward1.y = 0;
				}

				if ((GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState('S') & 0x8000))
				{
					cam.forward1.x = cam.forward.x;
					cam.forward1.z = cam.forward.z;

				}

				if ((GetAsyncKeyState('W') & 0x8000) && (GetAsyncKeyState(VK_LCONTROL) & 0x8000))
				{
					cam.forward1.x = cam.forward.x + cam.forward.x / 1.2f;
					cam.forward1.z = cam.forward.z + cam.forward.z / 1.2f;
				}

				if (GetAsyncKeyState('D') & 0x8000)
				{
					cam.forward1.x -= cam.right.x;
					cam.forward1.y -= cam.right.y;
					cam.forward1.z -= cam.right.z;

				}

				if (GetAsyncKeyState('A') & 0x8000)
				{
					cam.forward1.x += cam.right.x;
					cam.forward1.y += cam.right.y;
					cam.forward1.z += cam.right.z;

				}

				if (onGround())
				{
					fallingSpeed = 0;
					std::cout << onGround();
				}

				if ((GetAsyncKeyState(VK_SPACE) & 1))
				{
					
					if (onGround())
					{
						jumpT = clock();
						isJumping = true;
						fallingSpeed = 0;
					}

					if (click == 1 && clock() - spaceBarT > 300)
					{
						click = 0;
					}

					if (click == 0)
					{
						spaceBarT = clock();
						click = 1;
					}

					else
					{
						spaceBarT = clock() - spaceBarT;
						click = 0;
					}

					if (spaceBarT < 170 && spaceBarT > 100)
					{

						if (!creativeMode)
						{
							creativeMode = true;
						}

						else
						{
							creativeMode = false;
						}
					}

				}

				if (isJumping)
				{
					cam.forward1.y = 4.0f - sqrt((float)(clock() - jumpT) / 15);
					if (3.25f - sqrt((float)(clock() - jumpT) / 15) < 0)
					{
						isJumping = false;
					}
				}

				if (GetAsyncKeyState(VK_SPACE) & 0x8000 && creativeMode)
				{
					cam.forward1.y = 1.0f;
				}

				if (!creativeMode && !isJumping && !onGround())
				{
					fallingSpeed -= 0.02f * (clock() - before);
					cam.forward1.y = fallingSpeed;
				}

				else
				{
					if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && !isJumping)
					{
						cam.forward1.y = -1.0f;
					}

					else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000) && !isJumping)
					{
						cam.forward1.y = 0;
					}
				}




			}


			if (clock() - before > 1)
			{
				Move(1, 0.52f * (clock() - before));
				Move(12, 0.52f * (clock() - before));
				Move(13, 0.52f * (clock() - before));

				before = clock();


			}




			glClearColor(0.1, 0.21, 0.31, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			eboCursor.Bind();
			vboCursor.Bind(vboCursor.ids[0]);
			VAO1.LinkVBO(vboCursor, 3, 0, vboCursor.ids[0], 9, 0);
			VAO1.LinkVBO(vboCursor, 3, 1, vboCursor.ids[0], 9, 3);
			VAO1.LinkVBO(vboCursor, 2, 2, vboCursor.ids[0], 9, 6);
			VAO1.LinkVBO(vboCursor, 1, 3, vboCursor.ids[0], 9, 8);
			glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
			eboCursor.Unbind();
			vboCursor.Unbind();

			glEnable(GL_BLEND);

			if (change)
			{
				for (int i = 0; i < 216; i += 9)
				{
					handBlock[i + 6] = uv.uvCords[(((i) / 9) * 2) + (48 * invBlock)];
					handBlock[i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (48 * invBlock)];
				}

				vboHandBlock.Bind(vboHandBlock.ids[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(handBlock), handBlock, GL_STATIC_DRAW);
				change = false;

			}

			glDepthFunc(GL_LESS);

			for (int j = 0; j < rChunkZ; j++)
			{
				for (int i = 0; i < rChunkX; i++)
				{
					isVisible[j][i] = false;

				}
			}

			currentChunk[0] = abs((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
			currentChunk[1] = abs((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;

			

			

			for (int j = 0; j < rChunkZ; j++)
			{
				for (int i = 0; i < rChunkX; i++)
				{
					if (chunk[j][i].ready)
					{
						
							chunk[j][i].Render(VAO1, vbo[j][i], ebo[j][i]);
						
						
					}

				}
			}


			glDisable(GL_BLEND);

			eboOutline.Bind();
			vboOutline.Bind(vboOutline.ids[1]);
			VAO1.LinkVBO(vboOutline, 3, 0, vboOutline.ids[1], 9, 0);
			VAO1.LinkVBO(vboOutline, 3, 1, vboOutline.ids[1], 9, 3);
			VAO1.LinkVBO(vboOutline, 2, 2, vboOutline.ids[1], 9, 6);
			VAO1.LinkVBO(vboOutline, 1, 3, vboOutline.ids[1], 9, 8);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
			eboOutline.Unbind();
			vboOutline.Unbind();

			glClear(GL_DEPTH_BUFFER_BIT);

			glDepthFunc(GL_LESS);

			eboHandBlock.Bind();
			vboHandBlock.Bind(vboHandBlock.ids[0]);
			VAO1.LinkVBO(vboHandBlock, 3, 0, vboHandBlock.ids[0], 9, 0);
			VAO1.LinkVBO(vboHandBlock, 3, 1, vboHandBlock.ids[0], 9, 3);
			VAO1.LinkVBO(vboHandBlock, 2, 2, vboHandBlock.ids[0], 9, 6);
			VAO1.LinkVBO(vboHandBlock, 1, 3, vboHandBlock.ids[0], 9, 8);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			eboHandBlock.Unbind();
			vboHandBlock.Unbind();

			glDepthFunc(GL_LESS);




			if (GetAsyncKeyState('B') & 0x8000)
			{
				framePassed++;
			}

			if (GetAsyncKeyState('M') & 0x8000)
			{
				std::cout << framePassed << '\n';
			}

			glfwSwapBuffers(window);
			glfwPollEvents();


		}

		glDeleteTextures(1, &grassTexture.id);
		glDeleteTextures(1, &steveTexture.id);
		VAO1.Delete();
		ShaderProgram.Delete();

		glfwTerminate();
	}
