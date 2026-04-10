#ifndef COMANDI_H
#define COMANDI_H

#include <iostream>
#include <Windows.h>
#include <math.h>
#include <glad/glad.h>
#include "Player.h"
#include <array>
#include "collision.h"

class Inventory;

class Comandi
{

private:
	
	POINT P = {};
	POINT P1 = {};
	bool firstTime = true;
	float changeMouseX = 0;
	float changeMouseY = 0;

	float proj = 60;
	float projMin = 60;
	float projMax = 65;

	bool _onGround = false;

	void MoveDir(int asse, float sens, Player& player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);

public:
	
	void Move(Player& player, float speed, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO);
	void GetInvBlock(Inventory& inv);
	void checkRotEnabled();
	void getMouseMovement(float& x, float& y);
	void Rotate(float x, float y, float sensX, float sensY);

};

#endif