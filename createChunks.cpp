#include <vector>
#include "vars.h"
#include "createChunks.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include "Renderer.h"
#include <thread>
#include <atomic>

void chunkManager::set()
{
	mutex.lock();
	std::vector<std::vector<bool>> assignedVBO1(nChunkX, std::vector<bool>(nChunkX));
	std::vector<std::vector<doppio>> vboAssignedChunk1(nChunkX, std::vector<doppio>(nChunkX));

	allVerticesZ = std::vector<std::vector<float>>(nThreads, std::vector<float>(216 * chunkSize * chunkSize * sizeY));

	visibleChunksRot.resize(xRotInd * yRotInd * xVisibleInd * zVisibleInd);

	assignedVBO = assignedVBO1;
	vboAssignedChunk = vboAssignedChunk1;

	shouldCreate.resize(nThreads);
	shouldPreRender.resize(nThreads);
	finishedChunks.resize(nThreads);

	preRenderChunks.resize(nThreads);
	_createChunks.resize(nThreads);
	_createModifiedChunks.resize(nThreads);
	_createChunksPos.resize(nThreads);

	threads.resize(nThreads);


	for (int i = 0; i < nChunkX; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{
			assignedVBO.at(i).at(j) = false;
		}
	}


	for (int j = 0; j < nThreads; j++)
	{
		shouldCreate.at(j) = false;
		shouldPreRender.at(j) = false;
		finishedChunks.at(j) = false;

		threads.at(j) = std::thread(&chunkManager::chunkThreads, this, j);

	}
	mutex.unlock();
}

bool chunkManager::checkFreeVbo()
{
	bool stop = false;

	for (int i = 0; i < nChunkX && !stop; i++)
	{
		for (int j = 0; j < nChunkX && !stop; j++)
		{
			if (!assignedVBO[i][j])
			{


				return true;
				stop = true;




			}
		}
	}

	return false;
}

void chunkManager::findFirstFreeVBO(std::vector<std::vector<bool>>& assignedVBO, int& x, int& z)
{
	bool stop = false;

	for (int i = 0; i < nChunkX && !stop; i++)
	{
		for (int j = 0; j < nChunkX && !stop; j++)
		{
			if (!assignedVBO[i][j])
			{
				x = i;
				z = j;

				assignedVBO[i][j] = true;
				stop = true;

				//std::cout << "   " << "x: " << x << '\n' << "   " << "z: " << z << '\n';


			}
		}
	}

	if (!stop)
		x = -1;
}

void chunkManager::FreeOldChunks()
{
	if (genChunk) {

		currentChunk1[0] = (int)(player.position[0] + 0.5f) / chunkSize;
		currentChunk1[1] = (int)(player.position[1] + 0.5f) / chunkSize;

		if (player.position[0] < 0)
			currentChunk1[0]--;

		if (player.position[1] < 0)
			currentChunk1[1]--;

		int cx = offset;
		int cy = offset;
		int radius = (nChunkX - 1) / 2;

		bool stop = false;

		for (int d = radius; d >= 0 && !stop; --d) {
			for (int dx = -d; dx <= d && !stop; ++dx) {
				for (int dy = -d; dy <= d && !stop; ++dy) {


					int abs_dx = dx >= 0 ? dx : -dx;
					int abs_dy = dy >= 0 ? dy : -dy;
					int dist = (abs_dx > abs_dy) ? abs_dx : abs_dy;

					if (dist != d)
						continue;  // solo celle che "nascono" in questo anello

					int i = floor(cx + dx);
					int j = floor(cy + dy);


					if (abs(vboAssignedChunk[i][j].x - currentChunk1[0]) > offset || abs(vboAssignedChunk[i][j].z - currentChunk1[1]) > offset)
					{
						int assignedChunk[2] = { vboAssignedChunk[i][j].x, vboAssignedChunk[i][j].z };

						//mutex.lock();
						isCreated[assignedChunk[0]][assignedChunk[1]].store(false);
						//mutex.unlock();

						assignedVBO[i][j] = false;
						chunk[i][j].created = false;
						chunk[i][j] = {};
						vboAssignedChunk[i][j].x = 22321333;
						vboAssignedChunk[i][j].z = 22321333;

						chunkAssignedVBO[assignedChunk[0]].erase(assignedChunk[1]);
						chunkAssignedVBO[assignedChunk[0]].erase(assignedChunk[1]);

						

					}

				}
			}
		}
	}

}

void chunkManager::createForReal(int thread)
{

	int Cx = _createChunks[thread][0];
	int Cz = _createChunks[thread][1];

	int posX = _createChunksPos[thread][0];
	int posZ = _createChunksPos[thread][1];

	chunk[Cx][Cz].Create(posX, posZ, vbo[Cx][Cz].ids[1], ebo[Cx][Cz].id, allVerticesZ[thread], _createModifiedChunks[thread]);
	chunk[Cx][Cz].created = true;
	

}

void chunkManager::createChunks()
{
	while (true)
	{
		if (realGenChunk)
		{
			createingChunks = true;

			if (genChunk) {

				FreeOldChunks();

				countChunks = 0;



				currentChunk1[0] = (int)(player.position[0] + 0.5f) / chunkSize;
				currentChunk1[1] = (int)(player.position[1] + 0.5f) / chunkSize;

				if (player.position[0] < 0)
					currentChunk1[0]--;

				if (player.position[1] < 0)
					currentChunk1[1]--;

				int stop = 0;

				int cx = offset;
				int cy = offset;
				int radius = (nChunkX - 1) / 2;

				int startPoint1 = startPoint;

		

				for (int d = startPoint1; d <= radius && stop < nThreads; ++d) {
					for (int dx = -d; dx <= d && stop < nThreads; ++dx) {
						for (int dy = -d; dy <= d && stop < nThreads; ++dy) {


							int abs_dx = dx >= 0 ? dx : -dx;
							int abs_dy = dy >= 0 ? dy : -dy;
							int dist = (abs_dx > abs_dy) ? abs_dx : abs_dy;

							if (dist != d)
								continue;  // solo celle che "nascono" in questo anello

							

							int i = floor(cx + dx);
							int j = floor(cy + dy);

							if (checkFreeVbo())
							{
							
								//mutex.lock();
								bool _isCreated = isCreated[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j].load();
								//mutex.unlock();

								if (!_isCreated)
								{
									findFirstFreeVBO(assignedVBO, xVBO, zVBO);

									if (xVBO != -1)
									{
										if (hasBeenModified[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j])
										{
											_createModifiedChunks[stop] = true;
											chunk[xVBO][zVBO].posIsBlock = chunkBlocks[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j];
											//chunk[xVBO][zVBO].Create((currentChunk1[0] + (int)i) - offset, (currentChunk1[1] + (int)j) - offset, vbo[xVBO][zVBO].ids[1], ebo[xVBO][zVBO].id, allVerticesZ[stop], true);
											//chunk[xVBO][zVBO].preRender(vbo[xVBO][zVBO].ids[1], allVerticesZ[0], ebo[xVBO][zVBO].id);
										}

										else
										{
											_createModifiedChunks[stop] = false;
											//chunk[xVBO][zVBO].Create((currentChunk1[0] + (int)i) - offset, (currentChunk1[1] + (int)j) - offset, vbo[xVBO][zVBO].ids[1], ebo[xVBO][zVBO].id, allVerticesZ[stop], false);
											//chunk[xVBO][zVBO].preRender(vbo[xVBO][zVBO].ids[1], allVerticesZ[0], ebo[xVBO][zVBO].id);
										}

										genChunk = false;


										chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][0] = xVBO;
										chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][1] = zVBO;


										vboAssignedChunk[xVBO][zVBO].x = (currentChunk1[0] - offset) + (int)i;
										vboAssignedChunk[xVBO][zVBO].z = (currentChunk1[1] - offset) + (int)j;

										_createChunks[stop][0] = xVBO;
										_createChunks[stop][1] = zVBO;

										_createChunksPos[stop][0] = (currentChunk1[0] + (int)i) - offset;
										_createChunksPos[stop][1] = (currentChunk1[1] + (int)j) - offset;

										shouldCreate[stop] = true;

										countChunks++;

										//mutex.lock();
										isCreated[(currentChunk1[0] + (int)i) - offset][(currentChunk1[1] + (int)j) - offset].store(true);
										//mutex.unlock();

										stop++;
										
									}

									else
									{
										stop = nThreads + 1;
										startPoint = 0;
										std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
									}
								}

							}
							else
							{

								stop = nThreads + 1;
								startPoint = radius;
							}
						}
						
					}
					
					if(stop < nThreads)
					startPoint = d - 1;
				}

				if (stop == 0)
				{
					startPoint = 0;

				}
			}


			bool stop = false;
			bool genChunk1 = true;

			int i;

			for (i = 0; i < countChunks && !stop; i++)
			{
				if (finishedChunks[i])
					genChunk1 = true;
				else
				{
					genChunk1 = false;
					stop = true;
				}
			}

			

			if (genChunk1)
			{
				
				player = bufferPlayer;


			
				//std::cout << "mario" << '\n';
				//chunkAssignedVBO1 = chunkAssignedVBO;


				for (int i = 0; i < nThreads; i++)
				{
					finishedChunks[i] = false;
					updateChunkVBOCount++;

				}

				genChunk = genChunk1;
				
				createdFirstChunks = true;

			}
			
			
		

		}
		
		createingChunks = false;


	}

}

void chunkManager::outputVBOs()
{
	std::cout << "--------------VBOs--------------" << '\n';

	int x = chunkAssignedVBO[(currentChunk1[0])][(currentChunk1[1])][0];
	int z = chunkAssignedVBO[(currentChunk1[0])][(currentChunk1[1])][1];


	std::cout << "Chunk " << currentChunk1[0] << " " <<currentChunk1[1] << " assignedVBO: " << x << " " << z << '\n';
	std::cout << "VBO " << x << " " <<z << " assignedVBO: " << vboAssignedChunk[x][z].x << " " << vboAssignedChunk[x][z].z << '\n';
	std::cout << "isCreated " << isCreated[x][z] << '\n';

	/*
	for (int i = 0; i < nChunkX; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{
			int x = chunkAssignedVBO[i + (currentChunk1[0] - offset)][j + (currentChunk1[1] - offset)][0];
			int z = chunkAssignedVBO[i + (currentChunk1[0] - offset)][j + (currentChunk1[1] - offset)][1];


			std::cout << "Chunk " << i + (currentChunk1[0] - offset) << " " << j + (currentChunk1[1] - offset) << " assignedVBO: " << x << " " << z << '\n';

		}
	}

	for (int i = 0; i < nChunkX; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{


			int	x = vboAssignedChunk[i][j].x;
			int z = vboAssignedChunk[i][j].z;

			std::cout << "VBO " << i << " " << j << " assignedChunk: " << x << " " << z << '\n';

		}
	}*/

}

void chunkManager::getVisibleChunksRot(Player player)
{
	float Point[3] = { 0,0,0 };
	float Point1[3] = { 0,0,0 };
	float _Point[3] = { 0,0,0 };
	float _Point1[3] = { 0,0,0 };
	float offset8 = offset * 9;

	float cosXrot = cos((xRotation - 90) / 57.295779513);
	float sinXrot = sin((xRotation - 90) / 57.295779513);

	float step = 1.12f;

	for (float yRot = 0; yRot < 360; yRot += step)
	{
		float cosYrot = cos((yRot) / 57.295779513);
		float sinYrot = sin((yRot) / 57.295779513);

		for (float xRot = 0; xRot < 90; xRot += step)
		{
			float senXRot = sin((xRot) / 57.295779513);

			for (float i = -90 + yRot - 85; i < -90 + yRot + 85; i += step)
			{
				float sinI = sin(i / 57.295779513);
				float cosI = cos(i / 57.295779513);

				for (float l = 0; l <= (offset + 1) * chunkSize; l += 3)
				{

					Point[0] = ((cosI * l) + offset * chunkSize) - (cosYrot * chunkSize);
					Point[1] = ((sinI * l) + offset * chunkSize) - (sinYrot * chunkSize);


					int currentChunk1X = (int)(Point[0] + 0.5f) / chunkSize;
					int currentChunk1Z = (int)(Point[1] + 0.5f) / chunkSize;

					if (currentChunk1X >= 0 && currentChunk1Z >= 0 && currentChunk1X < (nChunkX - 1) && currentChunk1Z < (nChunkX - 1))
					{
					

						int a = (((int)floorf(yRot / 2)) * xRotInd * xVisibleInd * zVisibleInd);
						int b = ((abs((int)floorf(xRot / 2))) * xVisibleInd * zVisibleInd);
						int c = ((currentChunk1X)*zVisibleInd);
						int d = (currentChunk1Z);

						int index = a + b + c + d;
						if (index < 0)
							index = 0;
						if (index > (xRotInd * yRotInd * xVisibleInd * zVisibleInd) - 1)
							index = (xRotInd * yRotInd * xVisibleInd * zVisibleInd) - 1;

						visibleChunksRot[index] = true;
					}



				}
			}
		}

	}
}

void chunkManager::RenderChunks(VAO VAO1, Player player)
{
	int Cx;
	int Cz;

	int plusX = (player.position[2] / (40.0f / (16.0f / chunkSize))) * 3;
	int plusY = (player.position[2] / (40.0f / (16.0f / chunkSize))) * 3;

	rendering.store(true);


	if (xRotation > xRotationMax)
		xRotationMax = xRotation;


	if (yRotation > yRotationMax)
		yRotationMax = yRotation;

	if (GetAsyncKeyState('C'))
		std::cout << xRotationMax << "  " << yRotationMax << '\n';

	for (int j = 0; j < rChunkZ; j++)
	{
		for (int i = 0; i < rChunkX; i++)
		{
			int a = ((abs((int)floorf(yRotation / 2))) * xRotInd * xVisibleInd * zVisibleInd);
			int b = ((abs((int)floorf(xRotation / 2))) * xVisibleInd * zVisibleInd);
			int c = ((j)*zVisibleInd);
			int d = (i);

			int index = a + b + c + d;

			if (index < 0)
				index = 0;
			if (index > (xRotInd * yRotInd * xVisibleInd * zVisibleInd) - 1)
				index = (xRotInd * yRotInd * xVisibleInd * zVisibleInd) - 1;
			
			int chunkX = j + (int)(currentChunk1[0] - offset);
			int chunkZ = i + (int)(currentChunk1[1] - offset);

			//mutex.lock();
			bool _isCreated = isCreated[chunkX][chunkZ].load();
			//mutex.unlock();

			if (_isCreated)
			{

				if (visibleChunksRot[index])
				{

					Cx = bufferChunkAssignedVBO[chunkX][chunkZ][0];
					Cz = bufferChunkAssignedVBO[chunkX][chunkZ][1];


					if (chunk[Cx][Cz].created)
						RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], chunk[Cx][Cz].indexNumber);


				}

				else if ((xRotation > 41) && (j <= (offset + plusX) && j >= (offset - plusX) && i <= (offset + plusY) && i >= (offset - plusY)))
				{

					Cx = bufferChunkAssignedVBO[chunkX][chunkZ][0];
					Cz = bufferChunkAssignedVBO[chunkX][chunkZ][1];

					if (chunk[Cx][Cz].created)
						RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], chunk[Cx][Cz].indexNumber);
				}

				else if ((j >= offset - (1) && j <= offset + (1)) && (i >= offset - (1) && i <= offset + (1)))
				{

					Cx = bufferChunkAssignedVBO[chunkX][chunkZ][0];
					Cz = bufferChunkAssignedVBO[chunkX][chunkZ][1];
						RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], chunk[Cx][Cz].indexNumber);

				}
			}
		}
	}

	rendering.store(false);
}

void chunkManager::preRender()
{
	if (!genChunk)
	{
		if (updateChunkVBOCount >= 30)
		{
			for (auto& [keyOuter, innerMap] : chunkAssignedVBO) {
				auto& targetInner = bufferChunkAssignedVBO[keyOuter]; // crea o accede alla mappa interna
				for (auto& [keyInner, value] : innerMap) {
					targetInner[keyInner] = value; // copia valore
				}
			}


			for (int j = -rChunkZ; j < rChunkZ; j++)
			{
				for (int i = -rChunkZ; i < rChunkX; i++)
				{

					int chunkX = j + (int)(currentChunk1[0] - offset);
					int chunkZ = i + (int)(currentChunk1[1] - offset);

					if(!isCreated[chunkX][chunkZ].load())
					isCreated[chunkX][chunkZ] = false;
				}
			}

			updateChunkVBOCount = 0;
		}


		for (int i = 0; i < nThreads; i++)
		{
			if (shouldPreRender[i])
			{
				int Cx = _createChunks[i][0];
				int Cz = _createChunks[i][1];

				chunk[Cx][Cz].preRender(vbo[Cx][Cz].ids[1], allVerticesZ[i], ebo[Cx][Cz].id);

				shouldPreRender[i] = false;
				finishedChunks[i] = true;
			}

		}
	}



}

void chunkManager::chunkThreads(int thread)
{
	while (true)
	{
		if (!genChunk)
		{
			if (shouldCreate[thread])
			{
				createForReal(thread);
				shouldCreate[thread] = false;
				shouldPreRender[thread] = true;
			}
		}
	}
}