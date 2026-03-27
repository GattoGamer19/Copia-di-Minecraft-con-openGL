#pragma once
#include <vector>
#include <math.h>
#include "Camera.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"
#include "chunkGen.h"
#include <unordered_map>
#include <time.h>
#include "Comandi.h"
#include <mutex>
#include <atomic>


inline std::mutex mutex;

inline 	int nThreads = 5;

inline float PI = 3.141592653589793;

inline bool collisionEnabled = true;

inline double fps = 0;
inline double frameTime = 0;

inline double titleUpdateTime = 0.5f;
inline double updateTime = 0;

inline int sizeY = 350;

inline int nChunkX = 15;
inline int rChunkX = 15;
inline int nChunkZ = 15;
inline int rChunkZ = 15;
inline float offset = ((float)nChunkX / 2);

inline float fallingSpeed = 0;

inline float altezza = 1080;
inline float larghezza = 1920;
inline float aSensZoom = 0.1;
inline int currentChunk[2] = { 0,0 };
inline float chunkPosition[3] = { 0,0,0 };
inline float prevSelectedBlock[3] = { 0,0,0 };
inline float prevSelectedChunk[2] = { 0,0 };
inline float yRotation = 0;
inline float yRotationMax = 0;
inline float xRotation = 0;
inline float xRotationMax = 0;
inline int asse = 0;
inline int placeBlockPosition[3] = { 0,0,0 };
inline int selectedBlockPosition[3] = { 0,0,0 };

inline std::vector<int> ChunkVBOX(nChunkX);
inline std::vector<int> ChunkVBOZ(nChunkX);


inline float pos0;
inline float pos1;
inline float pos2;

inline int invBlock = 2;
inline bool change = false;

inline camera cam;

inline glm::mat4 view1;

inline std::vector<std::vector<VBO>> vbo(nChunkX, std::vector<VBO>(nChunkZ));
inline std::vector<std::vector<EBO>> ebo(nChunkX, std::vector<EBO>(nChunkZ));
inline std::vector<std::vector<Chunk>> chunk(nChunkX, std::vector<Chunk>(nChunkZ));

inline std::vector<std::vector<bool>> isVisible(nChunkX, std::vector<bool>(nChunkZ));


inline std::unordered_map<int, std::unordered_map<int, std::atomic<bool> >> isCreated;
inline std::unordered_map<int, std::unordered_map<int, bool >> hasBeenModified;
inline std::unordered_map<int, std::unordered_map<int, std::vector<unsigned char>>> chunkBlocks;
inline std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>> bufferChunkAssignedVBO;
inline std::vector<bool> base(nChunkX, false);
inline std::vector<Chunk> baseChunk(1);
inline std::vector<Chunk> baseChunkArray(nChunkX);

inline clock_t destroyT = clock();
inline clock_t before = clock();
inline clock_t before2 = clock();
inline clock_t spaceBarT = clock();
inline clock_t jumpT = clock();
inline bool isJumping = false;

inline bool creativeMode = true;
inline int click = 0;

inline std::vector<std::vector<float>> allVerticesZ(nThreads, std::vector<float>(216 * chunkSize * chunkSize * sizeY));
inline std::vector<float> allVerticesX(216 * chunkSize * chunkSize * sizeY);

inline int refreshCountZ = 0;
inline int refreshCountX = 0;


inline int regChunkX = 0;
inline int regChunkX1 = 0;
inline int regChunkBX = 0;
inline int regChunkBX1 = 0;
inline int regChunkZ = 0;
inline int regChunkZ1 = 0;
inline int regChunkBZ = 0;
inline int regChunkBZ1 = 0;

inline float posX = 0;
inline float posY = 0;
inline float posZ = 0;

inline bool RotVisualEnabled = true;

inline bool genChunk = true;
inline bool realGenChunk = true;
