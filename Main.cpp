#include <iostream>
#include <Windows.h>
#include <windowsx.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "shaderClass.h"
#include "stb/stb_image.h"	
#include "Texture.h"
#include <math.h>
#include "Comandi.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{

	float larghezza = 1920;
	float altezza = 1080;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(larghezza, altezza, "Minecraft", NULL, NULL);

	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, larghezza, altezza);

	float aSensZoom = 0.1;
	float posizioneCam = 0;
	float sensRotazione = 0;
	float sensX = 0;
	float sensY = 0;
	int asse = 0; 
	
	camera cam;


	GLfloat TriangleVertices1[] =
	{
		-0.5f,-0.5f ,  0.5,		0.2, 0.2, 0.2,	 0.0f, 0.333f,
		-0.5f, 0.5f ,  0.5,		0.2, 0.2, 0.2,	 0.0f, 0.666f,
		 0.5f, -0.5f,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.333f,
		 0.5f, 0.5f,   0.5,		0.2, 0.2, 0.2,		0.25f, 0.666f,

		 -0.5f,-0.5f ,  -0.5,		0.2, 0.2, 0.2,		0.0f, 0.333f,
		-0.5f, 0.5f ,  -0.5,		0.2, 0.2, 0.2,		0.0f, 0.666f,
		 0.5f, -0.5f,  -0.5,		0.2, 0.2, 0.2,			0.25f, 0.333f,
		 0.5f, 0.5f,   -0.5,		0.2, 0.2, 0.2,			0.25f, 0.666f,

		 0.5f,-0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.333f,
		0.5f, 0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.666f,
		 0.5f, -0.5f,  -0.5,		0.2, 0.2, 0.2,			0.5f, 0.333f,
		 0.5f, 0.5f,   -0.5,		0.2, 0.2, 0.2,			0.5f, 0.666f,

		 -0.5f,-0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.333f,
		-0.5f, 0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.666f,
		 -0.5f, -0.5f,  -0.5,		0.2, 0.2, 0.2,			0.5f, 0.333f,
		 -0.5f, 0.5f,   -0.5,		0.2, 0.2, 0.2,			0.5f, 0.666f,

		  -0.5f, 0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.666f,
		-0.5f, 0.5f ,  -0.5,		0.2, 0.2, 0.2,		0.25f, 1.0f,
		 0.5f, 0.5f,   0.5,		0.2, 0.2, 0.2,			0.5f, 0.666f,
		 0.5f, 0.5f,   -0.5,		0.2, 0.2, 0.2,			0.5f, 1.0f,

		 -0.5f,-0.5f ,  0.5,		0.2, 0.2, 0.2,		0.25f, 0.666f,
		-0.5f, -0.5f ,  -0.5,		0.2, 0.2, 0.2,		0.25f, 1.0f,
		 0.5f, -0.5f,  0.5,		0.2, 0.2, 0.2,			0.5f, 0.666f,
		 0.5f, -0.5f,   -0.5,		0.2, 0.2, 0.2,		0.5f, 1.0f

	};

	GLfloat allVertices[1000];


		for (int i = 0; i < 1000; i++)
		{
			allVertices[i] = 0;
		}
	

	GLuint Indices[]
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7,
		8,9,10,
		9,10,11,
		12,13,14,
		13,14,15,
		16,17,18,
		17,18,19,
		20,21,22,
		21,22,23,

	};


	Shader ShaderProgram("default.vert","default.frag");


	VAO VAO1;
	VAO1.Bind();

	EBO EBO1(Indices, sizeof(Indices));


	VBO VBO1(TriangleVertices1, sizeof(TriangleVertices1));

	VAO1.LinkVBO(VBO1, 3, 0, VBO1.ids[999], 8, 0);
	VAO1.LinkVBO(VBO1, 3, 1, VBO1.ids[999], 8, 3);
	VAO1.LinkVBO(VBO1, 2, 2, VBO1.ids[999], 8, 6);



	VBO1.Unbind();
	VAO1.Unbind();
	EBO1.Unbind();

	ShaderProgram.Activate();

	Texture grassTexture(ShaderProgram.ID, "grass_texture.png");
	Texture steveTexture(ShaderProgram.ID, "steve.png");
 

	float countX = 1;
	float countY = 0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			VBO1.Bind(VBO1.ids[j + (i * 10)]);
			GLfloat vertices[192];


			for (int k = 0; k < 192; k++)
			{

				vertices[k] = TriangleVertices1[k];

			}


			for (int l = 0; l < 24; l++)
			{

				vertices[(l * 8)] += 1.0f * countX;
				vertices[(l * 8) + 1] += 1.0f * countY;

			}




			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			countX++;
		}
		countX = 1;
		countY++;
	}

	countX = 1;
	countY = 0;


	for (int i = 10; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			VBO1.Bind(VBO1.ids[j + (i * 10)]);
			GLfloat vertices[192];


			for (int k = 0; k < 192; k++)
			{

				vertices[k] = TriangleVertices1[k];

			}


			for (int l = 0; l < 24; l++)
			{

				vertices[(l * 8)] += 15 + (1.0f * countX);
				vertices[(l * 8) + 1] += 1.0f * countY;
				vertices[(l * 8) + 2] += 10;

			}




			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			countX++;
		}
		countX = 1;
		countY++;
	}

	countX = 1;
	countY = 1;



	for (int i = 20; i < 40; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			VBO1.Bind(VBO1.ids[j + (i * 20)]);
			GLfloat vertices[192];


			for (int k = 0; k < 192; k++)
			{

				vertices[k] = TriangleVertices1[k];

			}


			for (int l = 0; l < 24; l++)
			{

				vertices[(l * 8)] += -4 + (1.0f * countX);
				vertices[(l * 8) + 1] -= 1.0f;
				vertices[(l * 8) + 2] += -4 + (1.0f * countY);

			}




			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			countX++;
		}
		countX = 1;
		countY++;
	}

	countX = 1;
	countY = 1;


	glEnable(GL_DEPTH_TEST);

	POINT P;
	POINT P1;
	bool firstTime = true;
	float changeMouseX;
	float changeMouseY;
	bool yes = true;
	while (!glfwWindowShouldClose(window))
	{
		
		{
			if (GetAsyncKeyState('L') & 0x8000 && (TriangleVertices1[6] + 0.002f) < 1)
			{
				for (int i = 3; i < 64; i += 8)
				{
					TriangleVertices1[i] += 0.002f;
					TriangleVertices1[i+1] += 0.002f;
					TriangleVertices1[i+2] += 0.002f;

					std::cout << TriangleVertices1[i] << "  " << i << '\n';

				}



				std::cout << "premuto";

			}

			if (GetAsyncKeyState('O') & 0x8000 && (TriangleVertices1[0] - 0.002f) > -1)
			{
				for (int i = 3; i < 64; i += 8)
				{
					TriangleVertices1[i] -= 0.002f;
					TriangleVertices1[i+1] -= 0.002f;
					TriangleVertices1[i+2] -= 0.002f;

					std::cout << TriangleVertices1[i] << "  " << i << '\n';

				
				}

				std::cout << "premuto";

			}
			
		}
		Comandi comandi;


		if (yes)
		{


			GetCursorPos(&P);

			if (firstTime)
			{
				P1 = P;
				firstTime = false;
			}

			changeMouseX = (float)(P1.x - P.x) / 1920;
			changeMouseY = (float)(P1.y - P.y) / 1080;
			P1 = P;
			P1.x += P1.x - 960;
			P1.y += P1.y - 540;



			SetCursorPos(960, 540);
			
		}


		cam.proj(45.0f, 0.1f, larghezza, altezza, 100000.0f);


		GLuint aR = glGetUniformLocation(ShaderProgram.ID, "R");
		glUniformMatrix4fv(aR,1,GL_FALSE, glm::value_ptr(cam.R));

		GLuint aView = glGetUniformLocation(ShaderProgram.ID, "view");
		glUniformMatrix4fv(aView, 1, GL_FALSE, glm::value_ptr(cam.view));

		GLuint aProj = glGetUniformLocation(ShaderProgram.ID, "proj");
		glUniformMatrix4fv(aProj, 1, GL_FALSE, glm::value_ptr(cam.pro));




		if (GetAsyncKeyState('N') & 0x8000)
		{
			cam.rotate(0, 1, 0, -0.2f);
			yes = false;
		}

		if (GetAsyncKeyState('M') & 0x8000)
		{
			cam.rotate(0, 1, 0, 0.2f);
		}


			cam.rotate(0, 1, 0, changeMouseX * 70);
			cam.rotate(cam.right.x, cam.right.y, cam.right.z, -changeMouseY * 70);
				

		if (GetAsyncKeyState('S') & 0x8000)
		{
			cam.move(0, 0.2f);
			yes = true;
		}

		if (GetAsyncKeyState('W') & 0x8000)
		{
			cam.move(1, 0.2f);
		}


		if (GetAsyncKeyState('D') & 0x8000)
		{
			cam.move(2, 0.07f);
		}

		if (GetAsyncKeyState('A') & 0x8000)
		{
			cam.move(3, 0.07f);
		}

		



		glClearColor(0.1, 0.21, 0.31, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ShaderProgram.Activate();
	
		grassTexture.Bind();

		VAO1.Bind();
		

		for (int i = 0; i < 801; i++)
		{
			VBO1.Bind(VBO1.ids[i]);
			VAO1.LinkVBO(VBO1, 3, 0, VBO1.ids[i], 8, 0);
			VAO1.LinkVBO(VBO1, 3, 1, VBO1.ids[i], 8, 3);
			VAO1.LinkVBO(VBO1, 2, 2, VBO1.ids[i], 8, 6);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}


		




		glfwSwapBuffers(window);
		glfwPollEvents();				
	}

	glDeleteTextures(1, &grassTexture.id);
	glDeleteTextures(1, &steveTexture.id);
	VAO1.Delete();
	ShaderProgram.Delete();

	glfwTerminate();
}

