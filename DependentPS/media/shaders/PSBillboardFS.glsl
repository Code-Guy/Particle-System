in vec4 F_Color;
in float F_TimeRatio;
in vec2 F_UV;

flat in float G_Alpha;

out vec4 FragmentColor;

uniform sampler2D ParticleTex;//纹理图
uniform vec3 UVAniamtion;//纹理图的长宽分段数+循环数
uniform vec2 TexSize;//纹理图大小
uniform int IsBlur = 0;//是否采样

vec2 TC_Offset[9];//采样的偏移数组
float Gaussian[9] = float[](0.0947416f, 0.118318f, 0.0947416f, 
                    0.118318f, 0.147761f, 0.118318f,
                    0.0947416f, 0.118318f, 0.0947416f);

void CalcTexOffset();
vec4 CalcSampleColor(vec2 uv, int w1, int h1, int w2, int h2, float fadeValue);

void main()
{
	int tileX = int(UVAniamtion.x);
	int tileY = int(UVAniamtion.y);
	int cycle = int(UVAniamtion.z);

	float fN = mod(F_TimeRatio * tileX * tileY * cycle, tileX * tileY);
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

	CalcTexOffset();

	if(IsBlur == 1)
	{
		FragmentColor = vec4(0, 0, 0, 0.0);

		for (int i = 0; i < 9; ++i)
    	{
        	//获得采样数据
        	FragmentColor += CalcSampleColor(F_UV + TC_Offset[i], w1, h1, w2, h2, fadeValue) * Gaussian[i];
    	}
	}
	else if(IsBlur == 0)
	{
		FragmentColor = CalcSampleColor(F_UV, w1, h1, w2, h2, fadeValue);
	}
}

void CalcTexOffset()
{
	float xIn = 5.0f / TexSize.x;
	float yIn = 5.0f / TexSize.y;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			TC_Offset[3 * i + j][0] = (i - 1.0f) * xIn;
			TC_Offset[3 * i + j][1] = (j - 1.0f) * yIn;
		}
	}
}

vec4 CalcSampleColor(vec2 uv, int w1, int h1, int w2, int h2, float fadeValue)
{
	vec2 dividedUV = uv / UVAniamtion.xy;
	vec2 uv1 = dividedUV + vec2(w1, h1) / UVAniamtion.xy;
	vec2 uv2 = dividedUV + vec2(w2, h2) / UVAniamtion.xy;

	return 2.0f * mix(texture2D(ParticleTex, uv1), texture2D(ParticleTex, uv2), fadeValue) * F_Color * vec4(1, 1, 1, G_Alpha);
}