#include "Comandi.h"

#include <iostream>
#include <Windows.h>
#include <math.h>
#include <glad/glad.h>


void Comandi::zoom(GLfloat allVertices[], char keyButton, float sens)
{
	if (GetAsyncKeyState(keyButton) & 0x8000)
	{
		GLfloat Bx;
		GLfloat By;
		GLfloat Bz;

		for (int i = 0; i < 900; i += 8)
		{
			Bx = allVertices[i];
			By = allVertices[i + 1];
			Bz = allVertices[i + 2];
			allVertices[i] *= sens;
			allVertices[i + 1] *= sens;
			allVertices[i + 2] *= sens;

			Bx = allVertices[i] - Bx;
			By = allVertices[i + 1] - By;
			Bz = allVertices[i + 2] - Bz;

		}

		for (int i = 0; i < 900; i += 8)
		{
			allVertices[i] = allVertices[i] - Bx;
			allVertices[i + 1] = allVertices[i + 1] - By;
			allVertices[i + 2] = allVertices[i + 2] - Bz;
		}
	}
}

void Comandi::muoviX(GLfloat TriangleVertices[], char keyButton, float sens)
{
	if (GetAsyncKeyState(keyButton) & 0x8000)
	{
		for (int i = 0; i < 127; i += 8)
		{
			TriangleVertices[i] += sens;

			
		}

	}
}

void Comandi::muoviZ(GLfloat TriangleVertices[], char keyButton, float sens)
{
	if (GetAsyncKeyState(keyButton) & 0x8000)
	{
		for (int i = 2; i < 127; i += 8)
		{
			TriangleVertices[i] += sens;

		}

	}
}

	void Comandi::Ruota(GLfloat vertices[], char keyButton, float sens, int asseRotazione)
	{
		
		if (GetAsyncKeyState(keyButton) & 0x8000)
		{

		
			for (int i = 0; i < 999; i += 8)
			{
				GLfloat x = vertices[i];
				GLfloat y = vertices[i + 1];
				GLfloat z = vertices[i + 2];

				switch (asseRotazione)
				{
				case 0:
					y += ((((cos(sens) * y) + (-sin(sens) * z))) - y) + (((cos(sens) * y) + (-sin(sens) * z)));
					z += ((((sin(sens) * y) + (cos(sens) * z))) - z) + (((sin(sens) * y) + (cos(sens) * z)));
					break;

				case 1:
					x = ((cos(sens) * x) + (sin(sens) * z));
					z = ((-sin(sens) * x) + (cos(sens) * z));
					break;

				case 2:
					x = ((cos(sens) * x) + (-sin(sens) * y));
					y = ((sin(sens) * x) + (cos(sens) * y));
					break;

				default:
					break;

				}

				vertices[i] = x;
				vertices[i + 1] = y;
				vertices[i + 2] = z;

			}
		}

	}