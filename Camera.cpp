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
	forward = glm::vec3((temp * R1).x, 0.0f, (temp * R1).z);
	backward = -forward;
	right = glm::vec3(forward.z,0.0f, -forward.x);
	left = -right;
}

void camera::move(int n, float sens)
{
	switch (n)
	{

	case 0:
		view = glm::translate(view, forward * sens);
		break;

	case 1:
		view = glm::translate(view, backward * sens);
		break;

	case 2:
		view = glm::translate(view, right * sens);
		break;

	case 3:
		view = glm::translate(view, left * sens);
		break;
	}
	
}

void camera::proj(float degree, float min, float larghezza, float altezza, float max)
{
	pro = glm::perspective(glm::radians(degree), larghezza / altezza, min, max);
}