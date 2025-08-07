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

float larghezza = 1920;
float aSensZoom = 0.1;
float position[3] = { 30,30,69 };
int currentChunk[2] = { 0,0 };
float chunkPosition[3] = { 0,0,0 };
float prevSelectedBlock[3] = { 0,0,0 };
float prevSelectedChunk[2] = { 0,0 };
float yRotation = 0;
float sensRotazione = 0;
float sensX = 0;
float sensY = 0;
int asse = 0;
int placeBlockPosition[3] = { 0,0,0 };
int selectedBlockPosition[3] = { 0,0,0 };
int nChunkX = 25;
int nChunkZ = 25;

float pos0 = position[0];
float pos1 = position[1];
float pos2 = position[2];
camera cam;

glm::mat4 view1;

std::vector<std::vector<VBO>> vbo(nChunkX, std::vector<VBO>(nChunkZ));
std::vector<std::vector<EBO>> ebo(nChunkX, std::vector<EBO>(nChunkZ));
std::vector<std::vector<Chunk>> chunk(nChunkX, std::vector<Chunk>(nChunkZ));

clock_t destroyT = clock();
clock_t before = clock();
clock_t before2 = clock();
clock_t spaceBarT = clock();
clock_t jumpT = clock();
bool isJumping = false;

bool creativeMode = false;
int click = 0;

bool onGround()
{
	currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
	currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
	chunkPosition[0] = position[0] - currentChunk[0] * 16 + 0.5f;
	chunkPosition[1] = position[1] - currentChunk[1] * 16 + 0.5f;
	chunkPosition[2] = position[2];

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

	if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 1.5f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 1.5f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}

	else if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 1.5f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 1.5f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}
}

bool checkBlock()
{
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

	if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] + 0.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] + 0.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}

	else if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] + 0.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] + 0.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}


	if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}

	else if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 1.25f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}



	if (chunk[cZ][cX].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}

	else if (chunk[cZ][cX1].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset) * 16))])
	{
		return true;
	}

	else if (chunk[cZ1][cX1].posIsBlock[(int)((roundf(position[2] - 0.75f) * 16 * 16) + (int)roundf(xOffset1) + ((int)roundf(zOffset1) * 16))])
	{
		return true;
	}



	return false;
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



	float altezza = 1080;

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


	GLuint Indices[]
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7
	};


	Shader ShaderProgram("default.vert", "default.frag");


	VAO VAO1;
	VAO1.Bind();

	VBO vboCursor;
	vboCursor.Gen(TriangleVertices1, sizeof(TriangleVertices1));
	EBO eboCursor;
	eboCursor.Gen(Indices, sizeof(Indices));



	for (int j = 0; j < 20; j++)
	{
		for (int i = 0; i < 20; i++)
		{
			vbo[j][i].Gen(TriangleVertices1, sizeof(TriangleVertices1));

		}
	}


	for (int j = 0; j < 20; j++)
	{
		for (int i = 0; i < 20; i++)
		{
			ebo[j][i].Gen(Indices, sizeof(Indices));
		}
	}

	VAO1.Unbind();
	ShaderProgram.Activate();

	Texture grassTexture(ShaderProgram.ID, "terrain.png");
	Texture steveTexture(ShaderProgram.ID, "steve.png");


	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			chunk[i][j].Create(j, i, vbo[i][j].ids[1], ebo[i][j].id);
		}

	}

	glEnable(GL_DEPTH_TEST);

	POINT P;
	POINT P1;
	bool firstTime = true;
	float changeMouseX;
	float changeMouseY;
	bool yes = true;

	cam.proj(60.0f, 0.001f, larghezza, altezza, 100000.0f);

	int kl = 0;
	int km = 0;

	glm::vec3 rayPoint;
	bool lookingBlock;

	float	currentChunk1;
	float	currentChunk2;
	float	chunkPosition1;
	float	chunkPosition2;

	bool run = true;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	while (!glfwWindowShouldClose(window))
	{

		rayPoint = glm::vec3(position[0], position[2], position[1]);
		int count = 0;

		while (run)
		{
			rayPoint += (cam.forward * 0.5f);
			currentChunk[0] = ((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16;
			currentChunk[1] = ((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16;
			chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
			chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);
			count++;
			if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
			{
				rayPoint -= (cam.forward * 0.5f);
				while (run)
				{
					
					
					rayPoint += (cam.forward * 0.05f);
					currentChunk[0] = ((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16;
					currentChunk[1] = ((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16;
					chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
					chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);

					if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
					{
						rayPoint -= (cam.forward * 0.05f);
						while (run)
						{
							rayPoint += (cam.forward * 0.005f);
							currentChunk[0] = ((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16;
							currentChunk[1] = ((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16;
							chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * 16);
							chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * 16);
							if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(rayPoint.y) * 16 * 16) + (chunkPosition[0]) + (chunkPosition[1] * 16))])
							{
								selectedBlockPosition[0] = chunkPosition[0];
								selectedBlockPosition[1] = chunkPosition[1];
								selectedBlockPosition[2] = roundf(rayPoint.y);
								placeBlockPosition[0] = selectedBlockPosition[0];
								placeBlockPosition[1] = selectedBlockPosition[1];
								placeBlockPosition[2] = selectedBlockPosition[2];

								rayPoint -= cam.forward * 0.005f;

								currentChunk1 = ((int)floorf(rayPoint.x + 0.5f) - ((int)floorf(rayPoint.x + 0.5f) % 16)) / 16;
								currentChunk2 = ((int)floorf(rayPoint.z + 0.5f) - ((int)floorf(rayPoint.z + 0.5f) % 16)) / 16;
								chunkPosition1 = roundf(rayPoint.x - currentChunk1 * 16);
								chunkPosition2 = roundf(rayPoint.z - currentChunk2 * 16);

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
				
			if (count > 45)
			{
				selectedBlockPosition[0] = 0;
				selectedBlockPosition[1] = 0;
				selectedBlockPosition[2] = 0;
				lookingBlock = false;
				run = false;
			}


		}


		run = true;

		if (prevSelectedChunk[0] != currentChunk[0] || prevSelectedChunk[1] != currentChunk[1])
		{
			chunk[prevSelectedChunk[1]][prevSelectedChunk[0]].highLight(0, 0, 0, vbo[prevSelectedChunk[1]][prevSelectedChunk[0]].ids[1]);
		}


		if (km > 15)
		{
			chunk[currentChunk[1]][currentChunk[0]].highLight(selectedBlockPosition[0], selectedBlockPosition[1], selectedBlockPosition[2], vbo[(currentChunk[1])][currentChunk[0]].ids[1]);
			km = 0;
		}

		prevSelectedChunk[0] = currentChunk[0];
		prevSelectedChunk[1] = currentChunk[1];



		km++;

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && kl > 50 && lookingBlock)
		{
			chunk[currentChunk[1]][currentChunk[0]].Update(selectedBlockPosition[0], selectedBlockPosition[1], selectedBlockPosition[2], false, ebo[(currentChunk[1])][currentChunk[0]].id, vbo[(currentChunk[1])][currentChunk[0]].ids[1]);
			kl = 0;
		}

		kl++;

		if (GetAsyncKeyState(VK_RBUTTON) & 1 && lookingBlock)
		{
			chunk[currentChunk2][currentChunk1].Update(placeBlockPosition[0], placeBlockPosition[1], placeBlockPosition[2], true, ebo[(currentChunk2)][currentChunk1].id, vbo[(currentChunk2)][currentChunk1].ids[1]);
		}



		if (yes)
		{


			GetCursorPos(&P);

			if (firstTime)
			{
				P1 = P;
				firstTime = false;
			}

			changeMouseX = (float)(P1.x - P.x) / 1920;
			changeMouseY = (float)(P1.y - P.y) / 1080;
			P1 = P;
			P1.x += P1.x - 960;
			P1.y += P1.y - 540;



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
			cam.rotate(0, 1, 0, -0.2f);
			yes = false;
		}

		if (GetAsyncKeyState('M') & 0x8000)
		{
			cam.rotate(0, 1, 0, 0.2f);
		}


		cam.rotate(0, 1, 0, changeMouseX * 70);
		yRotation += changeMouseX * 70;

		if (yRotation > 360)
		{
			yRotation -= 360;
		}

		cam.rotate(cam.right.x, cam.right.y, cam.right.z, -changeMouseY * 70);

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


		
		
			if ((GetAsyncKeyState(VK_SPACE) & 1))
			{
				if (onGround())
				{
					jumpT = clock();
					isJumping = true;
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

				if (spaceBarT < 250 && spaceBarT > 70)
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
				cam.forward1.y = 5.0f - sqrt((float)(clock()-jumpT)/10);
				if (4.0f - sqrt((float)(clock() - jumpT)/10) < 0)
				{
					isJumping = false;
				}
			}

			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && creativeMode)
			{
				cam.forward1.y = 1.0f;
			}

		 if(!creativeMode && !isJumping)
		 {
			 cam.forward1.y = -2.0f;
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


		if (clock() - before > 4)
		{
			Move(1,2.8f);
			Move(12,2.8f);
			Move(13,2.8f);
			before = clock();
		}
	

		glClearColor(0.1, 0.21, 0.31, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderProgram.Activate();

		grassTexture.Bind();

		VAO1.Bind();

		eboCursor.Bind();
		vboCursor.Bind(vboCursor.ids[0]);
		VAO1.LinkVBO(vboCursor, 3, 0, vboCursor.ids[0], 9, 0);
		VAO1.LinkVBO(vboCursor, 3, 1, vboCursor.ids[0], 9, 3);
		VAO1.LinkVBO(vboCursor, 2, 2, vboCursor.ids[0], 9, 6);
		VAO1.LinkVBO(vboCursor, 1, 3, vboCursor.ids[0], 9, 8);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		eboCursor.Unbind();
		vboCursor.Unbind();

		for (int j = 0; j < 10; j++)
		{
			for (int i = 0; i < 10; i++)
			{
				if (chunk[j][i].created)
				{
					chunk[j][i].Render(VAO1, vbo[j][i], ebo[j][i]);
				}
				
			}
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
