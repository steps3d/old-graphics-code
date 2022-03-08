#version 410 core

layout (location = 0) in	vec3 n;
layout (location = 1) in	vec3 v;
layout (location = 2) in	vec3 l;
layout (location = 3) in	vec3 h;

out vec4 color;

void main(void)
{
	const	vec3  surfaceColor = vec3 ( 0.75, 0.75, 0.75 );
	const	vec3  warmColor    = vec3 ( 0.6, 0.6, 0.0 );
	const	vec3  coolColor    = vec3 ( 0.0, 0.0, 0.6 );
	const	float diffuseWarm  = 0.45;
	const	float diffuseCool  = 0.45;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	v2   = normalize ( h );
	vec3	r    = normalize ( reflect ( -l2, n2 ) );
	vec3	cool    = min ( coolColor + diffuseCool * surfaceColor, 1.0 );
	vec3	warm    = min ( warmColor + diffuseWarm * surfaceColor, 1.0 );
	vec3	final   = mix ( cool, warm, max(0,dot(n2,l2)));

	float	spec = pow ( max ( dot ( r, v2 ), 0.0 ), 32.0 );

	color = vec4 ( min ( final + spec, 1.0 ), 1.0 );
}
