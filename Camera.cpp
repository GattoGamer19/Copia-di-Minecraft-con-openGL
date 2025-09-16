#include "Camera.h"
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void camera::rotate(float x, float y, float z, float angolo)
{

	R = glm::rotate(R, glm::radians(angolo), glm::vec3(x, y, z));
	R1 = glm::rotate(R1, glm::radians(angolo), glm::vec3(x, y, z));
	forward = glm::vec3((temp * R1).x, (temp*R1).y, (temp * R1).z);
	float magn = sqrtf((forward.x*forward.x) + (forward.z*forward.z));
	forward = glm::vec3(forward.x / magn, forward.y / magn, forward.z / magn);
	backward = -forward;
	right = glm::vec3(forward.z,0.0f, -forward.x);
	left = -right;
}

void camera::move(int n, float sens, float position[3])
{

	glm::vec3 forwardd1 = glm::vec3(forward1.x, 0, 0);
	glm::vec3 forward2 = glm::vec3(0, 0, forward1.z);
	glm::vec3 forward3 = glm::vec3(0, forward1.y, 0);
	glm::vec3 right1 = glm::vec3(right.x, 0, 0);
	glm::vec3 right2 = glm::vec3(0, 0, right.z);

	switch (n)
	{

	case 0:
		view = glm::translate(view,forwardd1* sens);
		position[0] += (-forwardd1 * sens * 10.0f).x;
		position[1] += (-forwardd1 * sens * 10.0f).z;
		position[2] += (-forwardd1 * sens * 10.0f).y;

		break;

	case 1:
		view = glm::translate(view, -forwardd1 * sens);
		position[0] += (forwardd1 * sens * 10.0f).x;
		position[1] += (forwardd1 * sens * 10.0f).z;
		position[2] += (forwardd1 * sens * 10.0f).y;

		break;

	case 2:
		view = glm::translate(view, right1 * sens);
		position[0] += (-right1 * sens * 10.0f).x;
		position[1] += (-right1 * sens * 10.0f).z;
		position[2] += (-right1 * sens * 10.0f).y;

		break;

	case 3:
		view = glm::translate(view, -right1 * sens);
		position[0] += (right1 * sens * 10.0f).x;
		position[1] += (right1 * sens * 10.0f).z;
		position[2] += (right1 * sens * 10.0f).y;
		break;


	case 4:
		view = glm::translate(view, glm::vec3(1, 0, 0) * (sens/115.0f));

		
			position[0] += (1.0f * -sens/11.5f);
		
			break;

	case 5:
		view = glm::translate(view, glm::vec3(0, 0, 1) * (sens / 115.0f));

		
			position[1] += (1.0f * -sens / 11.5f);

		
		break;

	case 6:
		view = glm::translate(view, glm::vec3(-1, 0, 0) * (sens / 115.0f));

		
			position[0] += (1.0f * sens / 11.5f);
	

		break;

	case 7:
		view = glm::translate(view, glm::vec3(0, 0, -1) * (sens / 115.0f));

		
			position[1] += (1.0f * sens / 11.5f);

		break;	
	
	case 8:
		view = glm::translate(view, glm::vec3(0, -1, 0) * (sens / 115.0f));

		
			position[2] += (1.0f * sens / 11.5f);

		break;	
	
	case 9:
		view = glm::translate(view, glm::vec3(0, 1, 0) * (sens / 115.0f));

		
			position[2] += (1.0f * -sens / 11.5f);

		break;

	case 10:
		glm::vec3 up = glm::vec3(0, -1, 0);
		view = glm::translate(view, up * sens);
		position[0] += (-up * sens * 10.0f).x;
		position[1] += (-up * sens * 10.0f).z;
		position[2] += (-up * sens * 10.0f).y;
		break;	
	
	case 11:
		glm::vec3 down = glm::vec3(0, 1, 0);
		view = glm::translate(view, down * sens);
		position[0] += (-down * sens * 10.0f).x;
		position[1] += (-down * sens * 10.0f).z;
		position[2] += (-down * sens * 10.0f).y;
		break;

	case 12:
		view = glm::translate(view, forward2 * -sens);
		position[0] += (forward2 * sens * 10.0f).x;
		position[1] += (forward2 * sens * 10.0f).z;
		position[2] += (forward2 * sens * 10.0f).y;

		break;

	case 13:
		view = glm::translate(view, -forward3 * sens);
		position[0] += (forward3 * sens * 10.0f).x;
		position[1] += (forward3 * sens * 10.0f).z;
		position[2] += (forward3 * sens * 10.0f).y;
		break;

	case 14:
		view = glm::translate(view, right2 * sens);
		position[0] += (-right2 * sens * 10.0f).x;
		position[1] += (-right2 * sens * 10.0f).z;
		position[2] += (-right2 * sens * 10.0f).y;

		break;

	case 15:
		view = glm::translate(view, -right2 * sens);
		position[0] += (right2 * sens * 10.0f).x;
		position[1] += (right2 * sens * 10.0f).z;
		position[2] += (right2 * sens * 10.0f).y;
		break;

	}
	
}

void camera::proj(float degree, float min, float larghezza, float altezza, float max)
{
	pro = glm::perspective(glm::radians(degree), larghezza / altezza, min, max);
}