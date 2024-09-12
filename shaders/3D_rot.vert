#version 450

layout(location = 0) in vec3 pos_3D;
layout(location = 1) in vec3 vert_colour;
layout(location = 2) in vec2 vert_uv;

out vec4 gl_Position;
out vec3 frag_colour;
out vec2 frag_uv;

layout(location = 0) uniform float angle;

void main() {
	mat2 rot;
	rot[0] = vec2( cos(angle), -sin(angle) );
	rot[1] = vec2( sin(angle), cos(angle) );

	vec2 new_xz = rot * pos_3D.xz;
	gl_Position = vec4(new_xz.x, pos_3D.y, new_xz.y, 1.0);

	frag_colour = vert_colour;
	frag_uv = vert_uv;
}