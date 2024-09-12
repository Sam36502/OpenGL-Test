#version 450

in vec3 frag_colour;
in vec2 frag_uv;
in vec4 gl_FragCoord;

layout(location = 0) out vec4 diffuse_clr;

uniform sampler2D tex;

void main() {
	vec4 tex_clr = texture2D(tex, frag_uv);
	diffuse_clr = vec4( tex_clr.rgb, 1.0);
}