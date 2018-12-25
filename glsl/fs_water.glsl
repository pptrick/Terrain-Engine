#version 440
in vec2 TexCoord;
in float height;
out vec4 Color;

uniform float TimeCount;
uniform sampler2D Texture;

void main()
{
    Color = texture(Texture, TexCoord*60+TimeCount);
    Color.x = Color.x * 1.5;
    Color.y = Color.y * 1.5;
    Color.z = Color.z * 1.5;
    Color.w = Color.w - 0.3;
}