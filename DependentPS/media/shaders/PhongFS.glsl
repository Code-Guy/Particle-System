// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 T_cameraspace;
in vec3 B_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace1;
in vec3 LightDirection_cameraspace2;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D SpecularTexture;
uniform sampler2D AmbientTexture;

uniform bool UseDiffuseMap = false;
uniform bool UseNormalMap = false;
uniform bool UseSpecularMap = false;
uniform bool UseAmbientMap = false;
uniform bool UseShadowMap = false;

uniform vec3 LightPosition_worldspace1;
uniform vec3 LightPosition_worldspace2;

uniform vec3 LightColor1;
uniform float LightPower1;

uniform vec3 LightColor2;
uniform float LightPower2;

uniform vec3 MaterialDiffuseColor;
uniform vec3 MaterialAmbientColor;
uniform vec3 MaterialSpecularColor;

uniform float Alpha;

vec3 DiffuseColor;
vec3 AmbientColor;
vec3 SpecularColor;

vec3 Normal_tangentspace;
vec3 EyeDirection_tangentspace;
vec3 LightDirection_tangentspace1;
vec3 LightDirection_tangentspace2;

vec3 GetLightColor(vec3 LightDirection, vec3 Normal, vec3 EyeDirection, vec3 LightPosition_worldspace, vec3 LightColor, float LightPower);

void main()
{
	vec2 newUV = vec2(UV.x, 1 - UV.y);

	// Material properties
	DiffuseColor = UseDiffuseMap ? texture2D( DiffuseTexture, newUV ).rgb : MaterialDiffuseColor;
	AmbientColor = UseAmbientMap ? texture2D( AmbientTexture, newUV ).rgb : MaterialAmbientColor * DiffuseColor;
	SpecularColor = UseSpecularMap ? texture2D( SpecularTexture, newUV ).rgb : MaterialSpecularColor;

	if(UseNormalMap)
	{
		mat3 ITBN = transpose(mat3(
        T_cameraspace,
        B_cameraspace,
        Normal_cameraspace
    	));

    	EyeDirection_tangentspace = ITBN * EyeDirection_cameraspace;
    	LightDirection_tangentspace1 = ITBN * LightDirection_cameraspace1;
    	LightDirection_tangentspace2 = ITBN * LightDirection_cameraspace2;

    	Normal_tangentspace = normalize(texture2D( NormalTexture, newUV ).rgb*2.0 - 1.0);
	}

	// Ambient : simulates indirect lighting
	color.rgb =  AmbientColor;

	if(UseNormalMap)
	{
		color.rgb += GetLightColor(LightDirection_tangentspace1, Normal_tangentspace, EyeDirection_tangentspace, LightPosition_worldspace1, LightColor1, LightPower1) +
					 GetLightColor(LightDirection_tangentspace2, Normal_tangentspace, EyeDirection_tangentspace, LightPosition_worldspace2, LightColor2, LightPower2);
	}
	else
	{
		color.rgb += GetLightColor(LightDirection_cameraspace1, Normal_cameraspace, EyeDirection_cameraspace, LightPosition_worldspace1, LightColor1, LightPower1) +
					 GetLightColor(LightDirection_cameraspace2, Normal_cameraspace, EyeDirection_cameraspace, LightPosition_worldspace2, LightColor2, LightPower2);
	}
		
		
	clamp(color.rgb, 0, 1);

	color.a = Alpha;
}

vec3 GetLightColor(vec3 LightDirection, vec3 Normal, vec3 EyeDirection, vec3 LightPosition_worldspace, vec3 LightColor, float LightPower)
{
	// Distance to the light
	float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	// Diffuse : "color" of the object
	return DiffuseColor * LightColor * LightPower * cosTheta +
		// Specular : reflective highlight, like a mirror
		SpecularColor * LightColor * LightPower * pow(cosAlpha,5);

}