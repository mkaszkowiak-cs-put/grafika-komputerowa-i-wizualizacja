#version 330

//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;
layout (location=2) in vec2 texCoord;

out vec2 i_tc;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main()
{
    gl_Position=P*V*M*vertex;
    i_tc = texCoord;
}