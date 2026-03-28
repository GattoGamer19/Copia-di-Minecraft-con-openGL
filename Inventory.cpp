#include "Inventory.h"

Inventory::Inventory()
{
	hotBarVBO.Gen(hotBarVertices, sizeof(hotBarVertices));
	hotBarEBO.Gen(hotBarIndices, sizeof(hotBarIndices));

	selectedSlotVBO.Gen(selectedSlot, sizeof(selectedSlot));
	selectedSlotEBO.Gen(hotBarIndices, sizeof(hotBarIndices));

	blocksVBO.Gen(hotBarVertices, sizeof(hotBarVertices));
	blocksEBO.Gen(hotBarIndices, sizeof(hotBarIndices));

	blocksVertices.resize(9);
	_blocksVertices.resize(216 * 9);
	rotatedBlockVertices.resize(216);
	_blocksIndices.resize(6 * 54);


	for (int i = 0; i < 216; i += 9)
	{
		rotatedBlockVertices[i] = uv.handBlock[i] * scale;
		rotatedBlockVertices[i + 1] = uv.handBlock[i + 1] * scale;
		rotatedBlockVertices[i + 2] = uv.handBlock[i + 2] * scale;
		rotatedBlockVertices[i + 3] = uv.handBlock[i + 3];
		rotatedBlockVertices[i + 4] = uv.handBlock[i + 4];
		rotatedBlockVertices[i + 5] = uv.handBlock[i + 5];

		rotatedBlockVertices[i + 6] = uv.handBlock[i + 6];
		rotatedBlockVertices[i + 7] = uv.handBlock[i + 7];
		rotatedBlockVertices[i + 8] = 5.0f;

	}

	glm::mat4x4 R = cam.R;
	glm::mat4x4 R1 = cam.R1;

	cam.proj(60.0f, 0.001f, larghezza, altezza, 10000000.0f);
	cam.rotate(-1.0f, 1.0f, 0.5f, 305.0f);
	//cam.rotate(1.0f, 0.0f, 0.0f, 45.0f);

	for (int i = 0; i < 216; i += 9)
	{
		glm::vec4 R(rotatedBlockVertices[i], rotatedBlockVertices[i + 1], rotatedBlockVertices[i + 2], 1.0f);
		R = cam.pro * cam.R * R;

		rotatedBlockVertices[i] = R.x;
		rotatedBlockVertices[i + 1] = R.y;
		rotatedBlockVertices[i + 2] = R.z / 100;

	}

	cam.R1 = R1;
	cam.R = R;

	for (int j = 0; j < 9; j++)
	{
		blocksVertices[j].resize(216);

		for (int i = 0; i < 216; i += 9)
		{
			float _x = rotatedBlockVertices[i];
			float _y = rotatedBlockVertices[i + 1];
			float _z = rotatedBlockVertices[i + 2];
			float _Cx = rotatedBlockVertices[i + 3];
			float _Cy = rotatedBlockVertices[i + 4];
			float _Cz = rotatedBlockVertices[i + 5];

			blocksVertices[j][i] = _x + (-x + (x / (9 / 1.090909f)) + ((j) * ((x * 1.978f) / 9)));
			blocksVertices[j][i + 1] = _y + (yC);
			blocksVertices[j][i + 2] = _z;
			blocksVertices[j][i + 3] = _Cx;
			blocksVertices[j][i + 4] = _Cy;
			blocksVertices[j][i + 5] = _Cz;
			blocksVertices[j][i + 6] = uv.uvCords[(((i) / 9) * 2) + (48 * blocks[j])];
			blocksVertices[j][i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (48 * blocks[j])];
			blocksVertices[j][i + 8] = 5.0f;

			if (blocks[j] == air)
			{
				blocksVertices[j][i + 2] = 1000000;
			}

		}

	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 216; j++)
			_blocksVertices[j + (i * 216)] = blocksVertices[i][j];
	}

	int iCount = 0;

	for (int i = 0; i < 54 * 6; i += 6)
	{

		_blocksIndices[i] = iCount;
		_blocksIndices[i + 1] = 1 + iCount;
		_blocksIndices[i + 2] = 2 + iCount;
		_blocksIndices[i + 3] = 1 + iCount;
		_blocksIndices[i + 4] = 2 + iCount;
		_blocksIndices[i + 5] = 3 + iCount;
		iCount += 4;

	}


	blocksVBO.Bind(blocksVBO.ids[0]);
	glBufferData(GL_ARRAY_BUFFER, _blocksVertices.size() * sizeof(GLfloat), _blocksVertices.data(), GL_STATIC_DRAW);
	blocksVBO.Unbind();

	blocksEBO.Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _blocksIndices.size() * sizeof(GLuint), _blocksIndices.data(), GL_STATIC_DRAW);
	blocksEBO.Unbind();

}

void Inventory::updateBlocksHotbar(int blockPos, int blockID)
{
	blocks[blockPos] = blockID;
	
	for (int j = 0; j < 9; j++)
	{
		for (int i = 0; i < 216; i += 9)
		{
			float _x = rotatedBlockVertices[i];
			float _y = rotatedBlockVertices[i + 1];
			float _z = rotatedBlockVertices[i + 2];
			float _Cx = rotatedBlockVertices[i + 3];
			float _Cy = rotatedBlockVertices[i + 4];
			float _Cz = rotatedBlockVertices[i + 5];

			blocksVertices[j][i] = _x + (-x + (x / (9 / 1.090909f)) + ((j) * ((x * 1.978f) / 9)));
			blocksVertices[j][i + 1] = _y + (yC);
			blocksVertices[j][i + 2] = _z;
			blocksVertices[j][i + 3] = _Cx;
			blocksVertices[j][i + 4] = _Cy;
			blocksVertices[j][i + 5] = _Cz;
			blocksVertices[j][i + 6] = uv.uvCords[(((i) / 9) * 2) + (48 * blocks[j])];
			blocksVertices[j][i + 7] = uv.uvCords[1 + (((i) / 9) * 2) + (48 * blocks[j])];
			blocksVertices[j][i + 8] = 5.0f;

			if (blocks[j] == air)
			{
				blocksVertices[j][i + 2] = 1000000;
			}

		}

	}

}

void Inventory::changeSelectedBlock(int blockPos)
{
	invBlock = blocks[blockPos];

	selectedSlotVBO.Bind(selectedSlotVBO.ids[0]);

	for (int i = 0; i < 32; i += 18)
	{
		selectedSlot[i] = -x + ((x / 4.5f) * blockPos);
		selectedSlot[i + 9] = -x + ((x / 4.5f) * (blockPos + 1));
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(selectedSlot), selectedSlot, GL_STATIC_DRAW);

	selectedSlotVBO.Unbind();

}

void Inventory::renderBlocksHotbar(VAO vao)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderObjects(vao, blocksVBO, 0, blocksEBO, _blocksIndices.size());
}

void Inventory::renderHotbar(VAO vao, Texture hotbarTexture)
{
	hotbarTexture.Bind();
	RenderObjects(vao, hotBarVBO, 0, hotBarEBO, 6);
	hotbarTexture.UnBind();
}

void Inventory::renderSelectedSlot(VAO vao, Texture hotbarTexture)
{
	hotbarTexture.Bind();
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderObjects(vao, selectedSlotVBO, 0, hotBarEBO, 6);
	hotbarTexture.UnBind();
}

void Inventory::renderLowInv(VAO vao, Texture hotbarTexture, Texture grassBlock)
{
	renderHotbar(vao, hotbarTexture);
	grassBlock.Bind();
	renderBlocksHotbar(vao);
	grassBlock.UnBind();
	renderSelectedSlot(vao, hotbarTexture);
	grassBlock.Bind();
}