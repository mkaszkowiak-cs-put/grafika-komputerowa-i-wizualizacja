#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex;
layout (location=1) in vec4 normal;
layout (location=2) in vec2 texCoord;


//Zmienne interpolowane
out vec2 i_tc;
out vec3 Normal;
out vec3 FragPos;

void main(void) {
    gl_Position=P*V*M*vertex;

    i_tc = texCoord;

    // expensive computation wise, but required bcuz we're rotating and scaling our model
    Normal = mat3(transpose(inverse(M))) * normal.xyz;  
    FragPos = vertex.xyz;
}
