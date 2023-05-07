#version 330


uniform sampler2D tex;

out vec4 pixelColor;

//Zmienne interpolowane
in vec4 l;
in vec4 n;
in vec4 v;
in vec2 i_tc; // koordynaty tektury

void main(void) {
	//pixelColor = vec4(1,1,1,1);
	//pixelColor=;

	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	vec4 r = reflect(-ml, mn);
	float nl = clamp(dot(ml, mn), 0, 1);
	float rv = clamp(dot(r, mv), 0, 1);
	rv = pow(rv, 50);

	pixelColor = texture(tex,i_tc); // * nl + rv; //  
}
