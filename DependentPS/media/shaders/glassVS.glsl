layout(location = 0) in vec4 Vertex;
layout(location = 2) in vec3 Normal;

// Indices of refraction
const float Air = 0.5;
const float Glass = 1.51714;

// Air to glass ratio of the indices of refraction (Eta)
const float Eta = Air / Glass;
 
// see http://en.wikipedia.org/wiki/Refractive_index Reflectivity
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

uniform mat4 VP;
uniform mat4 M;
uniform mat4 N;
uniform vec3 CameraPos;

out vec3 v_reflection;
out vec3 v_refraction;
out float v_fresnel;

void main(void)
{
	// We calculate in world space.
	
	vec4 vertex = M * Vertex;
	
	vec3 incident = normalize(vec3(vertex) - CameraPos);

	// Assume incoming normal is normalized.
	vec3 normal = mat3(N) * Normal;
	
	v_refraction = refract(incident, normal, Eta);
	v_reflection = reflect(incident, normal);
			
	// see http://en.wikipedia.org/wiki/Schlick%27s_approximation
	v_fresnel = R0 + (1.0 - R0) * pow((1.0 - dot(-incident, normal)), 5.0);
		
	gl_Position = VP * vertex;
}