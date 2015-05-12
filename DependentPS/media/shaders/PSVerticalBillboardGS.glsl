layout(points) in;//输入一个点
layout(triangle_strip, max_vertices = 4) out;//输出一个4个点的triangle strip（四边形）

in vec4 G_Color[];//输入的颜色
in vec3 G_Size_Angle_TimeRatio[];//输入的大小——旋转角度——时间百分比三元组

out vec4 F_Color;//输出到FS的颜色
out float F_TimeRatio;//输出到FS的时间百分比
out vec2 F_UV;//输出到FS的UV

uniform mat4 MVP;//模型投影视图矩阵

void main()
{
    F_Color = G_Color[0];
	F_TimeRatio = G_Size_Angle_TimeRatio[0].z;

	vec3 center = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;

	float cosVal = cos(45 - G_Size_Angle_TimeRatio[0].y);
	float sinVal = sin(45 - G_Size_Angle_TimeRatio[0].y);

	vec4 v0 = vec4(center + vec3(1, 1, 0) * G_Size_Angle_TimeRatio[0].x, 1.0);	//1象限
    vec4 v1 = vec4(center + vec3(-1, 1, 0) * G_Size_Angle_TimeRatio[0].x, 1.0);//2象限
    vec4 v2 = vec4(center + vec3(-1, -1, 0) * G_Size_Angle_TimeRatio[0].x, 1.0);//3象限
   	vec4 v3 = vec4(center + vec3(1, -1, 0) * G_Size_Angle_TimeRatio[0].x, 1.0);//4象限

	//0--1--3--2三角形条带规定顺序
	gl_Position = MVP * v0;
	F_UV = vec2(1, 1);
	EmitVertex();

	gl_Position = MVP * v1;
	F_UV =  vec2(0, 1);
	EmitVertex();

	gl_Position = MVP * v3;
	F_UV =  vec2(1, 0);
	EmitVertex();

	gl_Position = MVP * v2;
	F_UV = vec2(0, 0);
	EmitVertex();

	EndPrimitive();
}