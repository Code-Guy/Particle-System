#ifdef _VERTEX_

layout(location = 0) in vec4 position;

out vec3 worldPos;

uniform mat4 screenToModelMat;

void main()
{
	vec4 worldPosTmp = screenToModelMat * vec4(position.xy, 0.9, 1.0);
	worldPos = worldPosTmp.xyz / worldPosTmp.w;
	gl_Position = vec4(position.xy, 0.0, 1.0);
}

#endif


#ifdef _FRAGMENT_

out vec3 fragmentColor;

in vec3 worldPos;

uniform samplerCube skyboxTex;
uniform vec3 cameraPos;

void main()
{
	vec3 worldDir = normalize(worldPos - cameraPos);
  	fragmentColor = texture(skyboxTex, worldDir.xzy).rgb;
}

#endif