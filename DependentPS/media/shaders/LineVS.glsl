layout(location = 0) in vec4 Vertex;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * Vertex;
}