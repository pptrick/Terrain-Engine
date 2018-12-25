#version 440

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;
out float height;
out float Draw;
out float down;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform int reflect;

void main()
{
    Draw = 0.0;
    down = 0.0;
    vec3 Position = position;
    if(reflect==1)
    {
       Position.z= - Position.z + 184;
       if(Position.z>92){Position.z= - Position.z + 184;}
     }
    if((Position.x>493||Position.y>490||Position.x<-490||Position.y<-490)&&Position.z<84){Draw=(Position.x-500)*(Position.x-500)+(Position.y-500)*(Position.y-500);}
    if(Position.z<60&&(Position.x>100||Position.x<-100||Position.y<-100||Position.y>100)){down=1.0;}
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(Position, 1.0f);
    TexCoord = vec2(texCoord.y, texCoord.x);
}