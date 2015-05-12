layout(points) in;//输入一个点
layout(triangle_strip, max_vertices = 4) out;//输出一个4个点的triangle strip（四边形）

in float G_Size[];
in int G_TileIndex[];

out vec2 F_UV;//输出到FS的UV
flat out int F_TileIndex;

uniform mat4 P;//输入的投影矩阵

void EmitParticle(vec4 center, float alpha);

void main()
{
	vec4 center = gl_in[0].gl_Position;
	F_TileIndex = G_TileIndex[0];
	EmitParticle(center, 1);
}

void EmitParticle(vec4 center, float alpha)
{
	float size = G_Size[0];

	float cosVal = cos(45);
	float sinVal = sin(45);

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