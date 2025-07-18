#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 Tex;


uniform sampler2D tex0;

void main()
{
   FragColor = texture(tex0, Tex);
   FragColor = vec4(FragColor.x + Color.x, FragColor.y + Color.y, FragColor.z + Color.z, 1);
}