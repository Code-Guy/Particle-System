layout(location = 0) in vec3 Pos;//位置
layout(location = 1) in float Size;//大小
layout(location = 2) in int TileIndex;//贴图编号

uniform mat4 MV;//模型视图矩阵

out float G_Size;
out int G_TileIndex;

void main()                     
{
	G_Size = Size;
	G_TileIndex = TileIndex;

    gl_Position = MV * vec4(Pos, 1.0);
}