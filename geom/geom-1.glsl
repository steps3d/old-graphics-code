//
// Shaders to produce billboards from points
//

-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;

in  vec4  posSize;		// position in xyz, size in w
out float size;

void main(void)
{
	gl_Position  = proj * mv * vec4 ( posSize.xyz, 1.0 );
	size         = 0.5 * posSize.w;
}

-- geometry

#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

uniform vec3 up;
uniform vec3 right;

in float size [];

out float clr;

void main ()
{
	vec3	u = up * size [0];
	vec3	r = right * size [0];
	vec3	p = gl_in [0].gl_Position.xyz;
	float	w = gl_in [0].gl_Position.w;
	
    gl_Position = vec4 ( p - u - r, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p - u + r, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p + u + r, w );
    EmitVertex   ();
    EndPrimitive ();				// 1st triangle
	
    gl_Position = vec4 ( p + u + r, w );
    EmitVertex   ();
	
    gl_Position = vec4 ( p + u - r, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p - u - r, w );
    EmitVertex   ();
    EndPrimitive ();				// 2nd triangle
}

-- fragment

#version 330 core

in float clr;
out vec4 color;

void main(void)
{
	color = vec4 ( 1.0 );
}
