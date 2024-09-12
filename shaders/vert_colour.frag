#version 450

in vec3 frag_colour;
in vec4 gl_FragCoord;

layout(location = 0) out vec4 diffuse_clr;

void main() {
	if (gl_FragCoord.x > 512.0) {
		vec3 inv = 1.0 - frag_colour;
		diffuse_clr = vec4(inv, 1.0);
	} else {
		diffuse_clr = vec4(frag_colour, 1.0);
	}

	//diffuse_clr = vec4(frag_colour, 1.0);
}