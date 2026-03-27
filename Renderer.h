#pragma once
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void RenderObjects(VAO vao, VBO vbo, int vboIND, EBO ebo, int indexNumbers);
void RenderLines(VAO vao, VBO vbo, int vboIND, EBO ebo, int indexNumbers);
