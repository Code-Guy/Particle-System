layout(points) in;//输入一个点
layout(triangle_strip, max_vertices = 24) out;//输出一个4个点的triangle strip（四边形）

in vec4 G_Color[];//输入的颜色
in vec3 G_Size_Angle_TimeRatio[];//输入的大小——旋转角度——时间百分比三元组
in vec3 G_TrailDir[];//输入的速度方向

out vec4 F_Color;//输出到FS的颜色
out float F_TimeRatio;//输出到FS的时间百分比
out vec2 F_UV;//输出到FS的UV

flat out float G_Alpha;

uniform mat4 P;//输入的投影矩阵
uniform int IsTrail = 0;//是否开启拖尾效果 0:不开启 1:开启

const int TrailNum = 5;

void EmitParticle(vec4 center, float alpha);

void main()
{
    F_Color = G_Color[0];
	F_TimeRatio = G_Size_Angle_TimeRatio[0].z;

	vec4 center = gl_in[0].gl_Position;

	if(IsTrail == 0)
	{
		EmitParticle(center, 1);
	}
	else if(IsTrail == 1)
	{
		for(int i=0; i<TrailNum; i++)
		{
			EmitParticle(center, 1 - 0.1 * i);
		}
	}
}

void EmitParticle(vec4 center, float alpha)
{
	float size = G_Size_Angle_TimeRatio[0].x;
	float angle = G_Size_Angle_TimeRatio[0].y;

	float cosVal = cos(45 - angle);
	float sinVal = sin(45 - angle);

	center.xy -= (G_TrailDir[0] * size * (1.0f - alpha) * 4).xy;//计算粒子的位置
	size *= alpha;//计算粒子的大小
	G_Alpha = pow(alpha, 5);//计算粒子的透明度

	vec4 v0 = vec4(center.xy + vec2(cosVal, sinVal) * size, center.z, 1.0);	//1象限
    vec4 v1 = vec4(center.xy + vec2(-sinVal, cosVal) * size, center.z, 1.0);	//2象限
    vec4 v2 = vec4(center.xy + vec2(-cosVal, -sinVal) * size, center.z, 1.0);  //3象限
   	vec4 v3 = vec4(center.xy + vec2(sinVal, -cosVal) * size, center.z, 1.0);  //4象限

	//0--1--3--2三角形条带规定顺序
	gl_Position = P * v0;
	F_UV = vec2(1, 1);
	EmitVertex();

	gl_Position = P * v1;
	F_UV =  vec2(0, 1);
	EmitVertex();

	gl_Position = P * v3;
	F_UV =  vec2(1, 0);
	EmitVertex();

	gl_Position = P * v2;
	F_UV = vec2(0, 0);
	EmitVertex();

	EndPrimitive();
}