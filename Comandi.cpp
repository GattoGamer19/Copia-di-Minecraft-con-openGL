#include "Comandi.h"
#include "vars.h"
#include <iostream>
#include <Windows.h>
#include <math.h>
#include <glad/glad.h>
#include "collision.h"
#include "Player.h"


void Comandi::MoveDir(int asse, float sens, Player& player, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{

	pos0 = player.position[0];
	pos1 = player.position[1];
	pos2 = player.position[2];

	view1 = cam.view;


	cam.move(asse, sens / 1000, player.position);


	currentChunk[0] = (int)((player.position[0] - 10000000) + 0.5f) / chunkSize;
	currentChunk[1] = (int)((player.position[1] - 10000000) + 0.5f) / chunkSize;

	if (player.position[0] < 0)
		currentChunk[0]--;

	if (player.position[1] < 0)
		currentChunk[1]--;

	chunkPosition[0] = player.position[0] - currentChunk[0] * chunkSize + 0.5f;
	chunkPosition[1] = player.position[1] - currentChunk[1] * chunkSize + 0.5f;
	chunkPosition[2] = player.position[2];

	if (checkBlock(player, chunkAssignedVBO, _onGround))
	{
		cam.view = view1;
		player.position[0] = pos0;
		player.position[1] = pos1;
		player.position[2] = pos2;
	}



}

void Comandi::Move(Player& player, float speed, std::unordered_map<int, std::unordered_map<int, std::array<int, 2>>>& chunkAssignedVBO)
{
	
	cam.proj(proj, 0.001f, larghezza, altezza, 10000.0f);

	if (GetAsyncKeyState('S') & 0x8000)
	{
		cam.forward1.x = -cam.forward.x;
		cam.forward1.z = -cam.forward.z;


	}

	if (!(GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState('S') & 0x8000))
	{
		cam.forward1.x = 0;
		cam.forward1.z = 0;
		cam.forward1.y = 0;
	}

	if ((GetAsyncKeyState('W') & 0x8000) && !(GetAsyncKeyState('S') & 0x8000))
	{
		cam.forward1.x = cam.forward.x;
		cam.forward1.z = cam.forward.z;

	}

	if ((GetAsyncKeyState('R') & 0x8000) && (GetAsyncKeyState(VK_LCONTROL) & 0x8000))
	{
		cam.forward1.x = cam.forward.x + cam.forward.x * 20.2f;
		cam.forward1.z = cam.forward.z + cam.forward.z * 20.2f;
		//cam.forward1.y = cam.forward.y + cam.forward.y * 20.2f;


		if (proj < projMax)
			proj += 0.03f * frameTime;
	}

	else if ((GetAsyncKeyState('W') & 0x8000) && (GetAsyncKeyState(VK_LCONTROL) & 0x8000))
	{
		cam.forward1.x = cam.forward.x + cam.forward.x * 1.2f;
		cam.forward1.z = cam.forward.z + cam.forward.z * 1.2f;

		if (proj < projMax)
			proj += 0.03f * frameTime;
	}
	
	else
	{
		if (proj > projMin)
			proj -= 0.03f * frameTime;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		cam.forward1.x -= cam.right.x;
		cam.forward1.y -= cam.right.y;
		cam.forward1.z -= cam.right.z;

	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		cam.forward1.x += cam.right.x;
		cam.forward1.y += cam.right.y;
		cam.forward1.z += cam.right.z;

	}

	if (_onGround)
	{
		fallingSpeed = 0;
	}

	if ((GetAsyncKeyState(VK_SPACE) & 1))
	{

		if (_onGround)
		{
			jumpT = clock();
			isJumping = true;
			fallingSpeed = 0;
		}

		if (click == 1 && clock() - spaceBarT > 300)
		{
			click = 0;
		}

		if (click == 0)
		{
			spaceBarT = clock();
			click = 1;
		}

		else
		{
			spaceBarT = clock() - spaceBarT;
			click = 0;
		}

		if (spaceBarT < 170 && spaceBarT > 100)
		{

			if (!creativeMode)
			{
				creativeMode = true;
			}

			else
			{
				creativeMode = false;
			}
		}

	}

	if (isJumping)
	{
		cam.forward1.y = 4.0f - sqrt((float)(clock() - jumpT) / 15);
		if (3.25f - sqrt((float)(clock() - jumpT) / 15) < 0)
		{
			isJumping = false;
		}
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && creativeMode)
	{
		cam.forward1.y = 1.0f;
	}

	if (!creativeMode && !isJumping && !_onGround)
	{
		if (fallingSpeed > -50)
		{
			fallingSpeed -= 0.02f * (clock() - before);
			cam.forward1.y = fallingSpeed;
		}

	}

	else
	{
		if(!creativeMode && !isJumping)
		{
			cam.forward1.y = -1.0f;
		}

		else if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && !isJumping)
		{
			cam.forward1.y = -1.0f;
		}
		else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000) && !isJumping)
		{
			cam.forward1.y = 0;
		}
	}





if (clock() - before > 1)
{
	MoveDir(1, speed * (clock() - before), player, chunkAssignedVBO);
	MoveDir(12, speed * (clock() - before), player, chunkAssignedVBO);
	MoveDir(13, speed * 10 * (clock() - before), player, chunkAssignedVBO);

	before = clock();


 }


}

void Comandi::GetInvBlock()
{
	if (GetAsyncKeyState('1') & 0x8000)
	{
		invBlock = 1;
		change = true;
	}

	if (GetAsyncKeyState('2') & 0x8000)
	{
		invBlock = 2;
		change = true;
	}

	if (GetAsyncKeyState('3') & 0x8000)
	{
		invBlock = 3;
		change = true;
	}

	if (GetAsyncKeyState('4') & 0x8000)
	{
		invBlock = 4;
		change = true;
	}
}

void Comandi::checkRotEnabled()
{

	if (GetAsyncKeyState('N') & 0x8000)
	{
		RotVisualEnabled = false;
	}

	else if (GetAsyncKeyState('S') & 0x8000)
	{
		RotVisualEnabled = true;
	}


}

void Comandi::getMouseMovement(float& x, float&  y)
{

	float changeMouseY1 = changeMouseY;

	if (RotVisualEnabled)
	{


		GetCursorPos(&P);

		if (firstTime)
		{
			P1 = P;
			firstTime = false;
		}

		changeMouseX = (float)(P.x - 960) / 1920;
		changeMouseY = (float)(P.y - 540) / 1080;

		x = changeMouseX;
		y = changeMouseY;


		SetCursorPos(960, 540);

	}

}

void Comandi::Rotate(float x, float y, float sensX, float sensY)
{

	float xRotation1 = xRotation;


	cam.rotate(0, 1, 0, x * sensX);

	yRotation += x * sensX;
	xRotation += y * sensY;

	if (yRotation > 360)
	{
		yRotation -= 360;
	}

	if (yRotation < 0)
	{
		yRotation += 360;
	}



	if (xRotation > 88)
	{
		xRotation = xRotation1;


	}

	else if (xRotation < -88)
	{
		xRotation = xRotation1;


	}

	else
	{
		cam.rotate(cam.right.x, cam.right.y, cam.right.z, -y * sensY);
	}
}