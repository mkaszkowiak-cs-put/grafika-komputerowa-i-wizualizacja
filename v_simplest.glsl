#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=2) in vec2 texCoord;

in vec4 normal;

out vec4 l;
out vec4 n;
out vec4 v;

//Zmienne interpolowane
out vec2 i_tc;

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;

    vec4 lp = vec4(5, 5, -6, 1); // zrodlo swiatla w przestrzeni swiata?
    l = normalize(V*lp - V*M*vertex);
    n = normalize(V*M*normal);
    v = normalize(vec4(1,0,1,1) - V*M*vertex); // to jest chyba kierunek swiatla?
}
