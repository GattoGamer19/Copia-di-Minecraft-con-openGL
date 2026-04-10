#include "vars.h"
#include "Player.h"
#include "createChunks.h"

void Player::set()
{
	position[0] = 0;
	position[1] = 0;
	position[2] = 150;

	currentChunk[0] = (int)(position[0] + 0.5f) / chunkSize;
	currentChunk[1] = (int)(position[1] + 0.5f) / chunkSize;

	pos0 = position[0];
	pos1 = position[1];
	pos2 = position[2];

}

void Player::getLookingBlock(std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{
	rayPoint = glm::dvec3(position[0], position[2], position[1]);

	int count = 0;

	while (run && collisionEnabled)
	{
		rayPoint += (cam.forwardRay * 0.05f);

		currentChunk[0] = (int)(rayPoint.x + 0.5f) / chunkSize;
		currentChunk[1] = (int)(rayPoint.z + 0.5f) / chunkSize;
	   
		if ((rayPoint.x + 0.5f) / chunkSize < 0)
			currentChunk[0]--;

		if ((rayPoint.z + 0.5f) / chunkSize < 0)
			currentChunk[1]--;

		chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * chunkSize);
		chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * chunkSize);
		

		int cx = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][0];
		int cz = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][1];


		count++;
		
		int index = (int)((roundf(rayPoint.y) * chunkSize * chunkSize) + (chunkPosition[0]) + (chunkPosition[1] * chunkSize));

		if(chunk[cx][cz].created)
		if(index >= 0 && index < chunkSize * chunkSize * sizeY)
		if (chunk[cx][cz].posIsBlock[index])
		{
			rayPoint -= (cam.forwardRay * 0.05f);
			while (run)
			{


				rayPoint += (cam.forwardRay * 0.005f);
				currentChunk[0] = (int)(rayPoint.x + 0.5f) / chunkSize;
				currentChunk[1] = (int)(rayPoint.z + 0.5f) / chunkSize;

				if ((rayPoint.x + 0.5f) / chunkSize < 0)
					currentChunk[0]--;

				if ((rayPoint.z + 0.5f) / chunkSize < 0)
					currentChunk[1]--;

				chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * chunkSize);
				chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * chunkSize);


				cx = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][0];
				cz = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][1];

				index = (int)((roundf(rayPoint.y) * chunkSize * chunkSize) + (chunkPosition[0]) + (chunkPosition[1] * chunkSize));

				if (chunk[cx][cz].created)
				if (index >= 0 && index < chunkSize * chunkSize * sizeY)
				if (chunk[cx][cz].posIsBlock[(int)((roundf(rayPoint.y) * chunkSize * chunkSize) + (chunkPosition[0]) + (chunkPosition[1] * chunkSize))])
				{
					rayPoint -= (cam.forwardRay * 0.005f);
					while (run)
					{
						rayPoint += (cam.forwardRay * 0.0005f);
						currentChunk[0] = (int)(rayPoint.x + 0.5f) / chunkSize;
						currentChunk[1] = (int)(rayPoint.z + 0.5f) / chunkSize;

						if ((rayPoint.x + 0.5f) / chunkSize < 0)
							currentChunk[0]--;

						if ((rayPoint.z + 0.5f) / chunkSize < 0)
							currentChunk[1]--;

						chunkPosition[0] = roundf(rayPoint.x - currentChunk[0] * chunkSize);
						chunkPosition[1] = roundf(rayPoint.z - currentChunk[1] * chunkSize);


						cx = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][0];
						cz = chunkAssignedVBO[currentChunk[0]][currentChunk[1]][1];

						index = (int)((roundf(rayPoint.y) * chunkSize * chunkSize) + (chunkPosition[0]) + (chunkPosition[1] * chunkSize));

						if (chunk[cx][cz].created)
						if (index >= 0 && index < chunkSize * chunkSize * sizeY)
						if (chunk[cx][cz].posIsBlock[(int)((roundf(rayPoint.y) * chunkSize * chunkSize) + (chunkPosition[0]) + (chunkPosition[1] * chunkSize))])
						{
							selectedBlockPosition[0] = chunkPosition[0];
							selectedBlockPosition[1] = chunkPosition[1];
							selectedBlockPosition[2] = roundf(rayPoint.y);
							placeBlockPosition[0] = selectedBlockPosition[0];
							placeBlockPosition[1] = selectedBlockPosition[1];
							placeBlockPosition[2] = selectedBlockPosition[2];

							rayPoint -= cam.forwardRay * 0.0005f;

							currentChunk1 = (int)(rayPoint.x + 0.5f) / chunkSize;
							currentChunk2 = (int)(rayPoint.z + 0.5f) / chunkSize;

							if ((rayPoint.x + 0.5f) / chunkSize < 0)
								currentChunk1--;

							if ((rayPoint.z + 0.5f) / chunkSize < 0)
								currentChunk2--;


							chunkPosition1 = roundf(rayPoint.x - currentChunk1 * chunkSize);
							chunkPosition2 = roundf(rayPoint.z - currentChunk2 * chunkSize);

							

							//std::cout << cx << "-" << currentChunk1 << "   " << cz << "-" << currentChunk2 << '\n';

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
									selectedBlockPosition[0] = (chunkSize - 1);
									placeBlockPosition[0] = 0;
								}

								else
								{
									selectedBlockPosition[0] = 0;
									placeBlockPosition[0] = (chunkSize - 1);
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
									selectedBlockPosition[1] = (chunkSize - 1);
									placeBlockPosition[1] = 0;
								}

								else
								{
									selectedBlockPosition[1] = 0;
									placeBlockPosition[1] = (chunkSize - 1);
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

		if (count > 1275)
		{
			selectedBlockPosition[0] = 0;
			selectedBlockPosition[1] = 0;
			selectedBlockPosition[2] = 0;
			lookingBlock = false;
			run = false;
		}


	}


	run = true;
}

void Player::blockInteraction(std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{
	if (GetAsyncKeyState(VK_RBUTTON) & 1 && lookingBlock)
	{
		int cX0 = currentChunk1;
		int cZ0 = currentChunk2;


		int cX = chunkAssignedVBO[cX0][cZ0][0];
		int cZ = chunkAssignedVBO[cX0][cZ0][1];


		//std::cout << "blockPos: " << currentChunk1 << "  " << selectedBlockPosition[2] << "   " << currentChunk2 << '\n';
		chunk[cX][cZ].Update(placeBlockPosition[0], placeBlockPosition[1], placeBlockPosition[2], true, ebo[cX][cZ].id, vbo[cX][cZ].ids[1], allVerticesX, invBlock, chunkAssignedVBO);
		PlaySound(TEXT("stone.wav"), NULL, SND_FILENAME | SND_ASYNC);

		
		

	}

	if (GetAsyncKeyState(VK_LBUTTON) && lookingBlock)
	{

		int cX0 = currentChunk[0];
		int cZ0 = currentChunk[1];


		int cX = chunkAssignedVBO[cX0][cZ0][0];
		int cZ = chunkAssignedVBO[cX0][cZ0][1];

		//std::cout << "blockPos: " << selectedBlockPosition[0] << "  " << selectedBlockPosition[2] << "   " << selectedBlockPosition[1] << '\n';

		chunk[cX][cZ].Update(selectedBlockPosition[0], selectedBlockPosition[1], selectedBlockPosition[2], false, ebo[cX][cZ].id, vbo[cX][cZ].ids[1], allVerticesX, 0, chunkAssignedVBO);

		posX = 2345235;
		posZ = 245234;
		posY = -1000000;
		
		PlaySound(TEXT("stone.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}

	if (lookingBlock)
	{
		rayPoint += (cam.forwardRay * 0.005f);
		posX = round(rayPoint.x);
		posZ = round(rayPoint.z);
		posY = round(rayPoint.y);

	}
	else
	{
		posX = 2345235;
		posZ = 245234;
		posY = -1000000;
	}

}
