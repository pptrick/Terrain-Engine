#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float edge;

out vec2 TexCoord;
out float Edge;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    Edge = edge;
    vec3 Position = position;
    Position.z = Position.z - 5;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(Position, 1.0f);
    TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}