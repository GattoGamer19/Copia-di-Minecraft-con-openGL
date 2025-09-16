#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in float count;

out vec3 Color;
out vec2 Tex;
out vec2 Tex1;
out float Count;

uniform mat4 R;
uniform mat4 view;
uniform mat4 proj;

float xU = 16.0f / 256.0f;
float yU = 16.0f / 256.0f;

void main()
{

 float y = aPos.y - 129 + 60;


   gl_Position = proj * R *(view * vec4(-0.5f + aPos.x * 0.1f, y * 0.1f,-0.5f + aPos.z * 0.1f, 1.0f));

   Color = aColor;
   Tex = aTex;


   if(count == 0)
   {
    Tex1 = vec2(xU*8,  yU*1);
   } 
   
   if(count == 1)
   {
    	 Tex1 = vec2( xU*8, yU*8);
   } 
   
   if(count == 2)
   {
     Tex1 = vec2(xU*15, yU*1);
   }
   
   if(count == 3)
   {
     Tex1 = vec2(xU*15, yU*8);
   } 

   else if(count == 4)
   {
     gl_Position = vec4(aPos.x * 0.035f, aPos.y * 0.035f, -0.99f, 1.0f);
   }

   else if (count == 5)
   {
     gl_Position = proj * vec4(-0.5f + aPos.x * 0.05f, -0.42f + aPos.y * 0.08f,-0.1f + aPos.z * 0.05f, 1.0f);
   }

   Count = count;
    


   
}