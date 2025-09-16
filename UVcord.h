#ifndef UVCORD_H
#define UVCORD_H

#define air 0
#define dirtGrass 1
#define oakPlanks 2
#define oakLog 3
#define stone 4
#define dirt 5
#define water 6

struct uvCord
{
	float x = 16.0f / 256.0f;
	float y = 16.0f / 256.0f;

	float xU = 16.0f / 256.0f;
	float yU = 16.0f / 256.0f;

	float cx = 1.7f;
	float cy = -0.7f;
	float cz = 0.6f;

	float s = 0.25;

	float contrasto = 1.55f;

	GLuint indices[36] = {
		0, 1, 2,
		1, 2, 3,
		4, 5, 6,
		5, 6, 7,
		8, 9,10,
	   9,10,11,
	   12,13,14,
	   13,14,15,
	   16,17,18,
	   17,18,19,
	   20,21,22,
	   21,22,23
	};

	GLfloat handBlock[216] =
	{

		-0.5f,-0.5f ,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 (xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 (xU / 16) + xU * 3, yU * 16, 1.0f,
		 0.5f, -0.5f,  0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 xU * 4,(yU / 16) + yU * 15, 2.0f,
		 0.5f, 0.5f,   0.5f,		-0.03f * contrasto, -0.03f * contrasto, -0.03f * contrasto,	 xU * 4, yU * 16, 3.0f,

		 -0.5f,-0.5f ,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15, 0.0f,
		-0.5f, 0.5f ,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.075f * contrasto, -0.075f * contrasto, -0.075f * contrasto,	xU * 4, yU * 16,3.0f,

		 0.5f,-0.5f ,  0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.0f,
		0.5f, 0.5f ,  0.5f,			-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 0.5f, -0.5f,  -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		-0.04f * contrasto, -0.04f * contrasto, -0.04f * contrasto,	xU * 4, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,		(xU / 16) + xU * 3, (yU / 16) + yU * 15,0.f,
		-0.5f, 0.5f ,  0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	(xU / 16) + xU * 3, yU * 16,1.0f,
		 -0.5f, -0.5f,  -0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	xU * 4,(yU / 16) + yU * 15,2.0f,
		 -0.5f, 0.5f,   -0.5f,		-0.05f * contrasto, -0.05f * contrasto, -0.05f * contrasto,	xU * 4, yU * 16,3.0f,

		  -0.5f, 0.5f ,  0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,		0, (yU / 16.0f) + yU * 15,0.0f,
		-0.5f, 0.5f ,  -0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	0, yU * 16,1.0f,
		 0.5f, 0.5f,   0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	  xU,(yU / 16.0f) + yU * 15,2.0f,
		 0.5f, 0.5f,   -0.5f,		0.0f * contrasto, 0.0f * contrasto, 0.0f * contrasto,	  xU, yU * 16,3.0f,

		 -0.5f,-0.5f ,  0.5f,		-0.115f * contrasto, -0.115f * contrasto, -0.115f * contrasto,		(xU / 16) + xU * 2,(yU / 16) + yU * 15,0.0f,
		-0.5f, -0.5f ,  -0.5f,		-0.115f * contrasto, -0.115f * contrasto, -0.115f * contrasto,		(xU / 16) + xU * 2, yU * 16,1.0f,
		 0.5f, -0.5f,  0.5f,		-0.115f * contrasto, -0.115f * contrasto, -0.115f * contrasto,		xU * 3, (yU / 16) + yU * 15,2.0f,
		 0.5f, -0.5f,   -0.5f,		-0.115f * contrasto, -0.115f * contrasto, -0.115f * contrasto,		xU * 3, yU * 16,3.0f
	};

	float uvCords[7 * 48] =
	{
		//air
		
		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0,

		0,0,
		0,0,
		0,0,
		0,0,

		//dirt grass

		 (x / 16) + x * 3, (y / 16) + y * 15,
		 (x / 16) + x * 3, y * 16,
		 x * 4,(y / 16) + y * 15,
		 x * 4, y * 16,

		 (x / 16) + x * 3, (y / 16) + y * 15,
		 (x / 16) + x * 3, y * 16,
		 x * 4,(y / 16) + y * 15,
		 x * 4, y * 16,

		 (x / 16) + x * 3, (y / 16) + y * 15,
		 (x / 16) + x * 3, y * 16,
		 x * 4,(y / 16) + y * 15,
		 x * 4, y * 16,

		 (x / 16) + x * 3, (y / 16) + y * 15,
		 (x / 16) + x * 3, y * 16,
		 x * 4,(y / 16) + y * 15,
		 x * 4, y * 16,

		 0, (y / 16.0f) + y * 15,
		 0, y * 16,
		 x,(y / 16.0f) + y * 15,
		 x, y * 16,

		 (x / 16) + x * 2,(y / 16) + y * 15,
		 (x / 16) + x * 2, y * 16,
		  x * 3, (y / 16) + y * 15,
		  x * 3, y * 16,


		  //oak planks


		  (x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,

			(x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,

			(x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,

			(x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,

			(x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,

			(x / 16) + x * 4, y * 16,
		  (x / 16) + x * 4,(y / 16) + y * 15,
		   x * 5,y * 16,
		   x * 5, (y / 16) + y * 15,





		   //oak log


			x * 5, y * 15,
			x * 5, y * 14,
			x * 4, y * 15,
			x * 4, y * 14,

			x * 5, y * 15,
			x * 5, y * 14,
			x * 4, y * 15,
			x * 4, y * 14,

			x * 5, y * 15,
			x * 5, y * 14,
			x * 4, y * 15,
			x * 4, y * 14,

			x * 5, y * 15,
			x * 5, y * 14,
			x * 4, y * 15,
			x * 4, y * 14,

			x * 6, y * 15,
			x * 6, y * 14,
			x * 5, y * 15,
			x * 5, y * 14,

			x * 6, y * 15,
			x * 6, y * 14,
			x * 5, y * 15,
			x * 5, y * 14,

			//stone

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			 x, y * 15,
			 x, y * 16,
			 x * 2, y * 15,
			 x * 2, y * 16,

			  //dirt

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  x * 2, y * 15,
			  x * 2, y * 16,
			  x * 3, y * 15,
			  x * 3, y * 16,

			  //water

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3,

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3,

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3,

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3,

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3,

			  x * 15, y * 2,
			  x * 15, y * 3,
			  x * 16, y * 2,
			  x * 16, y * 3
	};
};


#endif