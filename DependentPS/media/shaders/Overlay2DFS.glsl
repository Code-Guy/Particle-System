// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform float Alpha = 1.0f;

void main()
{
	color = texture2D( myTextureSampler, vec2(UV.x, 1 - UV.y) );
	color.a *= Alpha;
}