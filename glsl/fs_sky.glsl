#version 440
in vec2 TexCoord;
in float height;
out vec4 Color;

uniform sampler2D Texture;

void main()
{
    Color = texture(Texture, TexCoord);
}