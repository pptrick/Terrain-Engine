#version 440
in vec2 TexCoord;
in float Edge;
out vec4 Color;

uniform sampler2D Texture;
uniform float TimeCount;

void main()
{
    float Time=TimeCount;
    Color = texture(Texture, TexCoord*0.5 + Time);
    Color.x = Color.x * 1.5;
    Color.y = Color.y * 1.5;
    Color.z = Color.z * 1.5;
    Color.w = Color.w - 0.5;
    //Color = vec4(1.0,0.0,0.0,1.0);
}