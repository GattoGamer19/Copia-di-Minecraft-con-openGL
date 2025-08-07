#include "collisioni.h"
#include "Camera.h"
#include "chunkGen.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


bool collisioni::posIsBlockZ(int currentChunk[], float chunkPosition[], float position[])
{

	bool is =  false;

	if (chunkPosition[1] < 15.25f && chunkPosition[1] > -0.25f)
	{

		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
		{
			is =  true;

		}


		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
		{
			is =  true;
		}

		else if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
		{
			is =  true;

		}

		else if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
		{
			is =  true;;

		}

	}

	else
	{

		if (chunkPosition[1] < 0.75f)
		{

			if (chunk[currentChunk[1] - 1][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(15) * 16))])
			{
				is =  true;

			}
		}

		if (chunkPosition[1] < 0.75f)
		{

			if (chunk[currentChunk[1] - 1][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(15) * 16))])
			{
				is =  true;

			}
		}

		if (chunkPosition[1] > 15.25f)
		{
			if (chunk[currentChunk[1] + 1][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(0) * 16))])
			{
				is =  true;

			}
		}

		if (chunkPosition[1] > 15.25f)
		{
			if (chunk[currentChunk[1] + 1][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(0) * 16))])
			{
				is =  true;

			}
		}

	}


	IsBlockZ = is;
	return is;

}

bool collisioni::posIsBlockX(int currentChunk[], float chunkPosition[], float position[])
{

	bool is = false;

	if (chunkPosition[0] < 15.25f && chunkPosition[0] > -0.25f)
	{

		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
		{
			is = true;

		}


		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] + 0.25f) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
		{
			is = true;

		}

		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
		{
			is = true;

		}

		if (chunk[currentChunk[1]][currentChunk[0]].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(chunkPosition[0] - 0.25f) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
		{
			is = true;

		}

	}

	else
	{

		if (chunkPosition[0] < 0.75f)
		{

			if (chunk[currentChunk[1]][currentChunk[0] - 1].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(15) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
			{
				is = true;

			}
		}

		if (chunkPosition[0] < 0.75f)
		{

			if (chunk[currentChunk[1]][currentChunk[0] - 1].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(15) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
			{
				is = true;

			}
		}

		if (chunkPosition[0] > 15.25f)
		{
			if (chunk[currentChunk[1]][currentChunk[0] + 1].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(0) + ((int)roundf(chunkPosition[1] + 0.25f) * 16))])
			{
				is = true;

			}
		}

		if (chunkPosition[0] > 15.25f)
		{
			if (chunk[currentChunk[1]][currentChunk[0] + 1].posIsBlock[(int)((roundf(position[2]) * 16 * 16) + (int)roundf(0) + ((int)roundf(chunkPosition[1] - 0.25f) * 16))])
			{
				is = true;

			}
		}

	}

	IsBlockX = is;
	return is;

}

void collisioni::MoveWx(camera cam, float position[], float chunkPosition[], int currentChunk[])
{

	glm::mat4 view1 = cam.view;
	float pos0 = position[0];
	float pos1 = position[1];

	cam.move(1, 0.002f, position);


	currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
	currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
	chunkPosition[0] = position[0] - currentChunk[0] * 16;
	chunkPosition[1] = position[1] - currentChunk[1] * 16;

	if (IsBlockX)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}

	if (IsBlockZ)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}

	if (IsBlockY)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}

}

void collisioni::MoveWz(camera cam, float position[], float chunkPosition[], int currentChunk[])
{

	glm::mat4 view1 = cam.view;
	float pos0 = position[0];
	float pos1 = position[1];

	cam.move(12, -0.0002f, position);


	currentChunk[0] = ((int)floorf(position[0] + 0.5f) - ((int)floorf(position[0] + 0.5f) % 16)) / 16;
	currentChunk[1] = ((int)floorf(position[1] + 0.5f) - ((int)floorf(position[1] + 0.5f) % 16)) / 16;
	chunkPosition[0] = position[0] - currentChunk[0] * 16;
	chunkPosition[1] = position[1] - currentChunk[1] * 16;

	if (IsBlockX)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}

	if (IsBlockZ)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}

	if (IsBlockY)
	{
		cam.view = view1;
		position[1] = pos1;
		position[0] = pos0;
	}
}