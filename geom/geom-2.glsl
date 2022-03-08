-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform	vec3 eye;		// eye position

in  vec4  posSize;		// position in xyz, size in w
out float size;
out vec3  v;

void main(void)
{
	gl_Position  = proj * mv * vec4 ( posSize.xyz, 1.0 );
	size         = 0.5 * posSize.w;
	v            = -normalize ( gl_Position.xyz );
}

-- geometry

#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

uniform vec3 sun;		// sun direction

in float size [];
in vec3  v    [];

out float clr;

void main ()
{
	vec3	p  = gl_in [0].gl_Position.xyz;
	vec3	vn = v [0];
	vec3	up = normalize ( cross ( vn, sun ) ) * size [0];
	vec3	f  = normalize ( vn + sun ) * size [0];				// bisector of v and sun
	float	w  = gl_in [0].gl_Position.w;
	
    gl_Position = vec4 ( p - up - f, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p - up + f, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p + up + f, w );
    EmitVertex   ();
    EndPrimitive ();				// 1st triangle
	
    gl_Position = vec4 ( p + up + f, w );
    EmitVertex   ();
	
    gl_Position = vec4 ( p + up - f, w );
    EmitVertex ();
	
    gl_Position = vec4 ( p - up - f, w );
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
