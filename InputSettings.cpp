#include <iostream>
#include "vars.h"
#include "InputSettings.h"
#include "Player.h"

void InputSettings(Player& player) {
	std::cout << "Render Distance: " << std::endl;
	std::cin >> nChunkX;



	rChunkX = nChunkX;
	nChunkZ = nChunkX;
	rChunkZ = nChunkX;

	offset = floor((float)nChunkX / 2);



	pos0 = player.position[0];
	pos1 = player.position[1];
	pos2 = player.position[2];

	ChunkVBOX.resize(nChunkX);
	ChunkVBOZ.resize(nChunkX);

	ebo.resize(nChunkX);
	vbo.resize(nChunkX);
	chunk.resize(nChunkX);

	isVisible.resize(nChunkX);

	for (int i = 0; i < nChunkX; i++)
	{
		ebo[i].resize(nChunkX);
		vbo[i].resize(nChunkX);
		chunk[i].resize(nChunkX);

		isVisible[i].resize(nChunkX);
	}

	GLuint vuoto[2] = { 0,0 };
	GLfloat vuoto1[2] = { 0,0 };

	for (int i = 0; i < nChunkX; i++)
	{
		for (int j = 0; j < nChunkX; j++)
		{
			chunk[i][j] = {};
			ebo[i][j] = {};
			vbo[i][j] = {};

		}
	}

	base.resize(nChunkX);

	baseChunkArray.resize(nChunkX);

	for (int j = 0; j < nChunkX; j++)
	{
		baseChunkArray[j] = {};
	}

	std::cout << "Enter '1' for collision enabled, will also disable infinite world generation and may also crash going negativly on x and z axys" << std::endl;
	std::cout << "Enter '0' to not enable" << std::endl;
	std::cin >> collisionEnabled;

	std::cout << "Enter how many threads you want for chunk generation" << std::endl;
	std::cin >> nThreads;
}