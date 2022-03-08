#version 410 core

in	vec3 n;
in	vec3 v;
in	vec3 l;
in	vec3 h;
out 	vec4 color;

void main(void)
{

	const vec4	diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );
	const vec4	specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float	specPower = 30.0;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec4	diff = diffColor * max ( dot ( n2, l2 ), 0.1 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower );

	color = diff + spec;
}
