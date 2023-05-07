#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=2) in vec2 texCoord;


//Zmienne interpolowane
out vec2 i_tc;

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;
}
