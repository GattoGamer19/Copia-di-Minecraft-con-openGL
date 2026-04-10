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

	allVerticesZ = std::vector<std::vector<float>>(nThreads, std::vector<float>(216 * chunkSize * chunkSize * sizeY / 10));

	visibleChunksRot.resize(xRotInd * yRotInd * xVisibleInd * zVisibleInd);

	assignedVBO = assignedVBO1;
	vboAssignedChunk = vboAssignedChunk1;

	shouldCreate.resize(nThreads);
	shouldPreRender.resize(nThreads);
	finishedChunks.resize(nThreads);
	bufferFinishedChunks.resize(nThreads);
	threadRefreshCount.resize(nThreads);
	_createChunksOffset.resize(nThreads);
	_createChunksInd.resize(nThreads);

	preRenderChunks.resize(nThreads);
	_createChunks.resize(nThreads);
	_createModifiedChunks.resize(nThreads);
	_createChunksPos.resize(nThreads);

	threads.resize(nThreads);

	countChunks1 = 12039818;

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
		bufferFinishedChunks.at(j) = false;
		threadRefreshCount.at(j) = 0;
		_createChunksOffset.at(j) = 0;

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
				
				vbo[i][j].meshCount++;

				if(vbo[i][j].meshCount >= 8)
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
	mutexChunk.lock();
	bool work = genChunk;
	mutexChunk.unlock();

	if (work) {

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
							
							//mutex.unlock();

							
							vbo[i][j].meshCount--;

							if (vbo[i][j].meshCount <= 0 || isCreated[assignedChunk[0]][assignedChunk[1]] == 1)
							{
								vbo[i][j].verticesInizialized = false;

								assignedVBO[i][j] = false;

								vbo[i][j].meshCount = 0;
							}

							isCreated[assignedChunk[0]][assignedChunk[1]].store(0);

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

	int _Cx = _createChunksInd[thread][0];
	int _Cz = _createChunksInd[thread][1];


	int posX = _createChunksPos[thread][0];
	int posZ = _createChunksPos[thread][1];


	chunk[_Cx][_Cz].Create(posX, posZ, vbo[Cx][Cz], ebo[Cx][Cz], allVerticesZ[thread], _createModifiedChunks[thread], _createChunksOffset[thread]);
	chunk[_Cx][_Cz].created = true;
	
	_createChunksOffset[thread] = 0;

}

void chunkManager::createChunks()
{
	while (true)
	{
		if (realGenChunk)
		{
			createingChunks = true;

			mutexChunk.lock();
			bool work = genChunk;
			mutexChunk.unlock();

			if (work) {

				genThreadRefreshCount = 0;

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
						
								//mutex.lock();
								int _isCreated = isCreated[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j].load();
								//mutex.unlock();


								xVBO = chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][0];
								zVBO = chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][1];

								int cScale = chunk[i][j].scale;

								int hypScale = 1;

								if(d > lodDist)
								hypScale = 4;

								//int hypScale = (int)(2 * (1 * (1 + (d - lodDist) / 5)));



								if ((!_isCreated))
								{
									if (!_isCreated)
									{
										findFirstFreeVBO(assignedVBO, xVBO, zVBO);
									}
									


									_createChunksOffset[stop] = vbo[xVBO][zVBO].meshCount - 1;

									if (xVBO != -1)
									{
										if (hasBeenModified[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j])
										{
		
												if (d > lodDist)
												{
													chunk[i][j].scale = 4;
													chunk[i][j].size = 1;
													//chunk[xVBO][zVBO].scale = 2;
													

													_createChunksInd[stop][0] = i;
													_createChunksInd[stop][1] = j;

												}

												else
												{
													chunk[xVBO][zVBO].scale = 1;
													assignedVBO[xVBO][zVBO] = true;
													vbo[xVBO][zVBO].meshCount = 8;

													_createChunksInd[stop][0] = xVBO;
													_createChunksInd[stop][1] = zVBO;

												}

												_createModifiedChunks[stop] = true;
												chunk[xVBO][zVBO].posIsBlock = chunkBlocks[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j];

												
											
											//chunk[xVBO][zVBO].Create((currentChunk1[0] + (int)i) - offset, (currentChunk1[1] + (int)j) - offset, vbo[xVBO][zVBO].ids[1], ebo[xVBO][zVBO].id, allVerticesZ[stop], true);
											//chunk[xVBO][zVBO].preRender(vbo[xVBO][zVBO].ids[1], allVerticesZ[0], ebo[xVBO][zVBO].id);
										}

										else
										{

												if (d > lodDist)
												{
													chunk[i][j].scale = 4;
													chunk[i][j].size = 1;

													_createChunksInd[stop][0] = i;
													_createChunksInd[stop][1] = j;

													std::cout << "miao" << '\n';

												}

												else 
												{
													chunk[xVBO][zVBO].scale = 1;
													assignedVBO[xVBO][zVBO] = true;
													vbo[xVBO][zVBO].meshCount = 8;

													_createChunksInd[stop][0] = xVBO;
													_createChunksInd[stop][1] = zVBO;

													std::cout << "bau" << '\n';

												}

												_createModifiedChunks[stop] = false;


											
											//chunk[xVBO][zVBO].Create((currentChunk1[0] + (int)i) - offset, (currentChunk1[1] + (int)j) - offset, vbo[xVBO][zVBO].ids[1], ebo[xVBO][zVBO].id, allVerticesZ[stop], false);
											//chunk[xVBO][zVBO].preRender(vbo[xVBO][zVBO].ids[1], allVerticesZ[0], ebo[xVBO][zVBO].id);
										}

										int size = chunk[i][j].size;

										int Ox = i;
										int Oz = j;
										

											chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][0] = xVBO;
											chunkAssignedVBO[(currentChunk1[0] - offset) + (int)i][(currentChunk1[1] - offset) + (int)j][1] = zVBO;

											if (!_isCreated)
											{
												vboAssignedChunk[xVBO][zVBO].x = (currentChunk1[0] - offset) + (int)i;
												vboAssignedChunk[xVBO][zVBO].z = (currentChunk1[1] - offset) + (int)j;
											}
											_createChunks[stop][0] = xVBO;
											_createChunks[stop][1] = zVBO;

											if (!_isCreated)
											{
												_createChunksPos[stop][0] = (currentChunk1[0] + Ox) - offset;
												_createChunksPos[stop][1] = (currentChunk1[1] + Oz) - offset;
											}

											else
											{
												_createChunksPos[stop][0] = chunk[xVBO][zVBO].x;
												_createChunksPos[stop][1] = chunk[xVBO][zVBO].z;
											}

											shouldCreate[stop] = true;

											//mutex.lock();
											isCreated[(currentChunk1[0] + (int)i) - offset][(currentChunk1[1] + (int)j) - offset].store(1);

											if (chunk[i][j].scale > 1)
											{
												isCreated[(currentChunk1[0] + (int)i) - offset][(currentChunk1[1] + (int)j) - offset].store(2);
											}

											mutexChunk.lock();
											genChunk = false;
											mutexChunk.unlock();

											countChunks++;
											//mutex.unlock();

											stop++;
										
									}

									else
									{
										stop = nThreads + 1;
										startPoint = d - 1;
										//std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
									}
								}
						}
						
					}
					
					if(stop < nThreads)
					startPoint = d - 1;

					if ((rand() % 10) == 0)
						startPoint = 0;
				}

				if (stop == 0)
				{
					startPoint = 0;

				}



				mutexChunk.lock();
				countChunks1 = countChunks;
				genChunk = false;
				mutexChunk.unlock();
			}

			

			mutexChunk.lock();
			bool _genChunk1 = genChunk1;
			mutexChunk.unlock();

			if (_genChunk1)
			{

				player = bufferPlayer;

				//std::cout << "mario" << '\n';
				//chunkAssignedVBO1 = chunkAssignedVBO;


				for (int i = 0; i < nThreads; i++)
				{
					finishedChunks[i] = false;

					if(countChunks != 0)
					updateChunkVBOCount++;

				}

				mutexChunk.lock();
				genChunk = true;
				genChunk1 = false;
				mutexChunk.unlock();
				
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

	for (int i = 0; i < nChunkX * nChunkX; i++)
	{
		int lodLevel = (int)(2 * (1 * (1 + (i - lodDist) / 20)));
		std::cout << lodLevel << " " << i << '\n';
	}

	int count = 0;

	for (int i = 0; i < nChunkX; i++)
	{
		
		for (int j = 0; j < nChunkX; j++)
		{
			if (vbo[i][j].verticesInizialized)
				count++;
		}

		

	}

	std::cout << "initialized VBO: " << count << '\n';
	/*
	for (int i = 0; i < nChunkX; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{


			int	x = vboAssignedChunk[i][j].x;
			int z = vboAssignedChunk[i][j].z;

			std::cout << "VBO " << i << " " << j << " assignedChunk: " << x << " " << z << '\n';

		}
	}
	*/
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

	float step = 1.05f;

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

				for (float l = 0; l <= (offset + 1) * chunkSize; l += 3 * (chunkSize / 16))
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


	for (int j = 0; j < rChunkZ; j++)
	{
		for (int i = 0; i < rChunkX; i++)
		{
			vbo[i][j].rendered = false;
		}
	}

	int renderCount = 0;

	int a = ((abs((int)floorf(yRotation / 2))) * xRotInd * xVisibleInd * zVisibleInd);
	int b = ((abs((int)floorf(xRotation / 2))) * xVisibleInd * zVisibleInd);

	for (int j = 0; j < rChunkZ; j++)
	{
		for (int i = 0; i < rChunkX; i++)
		{
			
			int chunkX = j + (int)(currentChunk1[0] - offset);
			int chunkZ = i + (int)(currentChunk1[1] - offset);

			Cx = bufferChunkAssignedVBO[chunkX][chunkZ][0];
			Cz = bufferChunkAssignedVBO[chunkX][chunkZ][1];

			if (!vbo[Cx][Cz].rendered)
			{
				//mutex.lock();
				bool _isCreated = isCreated[chunkX][chunkZ].load();
				//mutex.unlock();


				int c = ((j)*zVisibleInd);
				int d = (i);

				int index = a + b + c + d;

				if (_isCreated)
				{

					if (visibleChunksRot[index])
					{

						bool render = vbo[Cx][Cz].verticesInizialized;


						if (render)
						{
							RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], ebo[Cx][Cz].indices[8].size());
							vbo[Cx][Cz].rendered = true;
							renderCount++;
						}



					}

					else if ((xRotation > 41) && (j <= (offset + plusX) && j >= (offset - plusX) && i <= (offset + plusY) && i >= (offset - plusY)))
					{

						bool render = vbo[Cx][Cz].verticesInizialized;


						if (render)
						{
							RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], ebo[Cx][Cz].indices[8].size());
							vbo[Cx][Cz].rendered = true;
							renderCount++;
						}

					}

					else if ((j >= offset - (1) && j <= offset + (1)) && (i >= offset - (1) && i <= offset + (1)))
					{


						bool render = vbo[Cx][Cz].verticesInizialized;


						if (render)
						{
							RenderObjects(VAO1, vbo[Cx][Cz], 1, ebo[Cx][Cz], ebo[Cx][Cz].indices[8].size());
							vbo[Cx][Cz].rendered = true;
							renderCount++;
						}


					}
				}
			}
		}
	}

	if (GetAsyncKeyState('G'))
		std::cout << renderCount << '\n';

	rendering.store(false);
}

void chunkManager::preRender()
{
	mutexChunk.lock();
	bool work = !genChunk;
	mutexChunk.unlock();

	if (work)
	{
		if (updateChunkVBOCount >= updateChunkVBOThreeshold)
		{
			for (auto& [keyOuter, innerMap] : chunkAssignedVBO) {
				auto& targetInner = bufferChunkAssignedVBO[keyOuter]; // crea o accede alla mappa interna
				for (auto& [keyInner, value] : innerMap) {
					targetInner[keyInner] = value; // copia valore
				}
			}


			for (int j = -rChunkZ * 1.2f; j < rChunkZ * 1.2f; j++)
			{
				for (int i = -rChunkZ * 1.2f; i < rChunkX * 1.2f; i++)
				{

					int chunkX = j + (int)(currentChunk1[0]);
					int chunkZ = i + (int)(currentChunk1[1]);

					if(!isCreated[chunkX][chunkZ].load())
					isCreated[chunkX][chunkZ].store(0);
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

				mutexVBO.lock();

				vbo[Cx][Cz].mergeMeshes();
				ebo[Cx][Cz].calcIndices(vbo[Cx][Cz].allVertices[8].size());

				mutexVBO.unlock();

				chunk[Cx][Cz].preRender(vbo[Cx][Cz].ids[1], vbo[Cx][Cz].allVertices[8], ebo[Cx][Cz], vbo[Cx][Cz].allVertices[8].size() * sizeof(float));

				mutexChunk.lock();
				shouldPreRender[i] = false;
				finishedChunks[i] = true;
				finishedCount++;
				mutexChunk.unlock();
			}

			mutexChunk.lock();

			if (finishedCount == countChunks1)
			{
				
				genChunk1 = true;

				finishedCount = 0;

				countChunks1 = 12039818;

				mutexChunk.unlock();

				break;
			}

			mutexChunk.unlock();

		}
		
	}

}

void chunkManager::chunkThreads(int thread)
{
	while (true)
	{
		mutexChunk.lock();
		bool work = !genChunk;
		mutexChunk.unlock();

		if (work)
		{
			if (shouldCreate[thread])
			{

				createForReal(thread);

				mutexChunk.lock();
				shouldCreate[thread] = false;
				shouldPreRender[thread] = true;
				mutexChunk.unlock();
			}
			threadRefreshCount[thread] = 0;
		}
	}
}