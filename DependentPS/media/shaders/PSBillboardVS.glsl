layout(location = 0) in vec3 Pos;//位置
layout(location = 1) in vec4 Color;//颜色
layout(location = 2) in vec3 Size_Angle_TimeRatio;//大小——旋转角度——时间百分比三元组
layout(location = 3) in vec3 VelocityDir;//速度方向
layout(location = 4) in float StretchScale;//沿速度方向上的拉伸比例

uniform mat4 MV;//模型视图矩阵
uniform mat4 N;//法向矩阵

out vec4 G_Color;//输出到GS的颜色
out vec3 G_Size_Angle_TimeRatio;//输出到GS的大小——旋转角度——时间百分比三元组
out vec3 G_TrailDir;//输出到GS的速度方向

void main()                     
{
	G_Color = Color;
	G_Size_Angle_TimeRatio = Size_Angle_TimeRatio;
	G_TrailDir = mat3(N) * VelocityDir;

    gl_Position = MV * vec4(Pos, 1.0);
}