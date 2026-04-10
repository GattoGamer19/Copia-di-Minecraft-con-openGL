#pragma once

#include <algorithm>
#include <array>
#include <thread>
#include <atomic>

class doppio
{
public:
	int x = 0;
	int z = 0;
};

class chunkManager
{
private:

	std::atomic<bool> rendering = false;
	bool genChunk1 = false;
	std::atomic<bool> createingChunks = false;

	int finishedCount = 0;

	std::vector<std::thread> threads;

	int currentChunk1[2] = { 0, 0 };

	int xVBO = 0;
	int zVBO = 0;

	int yRotInd = 180;
	int xRotInd = 45;
	int xVisibleInd = nChunkX + 1;
	int zVisibleInd = nChunkX + 1;

	int lodDist = 1110.5 * (128 / chunkSize);

	std::vector<bool> visibleChunksRot;

	std::vector<std::array<int,2>> preRenderChunks;
	std::vector<std::array<int, 2>> _createChunks;
	std::vector<bool> _createModifiedChunks;
	std::vector<std::array<int, 2>> _createChunksPos;
	std::vector<std::array<int, 2>> _createChunksInd;
	std::vector<int> _createChunksOffset;

	int countThread = 0;
	int countChunks = 0;
	int countChunks1 = 0;

	int startPoint = 0;
	std::vector<int> threadRefreshCount;
	int threadsRefreshRate = 10000;
	int genThreadRefreshCount = 0;

public:
	
	Player player;
	Player bufferPlayer;

	std::vector<bool> shouldPreRender;
	std::vector<bool> shouldCreate;
	std::vector<bool> finishedChunks;
	std::vector<bool> bufferFinishedChunks;

	std::unordered_map<int, std::unordered_map<int, std::array<int,2>>> chunkAssignedVBO;
	
	std::vector<std::vector<bool>> assignedVBO;
	std::vector<std::vector<doppio>> vboAssignedChunk;

	bool createdFirstChunks = false;
	std::atomic<int> updateChunkVBOCount = 30;
	std::atomic<int> updateChunkVBOThreeshold = 30;

	void set();
	void createChunks();
	void FreeOldChunks();
	bool checkFreeVbo();
	void findFirstFreeVBO(std::vector<std::vector<bool>>& assignedVBO, int& x, int& z);
	void takeVBOslot(std::vector<std::vector<bool>>& assignedVBO, int x, int z);
	void getVisibleChunksRot(Player player);
	void RenderChunks(VAO VAO1, Player player);
	void outputVBOs();
	void createForReal(int thread);
	void preRender();
	void chunkThreads(int thread);


};

