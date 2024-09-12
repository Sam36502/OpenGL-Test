#version 450

in vec3 frag_colour;
in vec2 frag_uv;
in vec4 gl_FragCoord;

layout(location = 0) out vec4 diffuse_clr;

void main() {
	diffuse_clr = vec4(frag_uv.x, frag_uv.y, 0.0, 1.0);
}