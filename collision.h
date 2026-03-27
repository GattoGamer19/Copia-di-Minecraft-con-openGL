#pragma once
#include "vars.h"
#include "Player.h"
#include <array>

bool onGround(Player& player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);
bool checkBlock(Player player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO, bool& onGround);