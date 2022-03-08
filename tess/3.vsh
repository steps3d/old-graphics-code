#version 410 core

uniform mat4 proj;
uniform mat4 mv;

in vec3 position;
in vec3 color;

out vec3 fragmentColor;

void main(void)
{
	gl_Position   = proj * mv * vec4 ( position, 1.0 );
	fragmentColor = vec3 ( 1.0 );
}
