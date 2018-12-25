#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float Time;

void main()
{
    vec3 Position = position;
    Position.z = Position.z - 3*sin(15*Time+20*Position.x + 20*Position.y)-3*sin(15*Time-20*Position.x + 20*Position.y) - 3;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(Position, 1.0f);
    TexCoord = vec2(texCoord.x, 1 - texCoord.y);
}