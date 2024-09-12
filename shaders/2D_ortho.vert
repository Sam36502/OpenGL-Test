#version 450

layout(location = 0) in vec2 pos_2D;
layout(location = 1) in vec3 vert_colour;
layout(location = 2) in vec2 vert_uv;

out vec4 gl_Position;
out vec3 frag_colour;
out vec2 frag_uv;

void main() {
	gl_Position = vec4(pos_2D.xy, 0, 1);

	frag_colour = vert_colour;
	frag_uv = vert_uv;
}