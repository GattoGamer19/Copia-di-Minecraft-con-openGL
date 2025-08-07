#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 Tex;
in vec2 Tex1;
in float Count;


uniform sampler2D tex0;
vec4 highLightTexture;

void main()
{

   FragColor = texture(tex0, Tex);
   FragColor = vec4(FragColor.x + Color.x, FragColor.y + Color.y, FragColor.z + Color.z, 1);

   if(Count >= 0 && Count < 4)
   {
      highLightTexture = texture(tex0, Tex1);
      FragColor = vec4(FragColor.x - highLightTexture.x, FragColor.y - highLightTexture.y, FragColor.z - highLightTexture.z,1);
   }


}