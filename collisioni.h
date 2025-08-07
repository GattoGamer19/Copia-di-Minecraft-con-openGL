#ifndef COLLISIONI_H
#define COLLISIONI_H


#include "chunkGen.h"
#include "Camera.h"
#include "vector"

class collisioni
{

public:

	std::vector<std::vector<Chunk>> chunk;

	bool IsBlockX = false;
	bool IsBlockY = false;
	bool IsBlockZ = false;

	bool posIsBlockX(int currentChunk[], float chunkPosition[], float position[]);
	bool posIsBlockY(int currentChunk[], float chunkPosition[], float position[]);
	bool posIsBlockZ(int currentChunk[], float chunkPosition[], float position[]);
	void MoveWx(camera cam, float position[], float chunkPosition[], int currentChunk[]);
	void MoveWz(camera cam, float position[], float chunkPosition[], int currentChunk[]);

};

#endif