#include "chunkGen.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "Blocco.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Texture.h"
#include <iostream>

void Chunk::Create(int x1, int y1, GLuint vbo, GLuint ebo)
{
	x = x1;
	z = y1;


	{
		squareVertices.resize(216 * sizeZ * sizeX * sizeY);
		std::vector<Blocco> blocco0(sizeX*sizeY*sizeZ);
		indices0.resize(800000*10);


		for (int i = 0; i < (sizeX * sizeY * sizeZ); i++)
		{
			posIsBlock[i] = false;
		}

		for (int l = 0; l < sizeY; l++)
		{
			for (int i = 0; i < sizeX; i++)
			{
				for (int j = 0; j < sizeX; j++)
				{
					for (int k = 0; k < 216; k += 9)
					{
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l* 216 * sizeX*sizeZ)] = TriangleVertices[k] + (sizeX * x) + j;
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 1] = TriangleVertices[k + 1] + l;
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 2] = TriangleVertices[k + 2] + (sizeX * z) + i;
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 3] = TriangleVertices[k + 3];
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 4] = TriangleVertices[k + 4];
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 5] = TriangleVertices[k + 5];
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 6] = TriangleVertices[k + 6];
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 7] = TriangleVertices[k + 7];
						squareVertices[k + (j * 216) + (i * 216 * 16) + (l * 216 * sizeX * sizeZ) + 8] = 45987;
					}
				}
			}

		}




		int count = 0;
		for (int j = 0; j < sizeX*(sizeY-60)*sizeZ; j++)
		{


			blocco0[j] = { count };
			blocco0[j].chunkReference = count;
			posIsBlock[j] = true;
			count++;
		}

		blocco.resize(sizeX * sizeY * sizeZ);
		blocco = blocco0;

		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{

					if (posIsBlock[k + (j * sizeX) + (i * sizeX * sizeZ)])
					{
						if (k > 0 && k < sizeZ - 1)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = false;
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = false;
						}
						else if (k == 0)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = false;
						}

						else
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = false;
						}



						if (j > 0 && j < sizeX - 1)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = false;
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = false;
						}
						else if (j == 0)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = false;
						}

						else
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = false;
						}



						if (i > 0 && i < sizeY - 1)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = false;
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = false;
						}
						else if (i == 0)
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = false;
						}
						else
						{
							blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = false;
						}




					}

					else
					{
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderFront = false;
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBack = false;
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderRight = false;
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderLeft = false;
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderTop = false;
						blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBottom = false;

					}

				}
			}
		}


		int count1 = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderFront)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24));
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 3);
						count1++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBack)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 4);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 7);
						count1++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderRight)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 8);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 11);
						count1++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderLeft)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 12);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 15);
						count1++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderTop)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 16);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 19);
						count1++;
					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBottom)
					{

						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 20);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count1++;
						indices0[count1] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 23);
						count1++;
					}
				}
			}
		}
		indices0.resize(count1);

		int size1 = indices0.size();


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 216 * 16 * 16 * 128 *sizeof(float), squareVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size1 * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexNumbers = indices0.size();
	}

	created = true;

}


void Chunk::Update(int x, int z, int y, bool build, GLuint ebo, GLuint vbo)
{
	int chunkReference = (x)+(z * sizeX) + (y * sizeX * sizeZ);
	int l = 0;
	int k = x; int j = z; int i = y;
	if (build && !posIsBlock[chunkReference])
	{
		posIsBlock[chunkReference] = true;
		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (k * 216) + (j * 216 * 16) + (i * 216 * sizeX * sizeZ) + 6] = TriangleVertices[h + 6];
			squareVertices[h + (k * 216) + (j * 216 * 16) + (i * 216 * sizeX * sizeZ) + 7] = TriangleVertices[h + 7];
		}
		
		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = false;
				l -= 6;
			}
			
			if (posIsBlock[chunkReference - 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = false;
				l -= 6;
			}
	/*
			for (int h = 144; h < 162; h += 9)
			{
				squareVertices[h + ((k+1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[h + ((k+1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[h + ((k+1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;
			}

			for (int h = 162; h < 180; h += 9)
			{
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;
			}
		*/

		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = false;
				l -= 6;
			}

			/*
			for (int h = 144; h < 162; h += 9)
			{
				squareVertices[h + ((k + 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[h + ((k + 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[h + ((k + 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;
			}
			*/
		}

		else
		{
			if (posIsBlock[chunkReference - 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = false;
				l -= 6;
			}

			/*for (int h = 162; h < 180; h += 9)
			{
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[h + ((k - 1) * 216) + (j * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;
			}
			*/
		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = false;
				l -= 6;
			}

			if (posIsBlock[chunkReference - sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = false;
				l -= 6;
			}		
	/*
				squareVertices[144 + 9+(k  * 216) + ((j+1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[144 + 9+(k * 216) + ((j+1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[144 + 9+(k * 216) + ((j+1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;	

				squareVertices[144 + 27+(k * 216) + ((j + 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[144 + 27+(k * 216) + ((j + 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[144 + 27+(k * 216) + ((j + 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;



			
				squareVertices[144+(k * 216) + ((j-1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[144+(k * 216) + ((j-1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[144+(k * 216) + ((j-1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;

				squareVertices[144+18+(k * 216) + ((j - 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 3] -= 0.23;
				squareVertices[144+18+(k * 216) + ((j - 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 4] -= 0.23f;
				squareVertices[144+18+(k * 216) + ((j - 1) * 216 * 16) + ((i - 1) * 216 * sizeX * sizeZ) + 5] -= 0.23f;
			*/
			
		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = false;
				l -= 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = false;
				l -= 6;
			}
		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = false;
				l -= 6;
			}

			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = false;
				l -= 6;
			}
					
		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = false;
				l -= 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = false;
				l -= 6;
			}
		}

		blocco[chunkReference].RenderFront = true;
		blocco[chunkReference].RenderBack = true;
		blocco[chunkReference].RenderRight = true;
		blocco[chunkReference].RenderLeft = true;
		blocco[chunkReference].RenderTop = true;
		blocco[chunkReference].RenderBottom = true;
		l += 36;

		indices0.resize(indices0.size() + l+1);

		l = 0;

		int count = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderFront)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24));
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 3);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBack)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 4);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 7);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderRight)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 8);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 11);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderLeft)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 12);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 15);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderTop)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 16);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 19);
						count++;
					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBottom)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 20);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 23);
						count++;

					}
				}
			}
		}

	

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 216 * 16 * 16 * 128 * sizeof(float), squareVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

	else if (!build)
	{
		l = 0;
		posIsBlock[chunkReference] = false;
		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
			if (posIsBlock[chunkReference - 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}
			
		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - 1])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}
		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}
			
			if (posIsBlock[chunkReference - sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}
	
		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - sizeX])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}
		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}
			
			if (posIsBlock[chunkReference - (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = true;
				l += 6;
			}
			
		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference - (sizeX * sizeZ)])
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = true;
				l += 6;
			}
		}

		if (blocco[chunkReference].RenderFront)
		{
			blocco[chunkReference].RenderFront = false;
			l += 6;
		}
		
		if (blocco[chunkReference].RenderBack)
		{
			blocco[chunkReference].RenderBack = false;
			l += 6;
		}

		if (blocco[chunkReference].RenderRight)
		{
			blocco[chunkReference].RenderRight = false;
			l += 6;
		}

		if (blocco[chunkReference].RenderLeft)
		{
			blocco[chunkReference].RenderLeft = false;
			l += 6;
		}

		if (blocco[chunkReference].RenderTop)
		{
			blocco[chunkReference].RenderTop = false;
			l += 6;
		}

		if (blocco[chunkReference].RenderBottom)
		{
			blocco[chunkReference].RenderBottom = false;
			l += 6;
		}

		indices0.resize(indices0.size() +l);

		l = 0;

		std::fill(indices0.begin(), indices0.end(), 0);

		int count = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderFront)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24));
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 1);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 2);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 3);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBack)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 4);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 5);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 6);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 7);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderRight)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 8);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 9);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 10);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 11);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderLeft)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 12);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 13);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 14);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 15);
						count++;

					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderTop)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 16);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 17);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 18);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 19);
						count++;
					}

					if (blocco[k + (j * sizeX) + (i * sizeX * sizeZ)].RenderBottom)
					{

						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 20);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 21);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 22);
						count++;
						indices0[count] = ((k * 24) + (j * sizeX * 24) + (i * sizeX * sizeZ * 24) + 23);
						count++;

					}
				}
			}
		}
		
		
	}

	int size1 = indices0.size();



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size1 * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	indexNumbers = indices0.size();
	}

void Chunk::highLight(int x, int z, int y, GLuint vbo)	
{	

	if (squareVertices[(x * 216) + (z * 216 * 16) + (y * 216 * sizeX * sizeZ) + 8] != 0)
	{

		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (highLightedBlock[0] * 216) + (highLightedBlock[1] * 216 * 16) + (highLightedBlock[2] * 216 * sizeX * sizeZ) + 8] = 289457;
		}


		int k = x; int j = z; int i = y;

		int count = 0;

		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (k * 216) + (j * 216 * 16) + (i * 216 * sizeX * sizeZ) + 8] = count;
			count++;
			if (count == 4)
			{
				count = 0;
			}


		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 216 * 16 * 16 * 128 * sizeof(float), squareVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		highLightedBlock[0] = x;
		highLightedBlock[1] = z;
		highLightedBlock[2] = y;
		highLightedBlock[3] = 1;


	}

	else if (highLightedBlock[3] == 1)
	{
		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (highLightedBlock[0] * 216) + (highLightedBlock[1] * 216 * 16) + (highLightedBlock[2] * 216 * sizeX * sizeZ) + 8] = 289457;
		}

		highLightedBlock[3] = 0;

	}


}


void Chunk::Render(VAO vao, VBO vbo, EBO ebo)
{
	ebo.Bind();
	vbo.Bind(vbo.ids[1]);
	vao.LinkVBO(vbo, 3, 0, vbo.ids[1], 9, 0);
	vao.LinkVBO(vbo, 3, 1, vbo.ids[1], 9, 3);
	vao.LinkVBO(vbo, 2, 2, vbo.ids[1], 9, 6);
	vao.LinkVBO(vbo, 1, 3, vbo.ids[1], 9, 8);
	glDrawElements(GL_TRIANGLES, indexNumbers, GL_UNSIGNED_INT, 0);
	ebo.Unbind();
	vbo.Unbind();
}



