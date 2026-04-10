#include "vars.h"
#include "collision.h"
#include "Player.h"
#include <array>
#include "UVcord.h"

bool onGround(Player& player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{
	if (collisionEnabled)
	{
		for (float i = player.position[0] - 0.25f; i < player.position[0] + 0.26f; i += 0.5f)
			for (float j = player.position[1] - 0.25f; j < player.position[1] + 0.26f; j += 0.5f)
			{
				currentChunk[0] = (int)(i + 0.5f) / chunkSize;
				currentChunk[1] = (int)(j + 0.5f) / chunkSize;

				if ((i + 0.5f) / chunkSize < 0)
					currentChunk[0]--;

				if ((j + 0.5f) / chunkSize < 0)
					currentChunk[1]--;


				chunkPosition[0] = i - currentChunk[0] * chunkSize;
				chunkPosition[1] = j - currentChunk[1] * chunkSize;
				chunkPosition[2] = player.position[2];


				mutex.lock();
				int cZ = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][1];
				int cX = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][0];
				mutex.unlock();

				if (chunk[cX][cZ].created)
				if (chunk[cX][cZ].posIsBlock[(int)((roundf(player.position[2] - 1.99f) * chunkSize * chunkSize) + (int)roundf(chunkPosition[0]) + ((int)roundf(chunkPosition[1]) * chunkSize))])
				{
					return true;
				}
			}

		return false;
	}

	else return false;
}


bool checkBlock(Player player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO, bool& onGround)
{
	if (collisionEnabled)
	{
		for (float i = player.position[0] - 0.25f; i < player.position[0] + 0.26f; i += 0.5f)
			for (float j = player.position[1] - 0.25f; j < player.position[1] + 0.26f; j += 0.5f)
			{
				currentChunk[0] = (int)(i + 0.5f) / chunkSize;
				currentChunk[1] = (int)(j + 0.5f) / chunkSize;

				if ((i + 0.5f) / chunkSize < 0)
					currentChunk[0]--;

				if ((j + 0.5f) / chunkSize < 0)
					currentChunk[1]--;


				chunkPosition[0] = i - currentChunk[0] * chunkSize;
				chunkPosition[1] = j - currentChunk[1] * chunkSize;
				chunkPosition[2] = player.position[2];



				int cZ = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][1];
				int cX = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][0];

				int indexDown = (int)((roundf(player.position[2] - 1.75f) * chunkSize * chunkSize) + (int)roundf(chunkPosition[0]) + ((int)roundf(chunkPosition[1]) * chunkSize));
				int indexFront = (int)((roundf(player.position[2] - 0.75f) * chunkSize * chunkSize) + (int)roundf(chunkPosition[0]) + ((int)roundf(chunkPosition[1]) * chunkSize));
				int indexUp = (int)((roundf(player.position[2] + 0.25f) * chunkSize * chunkSize) + (int)roundf(chunkPosition[0]) + ((int)roundf(chunkPosition[1]) * chunkSize));

				if(indexDown >= 0 && indexDown <= chunkSize * chunkSize * sizeY)
				if (chunk[cX][cZ].created)
					if (chunk[cX][cZ].posIsBlock[indexDown] > 0 && chunk[cX][cZ].posIsBlock[indexDown] != water)
					{
						onGround = true;
						return true;
					}

				if (indexFront >= 0 && indexFront <= chunkSize * chunkSize * sizeY)
				if (chunk[cX][cZ].created)
					if (chunk[cX][cZ].posIsBlock[indexFront] > 0 && chunk[cX][cZ].posIsBlock[indexFront] != water)
					{
						onGround = false;
						return true;
					}

				if (indexUp >= 0 && indexUp <= chunkSize * chunkSize * sizeY)
				if (chunk[cX][cZ].created)
					if (chunk[cX][cZ].posIsBlock[indexUp] > 0 && chunk[cX][cZ].posIsBlock[indexUp] != water)
					{
						onGround = false;
						return true;
					}

				

				
			}
	}
	onGround = false;
	return false;
		
}