#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
	
public:

	glm::vec3 forward = glm::vec3(0.0f, 0.0f, -0.01f);
	glm::vec3 forward1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 backward = glm::vec3(0.0f, 0.0f,0.01f);
	glm::vec3 right = glm::vec3(0.01f, 0.0f, 0.0f);
	glm::vec3 left = glm::vec3(-0.01f, 0.0f, 0.0f);
	glm::vec4 temp = glm::vec4(forward, 1.0f);



	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 R1 = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 pro = glm::mat4(1.0f);

	void rotate(float x, float y, float z, float angolo);
	void move(int n, float sens, float position[3]);
	void proj(float degree, float min, float larghezza, float altezza, float max);

};

#endif