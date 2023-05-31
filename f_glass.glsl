#version 330 core
out vec4 pixelColor;

in vec2 i_tc;

uniform sampler2D tex;

void main()
{
    vec4 texColor = texture(tex, i_tc);
    pixelColor = texColor;
}