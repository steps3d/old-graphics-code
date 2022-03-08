#version 410 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;		// eye position
uniform vec3 light;

layout(location = 0) in vec4  pos;			// position in xyz
layout(location = 1) in	vec3  normal;


layout (location = 0) out vec3  n;
layout (location = 1) out vec3  v;
layout (location = 2) out vec3  l;
layout (location = 3) out vec3  h;

void main(void)
{
	vec4 	p = mv * vec4 ( pos.xyz, 1.0 );
	
	gl_Position  = proj * p;
	
	n            = normalize ( nm * normal );
	v            = normalize ( eye - p.xyz );					// vector to the eye
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	

}

