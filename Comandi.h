#ifndef COMANDI_H
#define COMANDI_H

#include <iostream>
#include <Windows.h>
#include <math.h>
#include <glad/glad.h>

class Comandi
{

public:

	void zoom(GLfloat vertices[], char keyButton, float sens);
	void muoviX(GLfloat vertices[], char keyButton, float sens);
	void muoviZ(GLfloat vertices[], char keyButton, float sens);
	void Ruota(GLfloat vertices[], char keyButton, float sens, int asseRotazione);

};

#endif