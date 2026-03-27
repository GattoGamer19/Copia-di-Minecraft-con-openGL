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
#include "Blocco.h"
#include <vector>
#include "chunkGen.h"
#include "time.h"
#include <thread>
#include <unordered_map>
#include <map>
#include "UVcord.h"
#include "vars.h"
#include "createChunks.h"
#include "collision.h"
#include "InputSettings.h"
#include "Renderer.h"
#include "Player.h"
#include <iomanip>

int main()
{
	for (int j = -rChunkZ * 2; j < rChunkZ * 2; j++)
	{
		for (int i = -rChunkZ * 2; i < rChunkX * 2; i++)
		{

			int chunkX = j + (int)(currentChunk[0] - offset);
			int chunkZ = i + (int)(currentChunk[1] - offset);
			isCreated[chunkX][chunkZ];
		}
	}

	Comandi controller;

	Player player;
	Player player1;
	player.set();
	player1.set();

	player1.position[0] = 0;
	player1.position[1] = 0;

	InputSettings(player);

	chunkManager _chunkManager;
	_chunkManager.set();

	mutex.lock();
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	GLFWwindow* window = glfwCreateWindow(larghezza, altezza, "Minecraft", NULL, NULL);


	glfwMakeContextCurrent(window);

	gladLoadGL();

	glViewport(0, 0, larghezza, altezza);

	mutex.unlock();

	GLfloat TriangleVertices1[] =
	{
		-0.6f,-0.2f ,  0.5f,		1.0f, 1.0f, 1.0f,	 0.0f, 0.333f, 4.0f,
		-0.6f, 0.2f ,  0.5f,		1.0f, 1.0f, 1.0f,	 0.0f, 0.666f,4.0f,
		 0.6f, -0.2f,  0.5f,		1.0f, 1.0f, 1.0f,		0.25f, 0.333f,4.0f,
		 0.6f, 0.2f,   0.5f,		1.0f, 1.0f, 1.0f,		0.25f, 0.666f,4.0f,

		 -0.12f,-1.0f ,  -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.333f,4.0f,
		-0.12f, 1.0f ,  -0.5f,		1.0f, 1.0f, 1.0f,		0.0f, 0.666f,4.0f,
		 0.12f, -1.0f,  -0.5f,		1.0f, 1.0f, 1.0f,			0.25f, 0.333f,4.0f,
		 0.12f, 1.0f,   -0.5f,		1.0f, 1.0f, 1.0f,			0.25f, 0.666f,4.0f

	};

	GLfloat handBlock[216];

	float contrasto = 1;
	float outlineX = 1.0f;
	float outlineY = 1.0f;
	float outlineZ = 1.0f;

	float vx = outlineX / 2.0f;
	float vy = outlineZ / 2.0f;
	float vz = outlineY / 2.0f;


	float outlineVertices[] = {
		posX + vx, posY + vy, posZ + vz,-10,-10,-10,0,0,0,  // 0: +x,+y,+z
		posX - vx, posY + vy, posZ + vz,-10,-10,-10,0,0,0,  // 1: -x,+y,+z
		posX - vx, posY - vy, posZ + vz,-10,-10,-10,0,0,0,  // 2: -x,-y,+z
		posX + vx, posY - vy, posZ + vz,-10,-10,-10,0,0,0,  // 3: +x,-y,+z
		posX + vx, posY + vy, posZ - vz,-10,-10,-10,0,0,0,  // 4: +x,+y,-z
		posX - vx, posY + vy, posZ - vz,-10,-10,-10,0,0,0,  // 5: -x,+y,-z
		posX - vx, posY - vy, posZ - vz,-10,-10,-10,0,0,0,  // 6: -x,-y,-z
		posX + vx, posY - vy, posZ - vz,-10,-10,-10,0,0,0   // 7: +x,-y,-z
	};

	GLuint outlineIndices[] = {
		0,1,  1,2,  2,3,  3,0,
		4,5,  5,6,  6,7,  7,4,
		0,4,  1,5,  2,6,  3,7
	};

	GLuint Indices[]
	{
		0,1,2,
		1,2,3,
		4,5,6,
		5,6,7
	};


	uvCord uv;

	for (int i = 0; i < 216; i+=9)
	{
		handBlock[i] = uv.handBlock[i] + 13.15f;
		handBlock[i+1] = uv.handBlock[i+1] + 3.55f;
		handBlock[i+2] = uv.handBlock[i+2] + 2.9f;
		handBlock[i+3] = uv.handBlock[i+3];
		handBlock[i+4] = uv.handBlock[i+4];
		handBlock[i+5] = uv.handBlock[i+5];

		handBlock[i + 6] = uv.handBlock[i + 6];
		handBlock[i + 7] = uv.handBlock[i + 7];
		handBlock[i+8] = 5.0f;

	}

	glm::mat4x4 R = cam.R;
	glm::mat4x4 R1 = cam.R1;

	cam.proj(60.0f, 0.001f, larghezza, altezza, 10000.0f);
	cam.rotate(0.0f, 1.0f, 0.0f, 15.0f);
	cam.rotate(1.0f, 0.0f, 0.0f, -25.0f);

	for (int i = 0; i < 216; i += 9)
	{
		glm::vec4 R(handBlock[i], handBlock[i + 1], handBlock[i + 2], 1.0f);
		R = cam.R * R;

		handBlock[i] = R.x;
		handBlock[i+1] = R.y;
		handBlock[i+2] = R.z;

		handBlock[i + 6] = uv.uvCords[(((i) / 9) * 2) + (48 * invBlock)];
		handBlock[i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (48 * invBlock)];

	}

	cam.R1 = R1;
	cam.R = R;
	Shader ShaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();

	VBO vboCursor;
	vboCursor.Gen(TriangleVertices1, sizeof(TriangleVertices1));
	VBO vboHandBlock;
	vboHandBlock.Gen(handBlock, sizeof(handBlock));
	VBO vboOutline;
	vboOutline.Gen(outlineVertices, sizeof(outlineVertices));


	EBO eboCursor;
	eboCursor.Gen(Indices, sizeof(Indices));
	EBO eboHandBlock;
	eboHandBlock.Gen(uv.indices, sizeof(uv.indices));
	EBO eboOutline;
	eboOutline.Gen(outlineIndices, sizeof(outlineIndices));

	

	for (int j = 0; j < nChunkZ ;j++)
	{
		for (int i = 0; i < nChunkX; i++)
		{
			vbo[j][i].Gen(TriangleVertices1, sizeof(TriangleVertices1));
			isVisible[j][i] = false;
		}
	}


	for (int j = 0; j < nChunkZ; j++)
	{
		for (int i = 0; i < nChunkX; i++)
		{
			ebo[j][i].Gen(Indices, sizeof(Indices));
		}
	}



	Texture grassTexture(ShaderProgram.ID, "terrain.png");
	grassTexture.Bind();

	glEnable(GL_DEPTH_TEST);	


	ShaderProgram.Activate();


	glLineWidth(2.5f);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	GLuint fogDistance = glGetUniformLocation(ShaderProgram.ID, "fogDistance");
	glUniform1f(fogDistance, ((offset - ((offset / 15))) * chunkSize) / 100);
	
	GLuint fogLength = glGetUniformLocation(ShaderProgram.ID, "fogLength");
	glUniform1f(fogLength, (offset * 1.0f) / 100);

	GLuint offsetX = glGetUniformLocation(ShaderProgram.ID, "offsetX");
	glUniform1f(offsetX, -player.position[0] / 100);

	GLuint offsetZ = glGetUniformLocation(ShaderProgram.ID, "offsetZ");
	glUniform1f(offsetZ, -player.position[1] / 100);

	_chunkManager.getVisibleChunksRot(player);
	
	std::thread create(&chunkManager::createChunks, &_chunkManager);


	while (!glfwWindowShouldClose(window))
	{

		double startUpdateTime = clock();

		_chunkManager.bufferPlayer = player;
		_chunkManager.preRender();
		//_chunkManager.FreeOldChunks(player);
		//_chunkManager.createChunks(player);
	
		while (_chunkManager.createdFirstChunks)
		{

		}

		//if(GetAsyncKeyState('Y') & 1)
		//std::cout << std::setprecision(17) << player.position[0] << " " << player.position[2] << " " << player.position[1] << '\n';
		//std::cout << currentChunk[0] << " " << currentChunk[1]<<'\n';
			
		if (GetAsyncKeyState('M') & 1)
		{
			_chunkManager.outputVBOs();
			
		}

			controller.GetInvBlock();

			player.getLookingBlock(bufferChunkAssignedVBO);
			player.blockInteraction(bufferChunkAssignedVBO);
			

			for (int i = 0; i < 10; i++)
			{
				//std::cout << "giiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii" << std::endl;
			}

			float outlineVertices1[] =
			{
				posX + vx, posY + vy, posZ + vz,-10,-10,-10,0,0,0,  // 0: +x,+y,+z
				posX - vx, posY + vy, posZ + vz,-10,-10,-10,0,0,0,  // 1: -x,+y,+z
				posX - vx, posY - vy, posZ + vz,-10,-10,-10,0,0,0,  // 2: -x,-y,+z
				posX + vx, posY - vy, posZ + vz,-10,-10,-10,0,0,0,  // 3: +x,-y,+z
				posX + vx, posY + vy, posZ - vz,-10,-10,-10,0,0,0,  // 4: +x,+y,-z
				posX - vx, posY + vy, posZ - vz,-10,-10,-10,0,0,0,  // 5: -x,+y,-z
				posX - vx, posY - vy, posZ - vz,-10,-10,-10,0,0,0,  // 6: -x,-y,-z
				posX + vx, posY - vy, posZ - vz,-10,-10,-10,0,0,0   // 7: +x,-y,-z
			};


			vboOutline.Bind(vboOutline.ids[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(outlineVertices1), outlineVertices1, GL_STATIC_DRAW);
			vboOutline.Unbind();

			float x, y;

			controller.checkRotEnabled();

			controller.getMouseMovement(x, y);

			controller.Rotate(x, y, 70, 70);

			controller.Move(player, 0.04f, bufferChunkAssignedVBO);
			

			if (GetAsyncKeyState('T'))
			{
				GLuint fogDistance = glGetUniformLocation(ShaderProgram.ID, "fogDistance");
				glUniform1f(fogDistance, ((offset - (1 + (offset / 10))) * chunkSize) / 100);

				GLuint fogLength = glGetUniformLocation(ShaderProgram.ID, "fogLength");
				glUniform1f(fogLength, (offset * 1.0f) / 100);
			}

			if (GetAsyncKeyState('U'))
			{
				GLuint fogDistance = glGetUniformLocation(ShaderProgram.ID, "fogDistance");
				glUniform1f(fogDistance, (10000000 + (offset - (1 + (offset / 10))) * 16) / 100);

				GLuint fogLength = glGetUniformLocation(ShaderProgram.ID, "fogLength");
				glUniform1f(fogLength, (offset * 1.0f) / 100);
			}
				

			GLuint aR = glGetUniformLocation(ShaderProgram.ID, "R");
			glUniformMatrix4fv(aR, 1, GL_FALSE, glm::value_ptr(cam.R));

			GLuint aView = glGetUniformLocation(ShaderProgram.ID, "view");
			glUniformMatrix4fv(aView, 1, GL_FALSE, glm::value_ptr(cam.view));

			GLuint aProj = glGetUniformLocation(ShaderProgram.ID, "proj");
			glUniformMatrix4fv(aProj, 1, GL_FALSE, glm::value_ptr(cam.pro));




			glClearColor(0.411f, 0.827f, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			

			RenderObjects(VAO1, vboCursor, 0, eboCursor, 12);


			glEnable(GL_BLEND);

			if (change)
			{
				for (int i = 0; i < 216; i += 9)
				{
					handBlock[i + 6] = uv.uvCords[(((i) / 9) * 2) + (UVoffset * invBlock)];
					handBlock[i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (UVoffset * invBlock)];
				}

				vboHandBlock.Bind(vboHandBlock.ids[0]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(handBlock), handBlock, GL_STATIC_DRAW);
				change = false;

			}

			currentChunk[0] = (int)(player.position[0] + 0.5f) / chunkSize;
			currentChunk[1] = (int)(player.position[1] + 0.5f) / chunkSize;

			if (player.position[0] < 0)
				currentChunk[0]--;

			if (player.position[1] < 0)
				currentChunk[1]--;

			//std::cout << xRotation << "  " << yRotation << '\n';
	
			_chunkManager.RenderChunks(VAO1, player);

			//std::cout << currentChunk[1] << '\n';

			if (GetAsyncKeyState('H'))
				_chunkManager.outputVBOs();

			if (GetAsyncKeyState('L'))
				realGenChunk = false;

			if (GetAsyncKeyState('P'))
				realGenChunk = true;

			RenderLines(VAO1, vboOutline, 1 ,eboOutline, 24);

		

			glClear(GL_DEPTH_BUFFER_BIT);

			glDisable(GL_BLEND);

			RenderObjects(VAO1, vboHandBlock, 0, eboHandBlock, 36);

	

			ShaderProgram.Activate();
			VAO1.Bind();

			glfwSwapBuffers(window);
			glfwPollEvents();

			fps += 1;

			updateTime += clock() - startUpdateTime;
			frameTime = clock() - startUpdateTime;

			if ((updateTime / CLOCKS_PER_SEC) > titleUpdateTime)
			{
				std::stringstream fpsString;

				fpsString << "Minecraft" << " || FPS: " << (fps / (updateTime / CLOCKS_PER_SEC));

				glfwSetWindowTitle(window, fpsString.str().c_str());

				updateTime = 0;
				fps = 0;
			}

			//for (int i = 0; i < 1000; i++)
				//std::cout << "adslkjfkajfsaòlsafjdòlkasfjlòsafjlòjasflkd";

		}

		glDeleteTextures(1, &grassTexture.id);
		VAO1.Delete();
		ShaderProgram.Delete();

		glfwTerminate();
	}
