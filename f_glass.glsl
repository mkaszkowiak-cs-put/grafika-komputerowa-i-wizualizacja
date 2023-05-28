#version 330 core
out vec4 pixelColor;

in vec2 i_tc;

uniform sampler2D tex;

void main()
{
    // pixelColor = = texture(tex, i_tc)

    vec3 texColorWithoutAlpha = texture(tex, i_tc).rgb;
    pixelColor = vec4(texColorWithoutAlpha, 0.3);
}