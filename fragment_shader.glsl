#version 330


in vec3 FragPos;
in vec3 Normal;
in vec2 i_tc; // koordynaty tektury

out vec4 pixelColor;

uniform sampler2D tex;
uniform vec3 lightPos;
uniform vec3 viewPos;


void main(void) {
    vec3 color = texture(tex, i_tc).rgb;
	
    // ambient
    vec3 ambient = 0.3 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    pixelColor = vec4(ambient + diffuse + specular, 1.0);
}
