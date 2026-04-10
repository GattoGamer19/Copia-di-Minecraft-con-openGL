#pragma once
#include "vars.h"
#include <array>

class Player
{
private:
	bool run = true;

	float	currentChunk1 = 0;
	float	currentChunk2 = 0;
	float	chunkPosition1 = 0;
	float	chunkPosition2 = 0;

public:

	double position[3] = { 0, 0, 0 };
	bool lookingBlock = false;
	glm::dvec3 rayPoint = glm::dvec3(0, 0, 0);
	
	void set();
	void getLookingBlock(std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);
	void blockInteraction(std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);
};