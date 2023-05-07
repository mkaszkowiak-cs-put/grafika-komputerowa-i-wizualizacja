#version 330


uniform sampler2D tex;

out vec4 pixelColor;

//Zmienne interpolowane
in vec2 i_tc;

void main(void) {
	//pixelColor = vec4(1,1,1,1);
	pixelColor=texture(tex,i_tc);
}
