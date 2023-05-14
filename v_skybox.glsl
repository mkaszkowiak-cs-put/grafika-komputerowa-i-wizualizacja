#version 400

in vec3 vp;
uniform mat4 P, V;
out vec3 texcoords;

void main() {
  texcoords = vp;
  vec4 pos = P * V * vec4(vp, 1.0);
  gl_Position = pos.xyww; /* sets z = 1 */	
}