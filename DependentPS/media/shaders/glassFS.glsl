uniform samplerCube CubemapTex;

in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;

out vec4 fragColor;

uniform float Alpha;

void main(void)
{
	vec4 refractionColor = texture(CubemapTex, normalize(v_refraction));
	vec4 reflectionColor = texture(CubemapTex, normalize(v_reflection));
		
	fragColor = mix(refractionColor, reflectionColor, v_fresnel);
	fragColor.a *= Alpha;
}