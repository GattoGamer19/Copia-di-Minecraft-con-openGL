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
#include "UVcord.h"
#include <math.h>

typedef struct {
	float x, y;
} vector2;

vector2 randomGradient(int ix, int iy) {
	// No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= 3284157443;

	b ^= a << s | a >> w - s;
	b *= 1911520717;

	a ^= b << s | b >> w - s;
	a *= 2048419325;
	float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

	// Create the vector from the angle
	vector2 v;
	v.x = sin(random);
	v.y = cos(random);

	return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
	// Get gradient from integer coordinates
	vector2 gradient = randomGradient(ix, iy);

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx * gradient.x + dy * gradient.y);
}

float interpolate(float a0, float a1, float w)
{
	return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}


// Sample Perlin noise at coordinates x, y
float perlin(float x, float y) {

	// Determine grid cell corner coordinates
	int x0 = (int)x;
	int y0 = (int)y;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	// Compute Interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Compute and interpolate top two corners
	float n0 = dotGridGradient(x0, y0, x, y);
	float n1 = dotGridGradient(x1, y0, x, y);
	float ix0 = interpolate(n0, n1, sx);

	// Compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	float ix1 = interpolate(n0, n1, sx);

	// Final step: interpolate between the two previously interpolated values, now in y
	float value = interpolate(ix0, ix1, sy);

	return value;
}

void Chunk::addShadowTop(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	//convertire j e i in x e z prendendoli dai requisiti di funzione, per domani al me del futuro

	int j = z;
	int i = x;

	if (count <= sizeX * sizeZ * sizeY)
	{
			if (j > 0)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (sizeX)].id > 0)
				{
					allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
					allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
					allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;

					allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
					allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
					allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;
				}
			}


			if (j < sizeX - 1)
			{
				if (blocco[(count + (sizeX * sizeZ)) + (sizeX)].id > 0)
				{
					allVertices[allOff + 3] = TriangleVertices[(36 * 4) + 3] - contrasto / shadowValue;
					allVertices[allOff + 4] = TriangleVertices[(36 * 4) + 4] - contrasto / shadowValue;
					allVertices[allOff + 5] = TriangleVertices[(36 * 4) + 5] - contrasto / shadowValue;

					allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 4) + 18 + 3] - contrasto / shadowValue;
					allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 4) + 18 + 4] - contrasto / shadowValue;
					allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 4) + 18 + 5] - contrasto / shadowValue;
				}
			}

			
			if (i > 0)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (1)].id > 0)
				{
					allVertices[allOff + 3] = TriangleVertices[(36 * 4) + 3] - contrasto / shadowValue;
					allVertices[allOff + 4] = TriangleVertices[(36 * 4) + 4] - contrasto / shadowValue;
					allVertices[allOff + 5] = TriangleVertices[(36 * 4) + 5] - contrasto / shadowValue;

					allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
					allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
					allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;
				}
			}

			if (i < sizeX - 1)
			{
				if (blocco[(count + (sizeX * sizeZ)) + (1)].id > 0)
				{
					allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
					allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
					allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;

					allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 4) + 18 + 3] - contrasto / shadowValue;
					allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 4) + 18 + 4] - contrasto / shadowValue;
					allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 4) + 18 + 5] - contrasto / shadowValue;
				}
			}

			if (i > 0 && j < sizeX - 1)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (1 - sizeX)].id > 0)
				{
					allVertices[allOff + 3] = TriangleVertices[(36 * 4) + 3] - contrasto / shadowValue;
					allVertices[allOff + 4] = TriangleVertices[(36 * 4) + 4] - contrasto / shadowValue;
					allVertices[allOff + 5] = TriangleVertices[(36 * 4) + 5] - contrasto / shadowValue;
				}
			}

			if (i > 0 && j > 0)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (1 + sizeX)].id > 0)
				{
					allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
					allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
					allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;
				}
			}

			if (i < sizeX - 1 && j < sizeX - 1)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (-1 - sizeX)].id > 0)
				{
					allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 4) + 18 + 3] - contrasto / shadowValue;
					allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 4) + 18 + 4] - contrasto / shadowValue;
					allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 4) + 18 + 5] - contrasto / shadowValue;
				}
			}

			if (i < sizeX - 1 && j > 0)
			{
				if (blocco[(count + (sizeX * sizeZ)) - (-1 + sizeX)].id > 0)
				{
					allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
					allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
					allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;
				}
			}
		}

	
}

void Chunk::addShadowBottom(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	int j = z;
	int i = x;


	if (count >= sizeX * sizeZ && count < sizeX*sizeZ*sizeY && j < sizeX - 1)
	{

	
			if (blocco[(count - (sizeX * sizeZ)) + (sizeX)].id > 0)
			{
				allVertices[allOff + 0 + 3] = TriangleVertices[(36 * 5) + 0 + 3] - contrasto / shadowValue;
				allVertices[allOff + 0 + 4] = TriangleVertices[(36 * 5) + 0 + 4] - contrasto / shadowValue;
				allVertices[allOff + 0 + 5] = TriangleVertices[(36 * 5) + 0 + 5] - contrasto / shadowValue;

				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 5) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 5) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 5) + 18 + 5] - contrasto / shadowValue;
			}
		


	
			if (blocco[(count + (sizeX * sizeZ)) + (sizeX)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 5) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 5) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 5) + 9 + 5] - contrasto / shadowValue;

				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;
			}
		


		if (i > 0)
		{
			if (blocco[(count + (sizeX)) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 5) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 5) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 5) + 5] - contrasto / shadowValue;

				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 5) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 5) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 5) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[(count + (sizeX)) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;

				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 5) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 5) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 5) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[(count - (sizeX * sizeZ)) + sizeX + (1)].id > 0)
			{
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 5) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 5) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 5) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[(count + (sizeX * sizeZ)) + sizeX + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[(count + (sizeX * sizeZ)) + sizeX - (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 5) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 5) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 5) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[(count - (sizeX * sizeZ)) + sizeX - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 5) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 5) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 5) + 5] - contrasto / shadowValue;
			}
		}
	}
}

void Chunk::addShadowFront(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	int j = z;
	int i = x;


	if (count >= sizeX * sizeZ && count < sizeX * sizeZ * sizeY && j < sizeX - 1)
	{


		if (blocco[(count - (sizeX * sizeZ)) + (sizeX)].id > 0)
		{
			allVertices[allOff + 0 + 3] = TriangleVertices[(36 * 0) + 0 + 3] - contrasto / shadowValue;
			allVertices[allOff + 0 + 4] = TriangleVertices[(36 * 0) + 0 + 4] - contrasto / shadowValue;
			allVertices[allOff + 0 + 5] = TriangleVertices[(36 * 0) + 0 + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 0) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 0) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 0) + 18 + 5] - contrasto / shadowValue;
		}




		if (blocco[(count + (sizeX * sizeZ)) + (sizeX)].id > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 0) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 0) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 0) + 9 + 5] - contrasto / shadowValue;

			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 0) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 0) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 0) + 27 + 5] - contrasto / shadowValue;
		}



		if (i > 0)
		{
			if (blocco[(count + (sizeX)) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 0) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 0) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 0) + 5] - contrasto / shadowValue;

				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 0) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 0) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 0) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[(count + (sizeX)) + (1)].id > 0)
			{														  
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 0) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 0) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 0) + 27 + 5] - contrasto / shadowValue;
																	  
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 0) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 0) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 0) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[((count - (sizeX * sizeZ)) + sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 0) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 0) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 0) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[((count + (sizeX * sizeZ)) + sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 0) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 0) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 0) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[((count + (sizeX * sizeZ)) + sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 0) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 0) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 0) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[((count - (sizeX * sizeZ)) + sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 0) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 0) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 0) + 5] - contrasto / shadowValue;
			}
		}
	}
}

void Chunk::addShadowBack(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	int j = z;
	int i = x;


	if (count >= sizeX * sizeZ && count < sizeX * sizeZ * sizeY && j > 0)
	{


		if (blocco[(count - (sizeX * sizeZ)) - (sizeX)].id > 0)
		{
			allVertices[allOff + 0 + 3] = TriangleVertices[(36 * 1) + 0 + 3] - contrasto / shadowValue;
			allVertices[allOff + 0 + 4] = TriangleVertices[(36 * 1) + 0 + 4] - contrasto / shadowValue;
			allVertices[allOff + 0 + 5] = TriangleVertices[(36 * 1) + 0 + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 1) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 1) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 1) + 18 + 5] - contrasto / shadowValue;
		}




		if (blocco[(count + (sizeX * sizeZ)) - (sizeX)].id > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 1) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 1) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 1) + 9 + 5] - contrasto / shadowValue;

			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 1) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 1) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 1) + 27 + 5] - contrasto / shadowValue;
		}



		if (i > 0)
		{
			if (blocco[(count - (sizeX)) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 1) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 1) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 1) + 5] - contrasto / shadowValue;

				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 1) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 1) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 1) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[(count - (sizeX)) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 1) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 1) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 1) + 27 + 5] - contrasto / shadowValue;
																	  
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 1) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 1) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 1) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[((count - (sizeX * sizeZ)) - sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 *1) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 *1) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 *1) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 *1) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 *1) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 *1) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 *1) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 *1) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 *1) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[((count - (sizeX * sizeZ)) - sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 1) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 1) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 1) + 5] - contrasto / shadowValue;
			}
		}
	}
}

void Chunk::addShadowLeft(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	int j = z;
	int i = x;


	if (count >= sizeX * sizeZ && count < sizeX * sizeZ * sizeY && i > 0)
	{


		if (blocco[(count - (sizeX * sizeZ)) - (1)].id > 0)
		{
			allVertices[allOff + 0 + 3] = TriangleVertices[(36 * 3) + 0 + 3] - contrasto / shadowValue;
			allVertices[allOff + 0 + 4] = TriangleVertices[(36 * 3) + 0 + 4] - contrasto / shadowValue;
			allVertices[allOff + 0 + 5] = TriangleVertices[(36 * 3) + 0 + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 3) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 3) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 3) + 18 + 5] - contrasto / shadowValue;
		}




		if (blocco[(count + (sizeX * sizeZ)) - (1)].id > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 3) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 3) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 3) + 9 + 5] - contrasto / shadowValue;
																 
			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 3) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 3) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 3) + 27 + 5] - contrasto / shadowValue;
		}



		if (j < sizeX - 1)
		{
			if (blocco[(count + (sizeX)) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 3) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 3) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 3) + 5] - contrasto / shadowValue;

				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 3) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 3) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 3) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[(count - (sizeX)) - (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 3) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 3) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 3) + 27 + 5] - contrasto / shadowValue;
																	  
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 3) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 3) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 3) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[((count - (sizeX * sizeZ)) - sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 3) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 3) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 3) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 3) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 3) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 3) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (j < sizeX - 1)
		{
			if (blocco[((count + (sizeX * sizeZ)) + sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 3) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 3) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 3) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (j < sizeX - 1)
		{
			if (blocco[((count - (sizeX * sizeZ)) + sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 3) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 3) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 3) + 5] - contrasto / shadowValue;
			}
		}
	}
}

void Chunk::addShadowRight(std::vector<Blocco>& blocco, std::vector<float>& allVertices, int count, int allOff, int x, int z)
{
	int j = z;
	int i = x;


	if (count >= sizeX * sizeZ && count < sizeX * sizeZ * sizeY && i < sizeX - 1)
	{


		if (blocco[(count - (sizeX * sizeZ)) + (1)].id > 0)
		{
			allVertices[allOff + 0 + 3] = TriangleVertices[(36 * 2) + 0 + 3] - contrasto / shadowValue;
			allVertices[allOff + 0 + 4] = TriangleVertices[(36 * 2) + 0 + 4] - contrasto / shadowValue;
			allVertices[allOff + 0 + 5] = TriangleVertices[(36 * 2) + 0 + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 2) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 2) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 2) + 18 + 5] - contrasto / shadowValue;
		}




		if (blocco[(count + (sizeX * sizeZ)) + (1)].id > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 2) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 2) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 2) + 9 + 5] - contrasto / shadowValue;

			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 2) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 2) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 2) + 27 + 5] - contrasto / shadowValue;
		}



		if (j < sizeX - 1)
		{
			if (blocco[(count + (sizeX)) + (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 2) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 2) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 2) + 5] - contrasto / shadowValue;

				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 2) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 2) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 2) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[(count - (sizeX)) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 2) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 2) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 2) + 27 + 5] - contrasto / shadowValue;

				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 2) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 2) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 2) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[((count - (sizeX * sizeZ)) - sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 2) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 2) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 2) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (j > 0)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 2) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 2) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 2) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (j < sizeX - 1)
		{
			if (blocco[((count + (sizeX * sizeZ)) + sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 2) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 2) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 2) + 9 + 5] - contrasto / shadowValue;
			}
		}

		if (j < sizeX - 1)
		{
			if (blocco[((count - (sizeX * sizeZ)) + sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 2) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 2) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 2) + 5] - contrasto / shadowValue;
			}
		}
	}
}

void Chunk::Create(int x1, int y1, GLuint vbo, GLuint ebo, std::vector<float> &allVertices)
{
	uvCord uv;
	x = x1;
	z = y1;
	id = x + z * 41;
	float y = 256;
	sizeX = size * sizeX / scale;
	sizeZ = size * sizeZ / scale;

	std::cout << id << '\n';

	float heights[1000];

	float generalHeight = ((perlin((1000000.0f + x)/20, (1000000.0f + z)/20) + 1) / 2) * 200;

	for (int i = 0; i < sizeX; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			generalHeight = ((perlin((1000000.0f + x + ((1.0f / 16) * j)) / 5, (1000000.0f + z + ((1.0f / 16) * i)) / 5) + 1) / 2);
			generalHeight *= generalHeight;
			generalHeight *= generalHeight*2;
			generalHeight *= 200;

			heights[j + i * sizeX] = ((perlin(1000000 + x + (1.0f / 16)*j+1, 1000000 + z + (1.0f / 16)*i+1) + 1) * 0.5f) * generalHeight;

		}
	}


	{
		std::vector<Blocco> blocco0(sizeX*sizeY*sizeZ);
		indices0.resize(80000);


		for (int i = 0; i < (16 * 16 * 128); i++)
		{
			posIsBlock[i] = 0;
		}

		int faceCount = 0;

		
	
		int jk;
		int count = 0;
		for (int y1 = 0; y1 < 126; y1++)
		{
			for (int z2 = 0; z2 < sizeZ; z2 ++)
			{
				for (int x2 = 0; x2 < sizeX; x2++)
				{
					if (y1 < 15)
					{
						jk = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
						blocco0[jk] = { count };
						blocco0[jk].chunkReference = count;
						blocco0[jk].id = stone;
						posIsBlock[jk] = stone;
					}

					else if (y1 - 15 <= heights[x2 + z2 * sizeX] && heights[x2 + z2 * sizeX] + 15 < 127)
					{
						jk = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
						blocco0[jk] = { count };
						blocco0[jk].chunkReference = count;
						blocco0[jk].id = dirtGrass;
						posIsBlock[jk] = dirtGrass;

						if (heights[x2 + z2 * sizeX] - (y1-15) > 4)
						{
							blocco0[jk].id = stone;
							posIsBlock[jk] = stone;
						}

						else if (heights[x2 + z2 * sizeX] - (y1-15) > 1)
						{
							blocco0[jk].id = dirt;
							posIsBlock[jk] = dirt;
						}

					
					
						
						count++;
					}

					else if (y1 < 20)
					{
						jk = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
						blocco0[jk].id = water;
						posIsBlock[jk] = water;
					}
				}
			}
		}


		blocco.resize(sizeX * sizeY * sizeZ);
		blocco = blocco0;

		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{

					if (posIsBlock[k + (j * sizeX) + (i * sizeX * sizeZ)] != air && posIsBlock[k + (j * sizeX) + (i * sizeX * sizeZ)] != water)
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

					else if(posIsBlock[k + (j * sizeX) + (i * sizeX * sizeZ)] != water)
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
		int allOff = 0;
		int count2 = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					count2 = k + (j * sizeX) + (i * sizeX * sizeZ);

					if (blocco[count2].RenderFront)
					{
						for (int m = 0; m < 36; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;

						}

						allOff -= 36;

						addShadowFront(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;


						faceCount++;

					}

					if (blocco[count2].RenderBack)
					{

						for (int m = 36 * 1; m < 72; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;




							allOff += 9;

						}

						allOff -= 36;

						addShadowBack(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count2].RenderRight)
					{

						for (int m = 36 * 2; m < 108; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;

						}

						allOff -= 36;

						addShadowRight(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count2].RenderLeft)
					{

						for (int m = 36 * 3; m < 36 * 4; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;




							allOff += 9;

						}

						allOff -= 36;

						addShadowLeft(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}


					if (blocco[count2].RenderTop)
					{

						for (int m = 36 * 4; m < 36 * 5; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							allOff += 9;

						}

						allOff -= 36;

						addShadowTop(blocco, allVertices, count2, allOff,k,j);


						allOff += 36;

						faceCount++;
					}


					if (blocco[count2].RenderBottom)
					{
						for (int m = 36 * 5; m < 216; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;
						}

						allOff -= 36;

						addShadowBottom(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;
					}
				}
			}
		}


		int iCount = 0;

		for (int i = 0; i < faceCount * 6; i += 6)
		{

			indices0[i] = iCount;
			indices0[i + 1] = 1 + iCount;
			indices0[i + 2] = 2 + iCount;
			indices0[i + 3] = 1 + iCount;
			indices0[i + 4] = 2 + iCount;
			indices0[i + 5] = 3 + iCount;
			iCount += 4;

		}

		if (faceCount > 0)
		{
			allVertices.resize(faceCount * 36);

			indices0.resize(faceCount * 6);
		}
		int size1 = indices0.size();


	}

	created = true;
	
}


void Chunk::preRender(GLuint vbo, std::vector<float>& allVertices, GLuint ebo)
{

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (allVertices.size()) * sizeof(float), allVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices0.size() * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	allVertices.resize(216 * 16 * 16 * 128);

	indexNumbers = indices0.size();

	ready = true;

}

void Chunk::Update(int x1, int z1, int y1, bool build, GLuint ebo, GLuint vbo, std::vector<float> &allVertices, int id)
{
	uvCord uv;
	allVertices.resize(216 * 16 * 16 * 128);
	int chunkReference = (x1)+(z1 * sizeX) + (y1 * sizeX * sizeZ);
	int l = 0;
	int k = x1; int j = z1; int i = y1;
	if (build && !(posIsBlock[chunkReference] != air))
	{
		posIsBlock[chunkReference] = id;

		
		blocco[chunkReference].RenderFront = true;
		blocco[chunkReference].RenderBack = true;
		blocco[chunkReference].RenderRight = true;
		blocco[chunkReference].RenderLeft = true;
		blocco[chunkReference].RenderTop = true;
		blocco[chunkReference].RenderBottom = true;
		blocco[chunkReference].id = id;

		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}

		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}
		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}

			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}

		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}
		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}

			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = true;
				l += 6;
			}

		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = true;
				l += 6;
			}
		}



		blocco[chunkReference].RenderFront = true;
		blocco[chunkReference].RenderBack = true;
		blocco[chunkReference].RenderRight = true;
		blocco[chunkReference].RenderLeft = true;
		blocco[chunkReference].RenderTop = true;
		blocco[chunkReference].RenderBottom = true;
		blocco[chunkReference].id = id;
		l += 36;

		indices0.resize(indices0.size() + 100);
		allVertices.resize(allVertices.size() + 500);

		l = 0;

		std::fill(indices0.begin(), indices0.end(), 0);

		int faceCount = 0;

		int count1 = 0;
		int allOff = 0;
		int count = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					count = k + (j * sizeX) + (i * sizeX * sizeZ);

					if (blocco[count].RenderFront)
					{
						for (int m = 0; m < 36; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;

						}

						allOff -= 36;

						addShadowFront(blocco, allVertices, count, allOff, k, j);

						allOff += 36;


						faceCount++;

					}

					if (blocco[count].RenderBack)
					{

						for (int m = 36 * 1; m < 72; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;




							allOff += 9;

						}

						allOff -= 36;

						addShadowBack(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count].RenderRight)
					{
						for (int m = 36 * 2; m < 36 * 3; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];

							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];

							allVertices[allOff + 8] = 45987;



							allOff += 9;
						}

						allOff -= 36;

						addShadowRight(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count].RenderLeft)
					{

						for (int m = 36 * 3; m < 36 * 4; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];

							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];

							allVertices[allOff + 8] = 45987;



							allOff += 9;
						}

						allOff -= 36;

						addShadowLeft(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}


					if (blocco[count].RenderTop)
					{

						for (int m = 36 * 4; m < 36 * 5; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;

							allOff += 9;

						}

						allOff -= 36;

						addShadowTop(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;
					}


					if (blocco[count].RenderBottom)
					{
						for (int m = 36 * 5; m < 216; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;
						}

						allOff -= 36;

						addShadowBottom(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;
					}

				}

			}
		}


		int iCount = 0;

		for (int i = 0; i < faceCount * 6; i += 6)
		{

			indices0[i] = iCount;
			indices0[i + 1] = 1 + iCount;
			indices0[i + 2] = 2 + iCount;
			indices0[i + 3] = 1 + iCount;
			indices0[i + 4] = 2 + iCount;
			indices0[i + 5] = 3 + iCount;
			iCount += 4;

		}

		allVertices.resize(faceCount * 36);

		indices0.resize(faceCount * 6);


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(float), allVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices0.size() * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexNumbers = indices0.size();
		allVertices.resize(216*16*16*128);
		}

	else if (!build)
	{

		l = 0;
		posIsBlock[chunkReference] = false;
		blocco[chunkReference].id = air;
		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}

		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].RenderLeft = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].RenderRight = true;
				l += 6;
			}
		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}

			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}

		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].RenderBack = true;
				l += 6;
			}
		}

		else
		{
			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].RenderFront = true;
				l += 6;
			}
		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}

			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].RenderTop = true;
				l += 6;
			}

		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].RenderBottom = true;
				l += 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
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

		indices0.resize(indices0.size() + 100);
		allVertices.resize(allVertices.size() + 500);

		l = 0;

		std::fill(indices0.begin(), indices0.end(), 0);

		int faceCount = 0;

		int count1 = 0;
		int allOff = 0;
		int count = 0;
		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					count = k + (j * sizeX) + (i * sizeX * sizeZ);
					if (blocco[count].RenderFront)
					{
						for (int m = 0; m < 36; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;

						}

						allOff -= 36;

						addShadowFront(blocco, allVertices, count, allOff, k, j);

						allOff += 36;


						faceCount++;

					}

					if (blocco[count].RenderBack)
					{

						for (int m = 36 * 1; m < 72; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;




							allOff += 9;

						}

						allOff -= 36;

						addShadowBack(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count].RenderRight)
					{

						for (int m = 36 * 2; m < 108; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;

							allOff += 9;

						}

						allOff -= 36;

						addShadowRight(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count].RenderLeft)
					{

						for (int m = 36 * 3; m < 36 * 4; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;

							allOff += 9;

						}

						allOff -= 36;

						addShadowLeft(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count].RenderTop)
					{

						for (int m = 36 * 4; m < 36 * 5; m += 9)
						{

							allVertices[allOff] = TriangleVertices[m] + (sizeX * x) + k;
							allVertices[allOff + 1] = TriangleVertices[m + 1] + i;
							allVertices[allOff + 2] = TriangleVertices[m + 2] + (sizeX * z) + j;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;

							allOff += 9;

						}

						allOff -= 36;

						addShadowTop(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;
					}


					if (blocco[count].RenderBottom)
					{
						for (int m = 36 * 5; m < 216; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + (sizeX * (x * scale)) + (k * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + i;
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + (sizeX * (z * scale)) + (scale * j);
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count].id)];
							allVertices[allOff + 8] = 45987;



							allOff += 9;
						}

						allOff -= 36;

						addShadowBottom(blocco, allVertices, count, allOff, k, j);

						allOff += 36;

						faceCount++;
					}

				}

			}
		}


		int iCount = 0;

		for (int i = 0; i < faceCount * 6; i += 6)
		{

			indices0[i] = iCount;
			indices0[i + 1] = 1 + iCount;
			indices0[i + 2] = 2 + iCount;
			indices0[i + 3] = 1 + iCount;
			indices0[i + 4] = 2 + iCount;
			indices0[i + 5] = 3 + iCount;
			iCount += 4;

		}

		if (faceCount > 0)
		{
			allVertices.resize(faceCount * 36);

			indices0.resize(faceCount * 6);
		}

		int size1 = indices0.size();



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size1 * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, allVertices.size()* sizeof(float), allVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		indexNumbers = indices0.size();
		allVertices.resize(216 * 16 * 16 * 128);
	}
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



