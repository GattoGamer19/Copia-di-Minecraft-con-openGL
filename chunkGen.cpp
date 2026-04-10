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
#include "vars.h"


typedef struct {
	float x, y;
} vector2;

vector2 randomGradient(int ix, int iy, int seed) {

	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2;
	unsigned a = ix, b = iy;
	a *= seed;

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
float dotGridGradient(int ix, int iy, float x, float y, int seed) {
	// Get gradient from integer coordinates
	vector2 gradient = randomGradient(ix, iy, seed);

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
float perlin(float x, float y, int seed) {

	// Determine grid cell corner coordinates
	int x0 = (int)floor(x);
	int y0 = (int)floor(y);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	// Compute Interpolation weights
	float sx = x - (float)x0;
	float sy = y - (float)y0;

	// Compute and interpolate top two corners
	float n0 = dotGridGradient(x0, y0, x, y, seed);
	float n1 = dotGridGradient(x1, y0, x, y, seed);
	float ix0 = interpolate(n0, n1, sx);

	// Compute and interpolate bottom two corners
	n0 = dotGridGradient(x0, y1, x, y, seed);
	n1 = dotGridGradient(x1, y1, x, y, seed);
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
	int y = ((count - i) - (j * sizeX)) / (sizeX * sizeZ);

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

		else if (getBlock(j - 1,i,y + 1) > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;

			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;
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

		else if (getBlock(j + 1, i, y + 1) > 0)
		{
			allVertices[allOff + 3] = TriangleVertices[(36 * 4) + 3] - contrasto / shadowValue;
			allVertices[allOff + 4] = TriangleVertices[(36 * 4) + 4] - contrasto / shadowValue;
			allVertices[allOff + 5] = TriangleVertices[(36 * 4) + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 4) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 4) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 4) + 18 + 5] - contrasto / shadowValue;
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

		else if (getBlock(j, i - 1, y + 1) > 0)
		{
			allVertices[allOff + 3] = TriangleVertices[(36 * 4) + 3] - contrasto / shadowValue;
			allVertices[allOff + 4] = TriangleVertices[(36 * 4) + 4] - contrasto / shadowValue;
			allVertices[allOff + 5] = TriangleVertices[(36 * 4) + 5] - contrasto / shadowValue;

			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;
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

		else if (getBlock(j, i + 1, y + 1) > 0)
		{
			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;

			allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 4) + 18 + 3] - contrasto / shadowValue;
			allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 4) + 18 + 4] - contrasto / shadowValue;
			allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 4) + 18 + 5] - contrasto / shadowValue;
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
	int y = count % (sizeX * sizeZ);

	if (count >= sizeX * sizeZ && count < sizeX * sizeZ * sizeY && j < sizeX - 1)
	{


		if (j > 0)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (sizeX)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 5) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 5) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 5) + 9 + 5] - contrasto / shadowValue;

				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;



			}
		}

		else if (getBlock(j - 1, i, y) > 0)
		{
			allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 4) + 9 + 3] - contrasto / shadowValue;
			allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 4) + 9 + 4] - contrasto / shadowValue;
			allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 4) + 9 + 5] - contrasto / shadowValue;

			allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 4) + 27 + 3] - contrasto / shadowValue;
			allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 4) + 27 + 4] - contrasto / shadowValue;
			allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 4) + 27 + 5] - contrasto / shadowValue;
		}

		if (j < sizeX - 1)
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
		}


		if (i > 0)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (1)].id > 0)
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
			if (blocco[(count - (sizeX * sizeZ)) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;

				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 5) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 5) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 5) + 18 + 5] - contrasto / shadowValue;


			}
		}

		if (i > 0 && j < sizeX - 1)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (1 - sizeX)].id > 0)
			{
				allVertices[allOff + 3] = TriangleVertices[(36 * 5) + 3] - contrasto / shadowValue;
				allVertices[allOff + 4] = TriangleVertices[(36 * 5) + 4] - contrasto / shadowValue;
				allVertices[allOff + 5] = TriangleVertices[(36 * 5) + 5] - contrasto / shadowValue;

			}
		}

		if (i > 0 && j > 0)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (1 + sizeX)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 5) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 5) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 5) + 9 + 5] - contrasto / shadowValue;

			}
		}

		if (i < sizeX - 1 && j < sizeX - 1)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (-1 - sizeX)].id > 0)
			{

				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 5) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 5) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 5) + 18 + 5] - contrasto / shadowValue;

			}
		}

		if (i < sizeX - 1 && j > 0)
		{
			if (blocco[(count - (sizeX * sizeZ)) - (-1 + sizeX)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 5) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 5) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 5) + 27 + 5] - contrasto / shadowValue;

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
				allVertices[allOff + 18 + 3] = TriangleVertices[(36 * 1) + 18 + 3] - contrasto / shadowValue;
				allVertices[allOff + 18 + 4] = TriangleVertices[(36 * 1) + 18 + 4] - contrasto / shadowValue;
				allVertices[allOff + 18 + 5] = TriangleVertices[(36 * 1) + 18 + 5] - contrasto / shadowValue;
			}
		}

		if (i < sizeX - 1)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) + (1)].id > 0)
			{
				allVertices[allOff + 27 + 3] = TriangleVertices[(36 * 1) + 27 + 3] - contrasto / shadowValue;
				allVertices[allOff + 27 + 4] = TriangleVertices[(36 * 1) + 27 + 4] - contrasto / shadowValue;
				allVertices[allOff + 27 + 5] = TriangleVertices[(36 * 1) + 27 + 5] - contrasto / shadowValue;
			}
		}

		if (i > 0)
		{
			if (blocco[((count + (sizeX * sizeZ)) - sizeX) - (1)].id > 0)
			{
				allVertices[allOff + 9 + 3] = TriangleVertices[(36 * 1) + 9 + 3] - contrasto / shadowValue;
				allVertices[allOff + 9 + 4] = TriangleVertices[(36 * 1) + 9 + 4] - contrasto / shadowValue;
				allVertices[allOff + 9 + 5] = TriangleVertices[(36 * 1) + 9 + 5] - contrasto / shadowValue;
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

int Chunk::getBlock(int z1, int x1, int y1)
{
	return air;

	float heights[1000];
	float stoneHeights[1000];
	float snowHeights[1000];

	int x2 = x1;
	int z2 = z1;

	int z3 = z;
	int x3 = x;

	if (x2 == -1)
	{
		x2 = (chunkSize - 1);
		x--;
	}

	if (z2 == -1)
	{
		z2 = (chunkSize - 1);
		z--;
	}

	if (x2 == chunkSize)
	{
		x2 = 0;
		x++;
	}

	if (z2 == chunkSize)
	{
		z2 = 0;
		z++;
	}

	x1 = x2;
	z1 = z2;



	float generalHeight = ((perlin((1000000.0f + x) / 20, (1000000.0f + z) / 20, seed) + 1) / 2) * 200;
	float pianeggiantezza = -0.5f + ((perlin((100000.0f + (x / 3.0f)) / 5, (100000.0f + (z / 3.0f)) / 5, seed) + 1) / 2) * 3;

	float cx = x + 100000.0f;
	float cf = (1.0f / chunkSize);
	float cz = z + 100000.0f;

	generalHeight = ((perlin((cx + (cf * x1)) / 5, (cz + (cf * z1)) / 5, seed) + 1) / 2);
	pianeggiantezza = -0.5f + ((perlin((100000.0f + ((x + (cf * x1)) / 4.5f)) / 5, (100000.0f + ((z + (cf * z1)) / 4.5f)) / 5, seed) + 1) / 2) * 3;
	pianeggiantezza *= pianeggiantezza;
	generalHeight *= generalHeight;
	generalHeight *= generalHeight * 1.1f;
	generalHeight *= 90 * pianeggiantezza;



	heights[x2 + z2 * sizeX] = (pianeggiantezza * pianeggiantezza * chunkSize) + ((perlin(1000000 + 1 + x + (1.0f / chunkSize) * x1 + 1, 1000000 + 1 + z + (1.0f / chunkSize) * z1 + 1, seed) + 1) * 0.5f) * generalHeight;
	stoneHeights[x2 + z2 * sizeX] = 50 + heights[x1 + z1 * sizeX] - ((perlin(1000000 + 1 + (x * 3.5f) + (3.5f / chunkSize) * x1 + 1, 1000000 + 1 + (z * 3.5f) + (3.5f / chunkSize) * z1 + 1, seed) + 1) * 0.5f) * pianeggiantezza * pianeggiantezza * 30.7f;
	snowHeights[x2 + z2 * sizeX] = 150 - ((perlin(1000000 + 1 + (x * 3.5f) + (3.5f / chunkSize) * x1 + 1, 1000000 + 1 + (z * 3.5f) + (3.5f / chunkSize) * z1 + 1, seed) + 1) * 0.5f) * pianeggiantezza * (pianeggiantezza)*pianeggiantezza * pianeggiantezza;

	int jk = 0;

	z = z3;
	x = x3;

	if (y1 < minHeight || ((y1 > 10) && (y1 > (snowHeights[x2 + z2 * sizeX])) && y1 < heights[x2 + z2 * sizeX]))
	{
		return snow;

	}

	else if (y1 < minHeight || ((y1 > 10) && (y1 > stoneHeights[x2 + z2 * sizeX]) && y1 < heights[x2 + z2 * sizeX]))
	{
		return stone;

	}

	else if (y1 - minHeight <= heights[x2 + z2 * sizeX] && heights[x2 + z2 * sizeX] + minHeight < sizeY - 1)
	{
		return dirtGrass;

		if (heights[x2 + z2 * sizeX] - (y1 - minHeight) > 4)
		{
			return stone;
		}

		else if (heights[x2 + z2 * sizeX] - (y1 - minHeight) > 1)
		{

			return dirt;
		}

		/*
		if (x2 == 0 || x2 == 15 || z2 == 0 || z2 == 15)
		{
			blocco0[jk].id = stone;
			posIsBlock[jk] = stone;
		}

		*/

	}

	else if (y1 < minHeight + 5)
	{
		return water;
	}

	return air;

}

void Chunk::placeTree(std::vector<unsigned char>& posIsBlock, std::vector<Blocco>& blocco, int _x, int _z, int _y)
{

	int count = 0;
	int count1 = 0;

	float tx = 10000.3f + ((((_x * scale) - (offsetX / 2))) + (x * ((sizeX * scale) - (offsetX))));
	float tz = 10000.2f + ((((_z * scale) - (offsetX / 2))) + (z * ((sizeX * scale) - (offsetX))));

	float treeHeightDelta = 5 + (perlin(tx, tz, seed) + 1) * 2;
	float treeHeight = 4 + treeHeightDelta;
	float leavesHeight = 3 + (treeHeightDelta / 2);
	float leavesHeightEnd = 8 + (treeHeightDelta / 1.5f);
	float leavesRadius = 3 + (treeHeightDelta / 2);
	float leavesHeightCenter = (leavesHeight + leavesHeightEnd) / 2;

	for (int y1 = _y; y1 < _y + treeHeight; y1++)
	{
		if (y1 < sizeY - 2)
		{
			int index = _x + (sizeZ * _z) + (sizeX * sizeZ * y1);
			posIsBlock[index] = oakLog;
			blocco[index].id = oakLog;
		}
	}


	for (int y1 = _y + leavesHeight; y1 < _y + leavesHeightEnd; y1++)
	{
		float centerDistY = abs(y1 - (_y + leavesHeightCenter));

		for (int z1 = _z - (leavesRadius - count); z1 < _z + (leavesRadius - count); z1++)
		{
			float centerDistZ = abs(z1 - _z);

			for (int x1 = _x - (leavesRadius - count); x1 < _x + (leavesRadius - count); x1++)
			{
				float centerDist = sqrtf((abs(x1 - _x) * abs(x1 - _x)) + (centerDistZ * centerDistZ) + (centerDistY * centerDistY));

				if (x1 >= 0 && x1 < sizeX && z1 >= 0 && z1 < sizeX && y1 < sizeY - 2)
				{
					float px = 10000.3f + ((((x1 * scale) - (offsetX / 2))) + (x * ((sizeX * scale) - (offsetX))));
					float pz = 10000.2f + ((((z1 * scale) - (offsetX / 2))) + (z * ((sizeX * scale) - (offsetX))));
					float leaves = (perlin(px, pz, seed) + 1);

					leaves /= centerDist / (2 / (treeHeightDelta / 2));

					//std::cout << leaves << '\n';
					if (leaves > 0.14f)
					{
						int index = x1 + (sizeZ * z1) + (sizeX * sizeZ * y1);

						if (posIsBlock[index] != oakLog)
						{
							posIsBlock[index] = oakLeaves;
							blocco[index].id = oakLeaves;
						}
					}
				}
			}

		}
		count++;
		if (count > 4)
			count = 4;
	}

}

void Chunk::Create(int x1, int y1, VBO& vbo, EBO& ebo, std::vector<float>& allVertices, bool hasBeenModified, int offset)
{
	uvCord uv;
	x = x1;
	z = y1;


	sizeX = (size * sizeX) + offsetX;
	sizeZ = (size * sizeZ) + offsetX;
	posIsBlock.resize(sizeX * scale * sizeX * scale * sizeY);

	float amplifier = 1;
	float multiplier = chunkSize / 16;

	x *= multiplier;
	z *= multiplier;

	std::vector<float> heights(20000);
	std::vector<float> stoneHeights(20000);
	std::vector<float> snowHeights(20000);
	std::vector<float> verdos1(20000);

	float generalHeight = ((perlin((1000000.0f + x) / 20, (1000000.0f + z) / 20, seed) + 1) / 2) * 200;
	float pianeggiantezza = -0.5f + ((perlin((100000.0f + (x / 3.0f)) / 5, (100000.0f + (z / 3.0f)) / 5, seed) + 1) / 2) * 3;
	
	
	float cx = x;
	float cf = (1.0f / (chunkSize / multiplier));
	float cz = z;
	srand(seed);

	//std::cout << "pianeggiantezza: " << pianeggiantezza << '\n';

	for (int i = -(offsetX/2); i < (sizeX - (offsetX/2)); i++)
	{

		for (int j = -(offsetX / 2); j < (sizeX - (offsetX / 2)); j++)
		{
			generalHeight = ((perlin((cx + (cf * j)) / (5 * amplifier), (cz + (cf * i)) / (5 * amplifier), seed) + 1) / 2);
			pianeggiantezza = -0.5f + ((perlin((((x + (cf * j)) / 4.5f)) / (5 * amplifier), (((z + (cf * i)) / 4.5f)) / (5 * amplifier), seed) + 1) / 2) * 3;
			pianeggiantezza *= pianeggiantezza * amplifier;
			generalHeight *= generalHeight;
			generalHeight *= generalHeight * 1.1f;
			generalHeight *= 90 * pianeggiantezza;

			int index = (j + (offsetX / 2)) + (i + (offsetX / 2)) * sizeX;

			float verdos = ((perlin((((x + (cf * j)) / 5.5f)) / (5 * amplifier), (((z + (cf * i)) / 5.5f)) / (5 * amplifier), seed) + 1) / 4);
			verdos1[index] = ((perlin(1 + (x * (300.5f * amplifier)) + (((300.5f * amplifier) / (chunkSize / multiplier)) * j) + 1,1 + (z * (300.5f * amplifier)) + (((300.5f * amplifier) / (chunkSize / multiplier)) * i) + 1, seed) + 1) * 0.5f);
			verdos1[index] += verdos;

			heights[index] = (pianeggiantezza * pianeggiantezza * 15) + ((perlin(1 + (x * (1.0f / amplifier)) + (((1.0f / amplifier) / (chunkSize / multiplier)) * j) + 1, 1 + (z * (1.0f / amplifier)) + (((1.0f / amplifier) / (chunkSize / multiplier)) * i) + 1, seed) + 1) * 0.5f) * generalHeight * amplifier;
			stoneHeights[index] = (50 * amplifier) + heights[(j + (offsetX / 2)) + (i + (offsetX / 2)) * sizeX] - ((perlin(1 + (x * (3.5f * amplifier)) + (((3.5f * amplifier) / (chunkSize / multiplier)) * j) + 1, 1 + (z * (3.5f * amplifier)) + (((3.5f * amplifier) / (chunkSize / multiplier)) * i) + 1, seed) + 1) * 0.5f) * pianeggiantezza * pianeggiantezza * pianeggiantezza * (30.7f * amplifier);
			snowHeights[index] = (150 * amplifier) - ((perlin(1 + (x * (3.5f * amplifier)) + (((3.5f * amplifier) / (chunkSize / multiplier)) * j) + 1, 1 + (z * (3.5f * amplifier)) + (((3.5f * amplifier) / (chunkSize / multiplier)) * i) + 1, seed) + 1) * 0.5f) * pianeggiantezza * pianeggiantezza * pianeggiantezza;
		}

	}

	z = y1;
	x = x1;

	sizeX /= scale;
	sizeZ /= scale;

	offsetX /= scale;
	offsetZ /= scale;

	{
		
		

		int faceCount = 0;


		
		if (!hasBeenModified)
		{
			std::vector<Blocco> blocco0(sizeX * sizeY * sizeZ);

			for (int i = 0; i < (sizeX * sizeX * sizeY); i++)
			{
				posIsBlock[i] = 0;
			}



			int jk;
			int count = 0;
			for (int y1 = 0; y1 < sizeY - 2; y1++)
			{
				for (int z2 = 0; z2 < sizeZ; z2++)
				{
					for (int x2 = 0; x2 < sizeX; x2++)
					{
						//verdos1 = -0.5f + perlin((100000.0f + (x2 * 5.33f)) / 5.0f, (100000.0f + (z2 * 5.33f)) / 5.0f) * 3;
						//verdos = -0.5f + ((perlin((200000.0f + (x / 3.0f)) / 5, (200000.0f + (z / 3.0f)) / 5) + 1) / 2) * 3;

						jk = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
						if (posIsBlock[jk] == 0)
						{

						
							if (((y1 > 10) && (y1 > (minHeight + snowHeights[(x2 * scale) + (z2 *  scale * scale * sizeX)])) && y1 < (minHeight + heights[(x2 * scale) + (z2 *  scale * scale * sizeX)])))
							{

								blocco0[jk].id = snow;
								posIsBlock[jk] = snow;

							}

							else if (y1 < minHeight || ((y1 > 10) && (y1 > (minHeight + stoneHeights[(x2 * scale) + (z2 *  scale * scale * sizeX)])) && y1 < (minHeight + heights[(x2 * scale) + (z2 *  scale * scale * sizeX)])))
							{

								blocco0[jk].id = stone;
								posIsBlock[jk] = stone;

							}


							else if (y1 - minHeight <= heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] && heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] + minHeight < sizeY - 1)
							{

								blocco0[jk].id = dirtGrass;
								posIsBlock[jk] = dirtGrass;

								if (heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] - (y1 - minHeight) > 4)
								{
									blocco0[jk].id = stone;
									posIsBlock[jk] = stone;
								}

								else if (heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] - (y1 - minHeight) > 1)
								{

									blocco0[jk].id = dirt;
									posIsBlock[jk] = dirt;
								}

					

								
								/*
								if (x2 == 0 || x2 == 15 || z2 == 0 || z2 == 15)
								{
									blocco0[jk].id = stone;
									posIsBlock[jk] = stone;
								}

								*/

								count++;
							}

							else if (y1 < minHeight + 5)
							{

								blocco0[jk].id = water;
								posIsBlock[jk] = water;
							}

							else
							{
								blocco0[jk].id = air;
								posIsBlock[jk] = air;
							}

						}

					}
				}
			}

			for (int y1 = 1; y1 < sizeY - 2; y1++)
			{
				for (int z2 = 0; z2 < sizeZ; z2++)
				{
					for (int x2 = 0; x2 < sizeX; x2++)
					{
						//verdos1 = -0.5f + perlin((100000.0f + (x2 * 5.33f)) / 5.0f, (100000.0f + (z2 * 5.33f)) / 5.0f) * 3;
						//verdos = -0.5f + ((perlin((200000.0f + (x / 3.0f)) / 5, (200000.0f + (z / 3.0f)) / 5) + 1) / 2) * 3;

						jk = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
						if (y1 - minHeight == (int)heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] && heights[(x2 * scale) + (z2 *  scale * scale * sizeX)] + minHeight < sizeY - 1)
						{
							if ((verdos1[(x2 * scale) + (z2 *  scale * scale * sizeX)] > 1.05f) && (y1 > minHeight + 10))
							{
								if(posIsBlock[x2 + (z2 * sizeX) + (y1 * sizeX * sizeX)] == dirtGrass)
								placeTree(posIsBlock, blocco0, x2, z2, y1 + 1);
							}
						}
					}
				}
			}

			blocco.resize(sizeX * sizeY * sizeZ);
			blocco = blocco0;

		}

			else
			{
				sizeX -= offsetX;
				sizeZ -= offsetX;
				offsetX = 0;



				std::vector<Blocco> blocco0(sizeX * sizeY * sizeZ);

				for (int y1 = 0; y1 < sizeY - 2; y1++)
				{
					for (int z2 = 0; z2 < sizeZ; z2++)
					{
						for (int x2 = 0; x2 < sizeX; x2++)
						{
							float index = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
							float index1 = (x2 * scale) + (sizeZ * scale * z2 * scale) + (sizeX * scale * sizeZ * scale * y1);
							blocco0[index].id = posIsBlock[index1];
						}
					}
				}

				if (scale != 1)
				{
					std::vector<unsigned char> _posIsBlock(sizeX* sizeZ* sizeY);

					for (int y1 = 0; y1 < sizeY - 2; y1++)
					{
						for (int z2 = 0; z2 < sizeZ; z2++)
						{
							for (int x2 = 0; x2 < sizeX; x2++)
							{
								float index = x2 + (sizeZ * z2) + (sizeX * sizeZ * y1);
								float index1 = (x2 * scale) + (sizeZ * scale * z2 * scale) + (sizeX * scale * sizeZ * scale * y1);
								_posIsBlock[index] = posIsBlock[index1];
							}
						}
					}

					posIsBlock = _posIsBlock;

				}

				blocco.resize(sizeX* sizeY* sizeZ);
				blocco = blocco0;

			}


		


		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeZ; k++)
				{
					int index = k + (j * sizeX) + (i * sizeX * sizeZ);

					if (posIsBlock[index] != air)
					{
						if (k > 0 && k < sizeZ - 1)
						{	
							if((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + 1].id == water))
							blocco[index + 1].renderFlags &= ~(1 << 3);
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - 1].id == water))
							blocco[index - 1].renderFlags &= ~(1 << 2);
						}
						else if (k == 0)
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + 1].id == water))
							blocco[index + 1].renderFlags &= ~(1 << 3);


						}

						else
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - 1].id == water))
							blocco[index - 1].renderFlags &= ~(1 << 2);

						}



						if (j > 0 && j < sizeX - 1)
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + sizeX].id == water))
							blocco[index + sizeX].renderFlags &= ~(1 << 1);
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - sizeX].id == water))
							blocco[index - sizeX].renderFlags &= ~(1 << 0);
						}
						else if (j == 0)
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + sizeX].id == water))
							blocco[index + sizeX].renderFlags &= ~(1 << 1);

						}

						else
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - sizeX].id == water))
							blocco[index - sizeX].renderFlags &= ~(1 << 0);

						}



						if (i > 0 && i < sizeY - 1)
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + (sizeX * sizeZ)].id == water))
							blocco[index + (sizeX * sizeZ)].renderFlags &= ~(1 << 5);
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - (sizeX * sizeZ)].id == water))
							blocco[index - (sizeX * sizeZ)].renderFlags &= ~(1 << 4);
						}
						else if (i == 0)
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index + (sizeX * sizeZ)].id == water))
							blocco[index + (sizeX * sizeZ)].renderFlags &= ~(1 << 5);
							blocco[index].renderFlags &= ~(1 << 5);
						}
						else
						{
							if ((posIsBlock[index] != water) || (posIsBlock[index] == water && blocco[index - (sizeX * sizeZ)].id == water))
							blocco[index - (sizeX * sizeZ)].renderFlags &= ~(1 << 4);
						}




					}

					else if (posIsBlock[index] != water)
					{
						blocco[index].renderFlags &= ~(1 << 0);
						blocco[index].renderFlags &= ~(1 << 1);
						blocco[index].renderFlags &= ~(1 << 2);
						blocco[index].renderFlags &= ~(1 << 3);
						blocco[index].renderFlags &= ~(1 << 4);
						blocco[index].renderFlags &= ~(1 << 5);

					}


				}
			}
		}



		int count1 = 0;
		int allOff = 0;
		int count2 = 0;
		for (int i = sizeY - 1; i >= 0; i--)
		{
			for (int j = (offsetX/2); j < sizeX - (offsetX / 2); j++)
			{
				for (int k = (offsetX / 2); k < sizeZ - (offsetX / 2); k++)
				{
					count2 = k + (j * sizeX) + (i * sizeX * sizeZ);

					if (blocco[count2].renderFlags & (1 << 4) && scale > 1)
					{
						if (k == offsetX / 2)
						{
							blocco[count2].renderFlags |= (1 << 2);
							if (i >= 1)
							{
								blocco[count2 - (sizeX * sizeZ)].renderFlags |= (1 << 2);
							}
						}
							
						if (k == sizeX - (offsetX / 2))
						{
							blocco[count2 - (sizeX * sizeZ)].renderFlags |= (1 << 3);
						}
							
						if (j == (offsetX / 2))
						{
							blocco[count2 - (sizeX * sizeZ)].renderFlags |= (1 << 0);
						}

						if (j == sizeX - (offsetX / 2))
						{
							blocco[count2 - (sizeX * sizeZ)].renderFlags |= (1 << 1);
						}
							
					}

					if (blocco[count2].renderFlags & (1 << 0))
					{
						for (int m = 0; m < 36; m += 9)
						{
							
								

							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

							allOff += 9;

						}

						allOff -= 36;

						addShadowFront(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;


						faceCount++;

					}

					if (blocco[count2].renderFlags & (1 << 1))
					{

						for (int m = 36 * 1; m < 72; m += 9)
						{


							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

							allOff += 9;

						}

						allOff -= 36;

						addShadowBack(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count2].renderFlags & (1 << 2))
					{

						for (int m = 36 * 2; m < 108; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

							allOff += 9;

						}

						allOff -= 36;

						addShadowRight(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}

					if (blocco[count2].renderFlags & (1 << 3))
					{

						for (int m = 36 * 3; m < 36 * 4; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

							allOff += 9;

						}

						allOff -= 36;

						addShadowLeft(blocco, allVertices, count2, allOff, k, j);

						allOff += 36;

						faceCount++;

					}


					if (blocco[count2].renderFlags & (1 << 4))
					{

						for (int m = 36 * 4; m < 36 * 5; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

							allOff += 9;

						}

						allOff -= 36;

						addShadowTop(blocco, allVertices, count2, allOff, k, j);


						allOff += 36;

						faceCount++;
					}


					if (blocco[count2].renderFlags & (1 << 5))
					{
						for (int m = 36 * 5; m < 216; m += 9)
						{

							allVertices[allOff] = (scale * TriangleVertices[m]) + ((sizeX - offsetX) * (x * scale)) + ((k - (offsetX / 2)) * scale);
							allVertices[allOff + 1] = (TriangleVertices[m + 1]) + (i);
							allVertices[allOff + 2] = (scale * TriangleVertices[m + 2]) + ((sizeX - offsetX) * (z * scale)) + (scale * (j - (offsetX / 2)));
							allVertices[allOff + 3] = TriangleVertices[m + 3];
							allVertices[allOff + 4] = TriangleVertices[m + 4];
							allVertices[allOff + 5] = TriangleVertices[m + 5];
							allVertices[allOff + 6] = uv.uvCords[(((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 7] = uv.uvCords[1 + (((m) / 9) * 2) + (48 * blocco[count2].id)];
							allVertices[allOff + 8] = 45987;

							if (scale > 1)
							{
								allVertices[allOff] += 1.5f;
								allVertices[allOff + 1] -= (scale);
								allVertices[allOff + 2] += 1.5f;
							}

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

		indices0.resize(faceCount * 6);

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
			verticesSize = (faceCount * 36);

		}
		int size1 = indices0.size();



	}

	//blocco = std::vector<Blocco>();

	sizeX = chunkSize;
	sizeZ = chunkSize;

	offsetX *= scale;
	offsetZ *= scale;

	if (scale == 1)
	{

		std::vector<unsigned char> posIsBlock1(sizeX * sizeX * sizeY);
		std::vector<Blocco> blocco1(sizeX * sizeX * sizeY);

		for (int i = 0; i < sizeY; i++)
		{
			for (int j = 0; j < sizeX; j++)
			{
				for (int k = 0; k < sizeX; k++)
				{
					int index1 = k + (j * sizeX) + (i * sizeX * sizeX);
					int index = (k + (offsetX / 2)) + ((j + (offsetX / 2)) * (sizeX + offsetX)) + (i * (sizeX + offsetX) * (sizeX + offsetX));

					blocco1[index1] = blocco[index];
					posIsBlock1[index1] = posIsBlock[index];
				}
			}
		}

		offsetX = offsetZ;

		//posIsBlock = std::vector<unsigned char>();
		posIsBlock.resize(sizeX * sizeY * sizeZ);
		blocco = std::vector<Blocco>();
		blocco.resize(sizeX * sizeY * sizeZ);

		posIsBlock = posIsBlock1;
		blocco = blocco1;
	}

	if (scale != 1)
	{
		posIsBlock = std::vector<unsigned char>();
		blocco = std::vector<Blocco>();

	}
	
	mutexVBO.lock();
	vbo.allVertices[offset].resize(verticesSize);
	
	mutexVBO.unlock();

	for (int i = 0; i < verticesSize; i++)
	{
		vbo.allVertices[offset][i] = allVertices[i];
	}


	vbo.verticesInizialized = true;

	offsetC = offset;

	created = true;
	std::cout << "chunkCreated" << '\n';
}


void Chunk::preRender(GLuint vbo, std::vector<float>& allVertices, EBO& ebo, int _verticesSize)
{

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, _verticesSize, allVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo.indices[8].size() * sizeof(GLuint), ebo.indices[8].data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

	//allVertices.resize(216 * chunkSize * chunkSize * sizeY);

	indexNumbers = ebo.indices[8].size();
	indexNumber = ebo.indices[8].size();

	//indices0 = std::vector<GLuint>();

	ready = true;

}

void Chunk::Update(int x1, int z1, int y1, bool build, GLuint ebo1, GLuint vbo1, std::vector<float>& allVertices, int id, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{
	uvCord uv;
	//allVertices.resize(216 * chunkSize * chunkSize * sizeY);
	int chunkReference = (x1)+(z1 * sizeX) + (y1 * sizeX * sizeZ);
	int l = 0;
	int k = x1; int j = z1; int i = y1;

	hasBeenModified[x][z] = true;

	if (build && id != 0 && i >= 0)
	{
		posIsBlock[chunkReference] = id;


		blocco[chunkReference].renderFlags |= (1 << 0);
		blocco[chunkReference].renderFlags |= (1 << 1);
		blocco[chunkReference].renderFlags |= (1 << 2);
		blocco[chunkReference].renderFlags |= (1 << 3);
		blocco[chunkReference].renderFlags |= (1 << 4);
		blocco[chunkReference].renderFlags |= (1 << 5);
		blocco[chunkReference].id = id;

		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].renderFlags |= (1 << 3);
				l += 6;
			}
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].renderFlags |= (1 << 2);
				l += 6;
			}

		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].renderFlags |= (1 << 3);
				l += 6;
			}
			
			
		}

		else
		{
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].renderFlags |= (1 << 2);
				l += 6;
			}

			
		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].renderFlags |= (1 << 1);
				l += 6;
			}

			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].renderFlags |= (1 << 0);
				l += 6;
			}

		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].renderFlags |= (1 << 1);
				l += 6;
			}

			
		}

		else
		{
			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].renderFlags |= (1 << 0);
				l += 6;
			}

			

		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].renderFlags |= (1 << 5);
				l += 6;
			}

			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].renderFlags |= (1 << 4);
				l += 6;
			}

		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].renderFlags |= (1 << 5);
				l += 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].renderFlags |= (1 << 4);
				l += 6;
			}
		}



		blocco[chunkReference].renderFlags |= (1 << 0);
		blocco[chunkReference].renderFlags |= (1 << 1);
		blocco[chunkReference].renderFlags |= (1 << 2);
		blocco[chunkReference].renderFlags |= (1 << 3);
		blocco[chunkReference].renderFlags |= (1 << 4);
		blocco[chunkReference].renderFlags |= (1 << 5);
		blocco[chunkReference].id = id;
		l += 36;

		indices0.resize(indices0.size() + 100);

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

					if (blocco[count].renderFlags & (1 << 0))
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

					if (blocco[count].renderFlags & (1 << 1))
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

					if (blocco[count].renderFlags & (1 << 2))
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

					if (blocco[count].renderFlags & (1 << 3))
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


					if (blocco[count].renderFlags & (1 << 4))
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


					if (blocco[count].renderFlags & (1 << 5))
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

		indices0.resize(faceCount * 6);

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

		verticesSize = (faceCount * 36);

		


		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), allVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices0.size() * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		indexNumbers = indices0.size();
		indexNumber = indices0.size();
		//allVertices.resize(216 * chunkSize * chunkSize * sizeY / 10);
	}

	else if (!build && i >= 0)
	{

		l = 0;
		posIsBlock[chunkReference] = 0;
		blocco[chunkReference].id = air;
		if (k > 0 && k < sizeZ - 1)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].renderFlags |= (1 << 3);
				l += 6;
			}
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].renderFlags |= (1 << 2);
				l += 6;
			}

		}
		else if (k == 0)
		{
			if (posIsBlock[chunkReference + 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + 1].renderFlags |= (1 << 3);
				l += 6;
			}

			int cx = chunkAssignedVBO[x - 1][z][0];
			int cz = chunkAssignedVBO[x - 1][z][1];
			char otherChunkBlock = chunk[cx][cz].posIsBlock[(chunkSize - 1) + (j * sizeX) + (i * sizeX * sizeZ)];
			chunk[cx][cz].Update((chunkSize - 1), j, i, true, ebo[cx][cz].id, vbo[cx][cz].ids[1], allVerticesX, otherChunkBlock, chunkAssignedVBO);

		}

		else
		{
			if (posIsBlock[chunkReference - 1] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - 1].renderFlags |= (1 << 2);
				l += 6;
			}

			int cx = chunkAssignedVBO[x + 1][z][0];
			int cz = chunkAssignedVBO[x + 1][z][1];
			char otherChunkBlock = chunk[cx][cz].posIsBlock[0 + (j * sizeX) + (i * sizeX * sizeZ)];
			chunk[cx][cz].Update(0, j, i, true, ebo[cx][cz].id, vbo[cx][cz].ids[1], allVerticesX, otherChunkBlock, chunkAssignedVBO);

		}



		if (j > 0 && j < sizeX - 1)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].renderFlags |= (1 << 1);
				l += 6;
			}

			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].renderFlags |= (1 << 0);
				l += 6;
			}

		}
		else if (j == 0)
		{
			if (posIsBlock[chunkReference + sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + sizeX].renderFlags |= (1 << 1);
				l += 6;
			}

			int cx = chunkAssignedVBO[x][z - 1][0];
			int cz = chunkAssignedVBO[x][z - 1][1];
			char otherChunkBlock = chunk[cx][cz].posIsBlock[k + ((chunkSize - 1) * sizeX) + (i * sizeX * sizeZ)];
			chunk[cx][cz].Update(k, (chunkSize - 1), i, true, ebo[cx][cz].id, vbo[cx][cz].ids[1], allVerticesX, otherChunkBlock, chunkAssignedVBO);

		}

		else
		{
			if (posIsBlock[chunkReference - sizeX] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - sizeX].renderFlags |= (1 << 0);
				l += 6;
			}

			int cx = chunkAssignedVBO[x][z + 1][0];
			int cz = chunkAssignedVBO[x][z + 1][1];
			char otherChunkBlock = chunk[cx][cz].posIsBlock[k + (0 * sizeX) + (i * sizeX * sizeZ)];
			chunk[cx][cz].Update(k, 0, i, true, ebo[cx][cz].id, vbo[cx][cz].ids[1], allVerticesX, otherChunkBlock, chunkAssignedVBO);

		}



		if (i > 0 && i < sizeY - 1)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].renderFlags |= (1 << 5);
				l += 6;
			}

			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].renderFlags |= (1 << 4);
				l += 6;
			}

		}
		else if (i == 0)
		{
			if (posIsBlock[chunkReference + (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) + (sizeX * sizeZ)].renderFlags |= (1 << 5);
				l += 6;
			}
		}
		else
		{
			if (posIsBlock[chunkReference - (sizeX * sizeZ)] != air)
			{
				blocco[k + (j * sizeX) + (i * sizeX * sizeZ) - (sizeX * sizeZ)].renderFlags |= (1 << 4);
				l += 6;
			}
		}

		if (blocco[chunkReference].renderFlags & (1 << 0))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 0);
			l += 6;
		}

		if (blocco[chunkReference].renderFlags & (1 << 1))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 1);
			l += 6;
		}

		if (blocco[chunkReference].renderFlags & (1 << 2))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 2);
			l += 6;
		}

		if (blocco[chunkReference].renderFlags & (1 << 3))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 3);
			l += 6;
		}

		if (blocco[chunkReference].renderFlags & (1 << 4))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 4);
			l += 6;
		}

		if (blocco[chunkReference].renderFlags & (1 << 5))
		{
			blocco[chunkReference].renderFlags &= ~(1 << 5);
			l += 6;
		}

		indices0.resize(indices0.size() + 100);

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
					if (blocco[count].renderFlags & (1 << 0))
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

					if (blocco[count].renderFlags & (1 << 1))
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

					if (blocco[count].renderFlags & (1 << 2))
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

					if (blocco[count].renderFlags & (1 << 3))
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

					if (blocco[count].renderFlags & (1 << 4))
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


					if (blocco[count].renderFlags & (1 << 5))
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

		indices0.resize(faceCount * 6);

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
			verticesSize = (faceCount * 36);

			
		}

		int size1 = indices0.size();



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size1 * sizeof(GLuint), indices0.data(), GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW);		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);
		glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), allVertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		indexNumbers = indices0.size();
		indexNumber = indices0.size();
		//allVertices.resize(216 * chunkSize * chunkSize * sizeY / 10);
	}

	chunkBlocks[x][z] = posIsBlock;

}

void Chunk::highLight(int x, int z, int y, GLuint vbo)
{



	if (squareVertices[(x * 216) + (z * 216 * chunkSize) + (y * 216 * sizeX * sizeZ) + 8] != 0)
	{

		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (highLightedBlock[0] * 216) + (highLightedBlock[1] * 216 * chunkSize) + (highLightedBlock[2] * 216 * sizeX * sizeZ) + 8] = 289457;
		}


		int k = x; int j = z; int i = y;

		int count = 0;

		for (int h = 0; h < 216; h += 9)
		{
			squareVertices[h + (k * 216) + (j * 216 * chunkSize) + (i * 216 * sizeX * sizeZ) + 8] = count;
			count++;
			if (count == 4)
			{
				count = 0;
			}


		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 216 * chunkSize * chunkSize * sizeY * sizeof(float), squareVertices.data(), GL_STATIC_DRAW);
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
			squareVertices[h + (highLightedBlock[0] * 216) + (highLightedBlock[1] * 216 * chunkSize) + (highLightedBlock[2] * 216 * sizeX * sizeZ) + 8] = 289457;
		}

		highLightedBlock[3] = 0;

	}


}


void Chunk::Render(VAO vao, VBO vbo, EBO ebo)
{
	ebo.Bind();
	vao.LinkVBO(vbo, 3, 0, vbo.ids[1], 9, 0, GL_FLOAT);
	vao.LinkVBO(vbo, 3, 1, vbo.ids[1], 9, 3, GL_RGBA16F);
	vao.LinkVBO(vbo, 2, 2, vbo.ids[1], 9, 6, GL_RGBA16F);
	vao.LinkVBO(vbo, 1, 3, vbo.ids[1], 9, 8, GL_RGBA16F);
	glDrawElements(GL_TRIANGLES, indexNumbers, GL_UNSIGNED_INT, 0);
	ebo.Unbind();
}



