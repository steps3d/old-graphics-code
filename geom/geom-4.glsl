-- vertex

#version 330 core

uniform mat4 proj;
uniform mat4 mv;
uniform mat4 nm;
uniform	vec3 eye;		// eye position

in  vec3  pos;			// position in xyz, size in w
in	vec3  normal;
out vec3  n;
out vec3  v;

void main(void)
{
	vec4 	p = mv * vec4 ( pos, 1.0 );
	
	gl_Position  = p;
	n            = normalize ( (nm * vec4 ( normal, 0.0 )).xyz );
	v            = normalize ( vec3 ( eye ) - p.xyz );					// vector to the eye
}


-- geometry

#version 330 core

#define EPS	0.001

layout (triangles_adjacency) in;
layout (triangle_strip, max_vertices = 12) out;

uniform mat4 proj;
uniform float finSize;

in  vec3 n [];
in  vec3 v [];
out vec4 clr;

void	emitEdge ( int i0, int i1 )
{
		gl_Position = proj * gl_in [i0].gl_Position;
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * ( gl_in [i0].gl_Position + finSize * vec4 ( n [i0], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * gl_in [i1].gl_Position;
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		EndPrimitive ();
			
		gl_Position = proj * ( gl_in [i0].gl_Position + finSize * vec4 ( n [i0], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * ( gl_in [i1].gl_Position + finSize * vec4 ( n [i1], 0.0 ) );
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		
		gl_Position = proj * gl_in [i1].gl_Position;
		clr         = vec4 ( 0.0, 1.0, 0.0, 1.0 );
		EmitVertex ();
		EndPrimitive ();
}

vec3	calcNormal ( int i0, int i1, int i2 )
{
	vec3	va = gl_in [i1].gl_Position.xyz - gl_in [i0].gl_Position.xyz;
	vec3	vb = gl_in [i2].gl_Position.xyz - gl_in [i0].gl_Position.xyz;
	
	return normalize ( cross ( va, vb ) );
}
	
void main ()
{
											// emit source triangle
    for ( int i = 0; i < 3; i++ )
    {
        gl_Position = proj * gl_in [2*i].gl_Position;
		clr         = vec4 ( 1.0 );
			
        EmitVertex ();
    }

    EndPrimitive ();
	
	vec3	n0 = calcNormal ( 0, 2, 4 );	// normal for triangle itself
	
	if ( dot ( n0, v [0] ) < EPS )			// front-facing
	{
		vec3	n1 = calcNormal ( 1, 2, 0 );
		vec3	n2 = calcNormal ( 3, 4, 2 );
		vec3	n3 = calcNormal ( 5, 0, 4 );
											// check other triangles for back-facing
		if ( dot ( n1, v [1] ) >= -EPS )
			emitEdge ( 0, 2 );
			
		if ( dot ( n2, v [3] ) >= -EPS )
			emitEdge ( 2, 4 );
			
		if ( dot ( n3, v [5] ) >= -EPS )
			emitEdge ( 0, 4 );
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
