
-- vertex

#version 330 core

uniform mat4 proj;

in vec3 position;
in vec3 color;

out vec3 fragmentColor;

void main(void)
{
	gl_Position   = proj * vec4(position, 1.0);
	fragmentColor = color;
}

-- fragment

#version 330 core

in vec3 fragmentColor;

out vec4 color;

void main(void)
{
	color = vec4(fragmentColor, 1.0);
}
