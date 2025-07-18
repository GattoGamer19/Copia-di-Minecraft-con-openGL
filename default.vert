#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;

out vec3 Color;
out vec2 Tex;

uniform mat4 R;
uniform mat4 view;
uniform mat4 proj;

void main()
{
   float y = aPos.y - 1;
   gl_Position = proj * R *(view * vec4(aPos.x * 0.1f, y * 0.1f, aPos.z * 0.1f, 1.0f));

   Color = aColor;
   Tex = aTex;
}