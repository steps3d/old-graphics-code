//
// Phong tessellation
//

-- vertex

#version 420 core

uniform mat4 mv;
uniform mat3 nm;

in  vec3 position;
in  vec3 normal;
out vec3 n;

void main(void)
{
	gl_Position = mv * vec4 ( position, 1.0 );
	n           = normalize ( nm * normal );
}

-- tesscontrol

#version 420 core

uniform int inner;
uniform int outer;

in  vec3 n [];

patch out vec3 b_300;		// Bezier patch coefficients for vertices
patch out vec3 b_030;
patch out vec3 b_003;
patch out vec3 b_210;
patch out vec3 b_120;
patch out vec3 b_021;
patch out vec3 b_012;
patch out vec3 b_102;
patch out vec3 b_201;
patch out vec3 b_111;

patch out vec3 n_200;		// Bezier  coefficients for normals
patch out vec3 n_020;
patch out vec3 n_002;
patch out vec3 n_110;
patch out vec3 n_011;
patch out vec3 n_101;

layout(vertices = 3) out;

float w ( int i, int j )
{
	return dot ( gl_in [j].gl_Position.xyz - gl_in [i].gl_Position.xyz, n [i] );
}

vec3 bb ( int i, int j )
{
	return (2.0*gl_in [i].gl_Position.xyz + gl_in [j].gl_Position.xyz - w ( i, j ) * n [i]) / 3.0;
}

float lengthSq ( const vec3 v )
{
	return dot ( v, v );
}

vec3 nn ( int i, int j )
{
	vec3	dv  = gl_in [j].gl_Position.xyz - gl_in [i].gl_Position.xyz;
	vec3	vij = 2.0*( dv, n [i] + n [j] ) / lengthSq ( dv );
	vec3	nij = n [i] + n [j] - vij * dv;
	
	return normalize ( nij );
}

void main(void)
{
	gl_TessLevelInner [0] = inner;
	gl_TessLevelInner [1] = inner;
	gl_TessLevelOuter [0] = outer;
	gl_TessLevelOuter [1] = outer;
	gl_TessLevelOuter [2] = outer;
	gl_TessLevelOuter [3] = outer;

	gl_out [gl_InvocationID].gl_Position = gl_in [gl_InvocationID].gl_Position;
	
	b_300 = gl_in [0].gl_Position.xyz;
	b_030 = gl_in [1].gl_Position.xyz;
	b_003 = gl_in [2].gl_Position.xyz;
	
	b_210 = bb ( 0, 1 );
	b_120 = bb ( 1, 0 );
	b_021 = bb ( 1, 2 );
	b_012 = bb ( 2, 1 );
	b_102 = bb ( 2, 0 );
	b_201 = bb ( 0, 2 );
	b_111 = 1.5*(b_210 + b_120 + b_021 + b_012 + b_102 + b_201) / 6.0 -
	        0.5*(gl_in [0].gl_Position.xyz + gl_in [1].gl_Position.xyz + gl_in [2].gl_Position.xyz) / 3.0;
			  
	n_200 = n [0];
	n_020 = n [1];
	n_002 = n [2];
	n_110 = nn ( 0, 1 );
	n_011 = nn ( 1, 2 );
	n_101 = nn ( 2, 0 );
}

-- tesseval

#version 420 core

uniform mat4 proj;

in vec3 nn [];
out vec3 normal;

patch in vec3 b_300;		// Bezier patch coefficients for vertices
patch in vec3 b_030;
patch in vec3 b_003;
patch in vec3 b_210;
patch in vec3 b_120;
patch in vec3 b_021;
patch in vec3 b_012;
patch in vec3 b_102;
patch in vec3 b_201;
patch in vec3 b_111;

patch in vec3 n_200;		// Bezier  coefficients for normals
patch in vec3 n_020;
patch in vec3 n_002;
patch in vec3 n_110;
patch in vec3 n_011;
patch in vec3 n_101;

layout(triangles, equal_spacing) in;

void main(void)
{
	float	u = gl_TessCoord.x;
	float	v = gl_TessCoord.y;
	float	w = gl_TessCoord.z;
	
	vec3	p = (b_300*u + 3.0*b_210*v + 3.0*b_201*w)*u*u + (b_030*v + 3.0*b_120*u + 3.0*b_021*w)*v*v + (b_003*w + 3.0*b_012*v + 3.0*b_102*u)*w*w + 6.0*b_111*u*v*w;
	vec3	n = n_200*u*u + n_020*v*v + n_002*w*w + 2.0*n_110*u*v + 2.0*n_011*v*w + 2.0*n_101*u*w;

	gl_Position = proj * vec4 ( p, 1.0 );
	normal      = normalize ( n );
}

-- fragment

#version 420 core

out vec4 color;

void main(void)
{
	color = vec4(1.0);
}

