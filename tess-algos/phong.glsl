//
// Phong tessellation
//

-- vertex

#version 410 core

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

#version 410 core

uniform int inner;
uniform int outer;

in  vec3 n  [];
out vec3 nn [];

layout(vertices = 3) out;

void main(void)
{
	gl_TessLevelInner [0] = inner;
	gl_TessLevelInner [1] = inner;
	gl_TessLevelOuter [0] = outer;
	gl_TessLevelOuter [1] = outer;
	gl_TessLevelOuter [2] = outer;
	gl_TessLevelOuter [3] = outer;

	gl_out [gl_InvocationID].gl_Position = gl_in [gl_InvocationID].gl_Position;
	nn     [gl_InvocationID]             = n     [gl_InvocationID];
}

-- tesseval

#version 410 core

uniform mat4 proj;

in vec3 nn [];

layout(triangles, equal_spacing) in;

void main(void)
{
	float	u = gl_TessCoord.x;
	float	v = gl_TessCoord.y;
	float	w = gl_TessCoord.z;
	
	vec4	p = u * gl_in [0].gl_Position + v * gl_in [1].gl_Position + w * gl_in [2].gl_Position;
	vec3	n = normalize ( u * nn [0] + v * nn [1] + w * nn [2] );

	vec4	p0 = p - dot ( p.xyz - gl_in [0].gl_Position.xyz, nn [0] ) * vec4 ( nn [0], 0.0 );
	vec4	p1 = p - dot ( p.xyz - gl_in [1].gl_Position.xyz, nn [1] ) * vec4 ( nn [1], 0.0 );
	vec4	p2 = p - dot ( p.xyz - gl_in [2].gl_Position.xyz, nn [2] ) * vec4 ( nn [2], 0.0 );
	
	gl_Position = proj * ( u * p0 + v * p1 + w * p2 );
}

-- fragment

#version 410 core

out vec4 color;

void main(void)
{
	color = vec4(1.0);
}

