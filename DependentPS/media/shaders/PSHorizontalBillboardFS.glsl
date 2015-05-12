in vec4 F_Color;
in float F_TimeRatio;
in vec2 F_UV;

out vec4 FragmentColor;

uniform sampler2D ParticleTex;//纹理图
uniform vec3 UVAniamtion;//纹理图的长宽分段数+循环数

void main()
{
	int tileX = int(UVAniamtion.x);
	int tileY = int(UVAniamtion.y);
	int cycle = int(UVAniamtion.z);

	float fN = F_TimeRatio * tileX * tileY * cycle;
	int n = int(fN);

	float fadeValue = fN - n;
	
	int w1 = int(n % tileX);
	int h1 = int(n / tileX);

	int w2 = w1;
	int h2 = h1;

	if(++n < tileX * tileY)
	{
		w2 = int(n % tileX);
		h2 = int(n / tileX);
	}

	vec2 dividedUV = F_UV / UVAniamtion.xy;
	vec2 uv1 = dividedUV + vec2(w1, h1) / UVAniamtion.xy;
	vec2 uv2 = dividedUV + vec2(w2, h2) / UVAniamtion.xy;

	FragmentColor = 2.0f * mix(texture2D(ParticleTex, uv1), texture2D(ParticleTex, uv2), fadeValue) * F_Color;
}