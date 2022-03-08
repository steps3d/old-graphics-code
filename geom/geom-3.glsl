-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat4 nm;
uniform	vec3 eye;		// eye position

in  vec3  pos;		// position in xyz, size in w
in	vec3  normal;
out vec3  n;
out vec3  v;
out float s;

void main(void)
{
	vec3 	p = vec3 ( mv * vec4 ( pos, 1.0 ) );
	
	gl_Position  = mv * vec4 ( pos, 1.0 );
	n            = (nm * vec4 ( normal, 0.0 )).xyz;
	v            = normalize ( vec3 ( eye ) - p );					// vector to the eye
	s            = dot ( n, v );
}


-- geometry

#version 330 core

#define EPS	0.001

layout (triangles) in;
layout (triangle_strip, max_vertices = 9) out;

uniform mat4 proj;
uniform float finSize;

in  vec3 n [];
in  vec3 v [];
out vec4 clr;

int signWithTolerance ( float v )
{
	if ( v > EPS )
		return 1;
		
	if ( v < -EPS )
		return -1;
		
	return 0;
}

int addPoint ( float f0, int i0, float f1, int i1, inout vec4 points [4], inout vec3 normals [4], int count )
{
	float	t = f1 / ( f1 - f0 );		// parameter across edge, t*f0 + (1-t)*f1 = 0
	
	points  [count]   = mix ( gl_in [i1].gl_Position, gl_in [i0].gl_Position, t );
	normals [count++] = finSize * normalize ( mix ( n [i1], n [i0], t ) );
	
	return count;
}

void main ()
{
	float f0 = dot ( v [0], n [0] );
	float f1 = dot ( v [1], n [1] );
	float f2 = dot ( v [2], n [2] );	
	int	  s0 = signWithTolerance ( f0 );
	int	  s1 = signWithTolerance ( f1 );
	int	  s2 = signWithTolerance ( f2 );
	
								// emit source triangle
    for ( int i = 0; i < 3; i++ )
    {
        gl_Position = proj * gl_in [i].gl_Position;
		clr         = vec4 ( 1.0 );
			
        EmitVertex ();
    }

    EndPrimitive ();
								// now check for fins
								// quick exit for common cases
	if ( s0 == 1 && s1 == 1 && s2 == 1 )
		return;
		
	if ( s0 == -1 && s1 == -1 && s2 == -1 )
		return;

	bool	on01 = s0 * s1 <= 0;
	bool	on02 = s0 * s2 <= 0;
	bool	on12 = s1 * s2 <= 0;
			
					// locate edges that contain cotour points
	vec4	points  [4];
	vec3	normals [4];
	int		count = 0;
	
	if ( on01 )
		count = addPoint ( f0, 0, f1, 1, points, normals, count );
			
	if ( on02 )
		count = addPoint ( f0, 0, f2, 2, points, normals, count );
			
	if ( on12 )
		count = addPoint ( f1, 1, f2, 2, points, normals, count );
	
	if ( count >= 2 )		// emit quad from store edge
	{
		gl_Position = proj * points [0];
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * ( points [0] + vec4 ( normals [0], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * points [1];
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		EndPrimitive ();
			
		gl_Position = proj * ( points [0] + vec4 ( normals [0], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * ( points [1] + vec4 ( normals [1], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * points [1];
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		EndPrimitive ();
	}
}

-- fragment

#version 330 core

in  vec4 clr;
out vec4 color;

void main(void)
{
	color = clr;
}
