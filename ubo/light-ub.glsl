-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;		// eye position
uniform vec3 light;

in  vec3  pos;			// position in xyz
in	vec3  normal;
out vec3  n;
out vec3  v;
out vec3  l;
out	vec3  h;

void main(void)
{
	vec4 	p = mv * vec4 ( pos, 1.0 );
	
	gl_Position  = proj * p;
	n            = normalize ( nm * normal );
	v            = normalize ( eye - p.xyz );					// vector to the eye
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	
}

-- fragment

#version 330 core

uniform Lighting
{
	vec4	diffColor;
	vec4	specColor;
	float	specPower;
};

in	vec3 n;
in	vec3 v;
in	vec3 l;
in	vec3 h;

out vec4 color;

void main(void)
{
	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec4	diff = diffColor * max ( dot ( n2, l2 ), 0.1 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower );

	color = diff + spec;
}
