#version 330 core
out vec4 FragColor;

in vec3 Color;
in vec2 Tex;
in vec2 Tex1;
in float Count;
in float dist;


uniform sampler2D tex0;
uniform float fogDistance;
uniform float fogLength;
vec4 highLightTexture;

void main()
{

   FragColor = texture(tex0, Tex);
   FragColor = vec4(FragColor.x + Color.x, FragColor.y + Color.y, FragColor.z + Color.z, FragColor.w);

   if(Count != 4)
   {
     FragColor.w = 1.0f;
   }


        if(dist > fogDistance && (Count > 7 || Count < 0))
        { 
             float t = -(fogDistance - dist) / fogLength;
             FragColor = vec4(FragColor.x + (0.411f - FragColor.x) * t, FragColor.y + (0.827f - FragColor.y) * t, FragColor.z + (1 - FragColor.z) * t, 1.0f);
             
             if(dist > fogLength + fogDistance)
             FragColor.w = 0.0f;
             
        }     


}