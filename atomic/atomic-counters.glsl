-- vertex

#version 420 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat3 nm;
uniform	vec3 eye;
uniform vec3 light;

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 normal;

out vec3 n;
out vec3 v;
out vec3 l;
out	vec3 h;

void main(void)
{
	vec4	p = mv * vec4 ( pos, 1.0 );

	v           = normalize ( eye - p.xyz );
	n           = normalize ( nm * normal );
	l            = normalize ( light - p.xyz );
	h            = normalize ( l + v );	
	gl_Position = proj * p;
}

-- fragment

#version 420 core

uniform	float		scale;

in vec3 n;
in vec3 v;
in vec3 l;
in vec3 h;

out vec4 color;

layout(binding=0, offset=0)  uniform atomic_uint	c0;
layout(binding=0, offset=4)  uniform atomic_uint	c1;
layout(binding=0, offset=8)  uniform atomic_uint	c2;
layout(binding=0, offset=12) uniform atomic_uint	total;

const vec3 lum = vec3 ( 0.27, 0.67, 0.06 );
const vec4 clr = vec4 ( 0.902, 0.694, 0.49, 1.0 );

void main (void)
{
	const vec4	specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
	const float	specPower = 70.0;

	vec3	n2   = normalize ( n );
	vec3	l2   = normalize ( l );
	vec3	h2   = normalize ( h );
	vec4	diff = vec4 ( max ( dot ( n2, l2 ), 0.0 ) + 0.3 );
	vec4	spec = specColor * pow ( max ( dot ( n2, h2 ), 0.0 ), specPower );
		
	atomicCounterIncrement ( total );			// total # of fragments
	
	color = diff * clr + spec;

	float	l = dot ( lum, color.rgb );
	
	if ( l < 0.5 )
		atomicCounterIncrement ( c0 );
	else
	if ( l < 1.0 )
		atomicCounterIncrement ( c1 );
	else
		atomicCounterIncrement ( c2 );
}
