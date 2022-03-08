//
// Bezier qubic patch
//

-- vertex

#version 410 core

uniform mat4 proj;
uniform mat4 mv;

in vec3 position;

void main(void)
{
	gl_Position = proj * mv * vec4 ( position, 1.0 );
}

-- tesscontrol

#version 410 core

uniform int inner;
uniform int outer;

layout(vertices = 16) out;

void main(void)
{
	gl_TessLevelInner [0] = inner;
	gl_TessLevelInner [1] = inner;
	gl_TessLevelOuter [0] = outer;
	gl_TessLevelOuter [1] = outer;
	gl_TessLevelOuter [2] = outer;
	gl_TessLevelOuter [3] = outer;

	gl_out [gl_InvocationID].gl_Position = gl_in [gl_InvocationID].gl_Position;
}

-- tesseval

#version 410 core

layout(quads, equal_spacing) in;

void main(void)
{
	float	x = gl_TessCoord.x;					// u,v coordinates for Bezier patch
	float	y = gl_TessCoord.y;
	vec4	u = vec4 ( 1.0, x, x*x, x*x*x );
	vec4	v = vec4 ( 1.0, y, y*y, y*y*y );
	mat4	b = mat4 (  1,  0,  0, 0,
	                   -3,  3,  0, 0,
					    3, -6,  3, 0,
					   -1,  3, -3, 1 );
					   
	vec4	bu = b * u;					// vector or Bernstein polynoms at u, bu.x is B0(u)
	vec4	bv = b * v;					// vector or Bernstein polynoms at v

//vec4	bu = vec4 ( 1-3*x+3*x*x-x*x*x, 3*x-6*x*x+3*x*x*x, 3*x*x-3*x*x*x, x*x*x );
//vec4	bv = vec4 ( 1-3*y+3*y*y-y*y*y, 3*y-6*y*y+3*y*y*y, 3*y*y-3*y*y*y, y*y*y );

	vec4 p00 = gl_in [ 0].gl_Position;
	vec4 p10 = gl_in [ 1].gl_Position;
	vec4 p20 = gl_in [ 2].gl_Position;
	vec4 p30 = gl_in [ 3].gl_Position;
	vec4 p01 = gl_in [ 4].gl_Position;
	vec4 p11 = gl_in [ 5].gl_Position;
	vec4 p21 = gl_in [ 6].gl_Position;
	vec4 p31 = gl_in [ 7].gl_Position;
	vec4 p02 = gl_in [ 8].gl_Position;
	vec4 p12 = gl_in [ 9].gl_Position;
	vec4 p22 = gl_in [10].gl_Position;
	vec4 p32 = gl_in [11].gl_Position;
	vec4 p03 = gl_in [12].gl_Position;
	vec4 p13 = gl_in [13].gl_Position;
	vec4 p23 = gl_in [14].gl_Position;
	vec4 p33 = gl_in [15].gl_Position;	

	gl_Position = bu.x * (bv.x * p00 + bv.y * p01 + bv.z * p02 + bv.w * p03) +
                  bu.y * (bv.x * p10 + bv.y * p11 + bv.z * p12 + bv.w * p13) +
                  bu.z * (bv.x * p20 + bv.y * p21 + bv.z * p22 + bv.w * p23) +
                  bu.w * (bv.x * p30 + bv.y * p31 + bv.z * p32 + bv.w * p33);
}

-- fragment

#version 410 core

out vec4 color;

void main(void)
{
	color = vec4(1.0);
}

