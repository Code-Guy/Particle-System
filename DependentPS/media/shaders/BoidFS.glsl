in vec2 F_UV;
flat in int F_TileIndex;

out vec4 FragmentColor;

uniform sampler2D ParticleTex;//纹理图
uniform ivec2 UVTile;

void main()
{
	int w = F_TileIndex % UVTile.x;
	int h = F_TileIndex % UVTile.y;

	vec2 uv = (vec2(F_UV.x, 1 - F_UV.y) + vec2(w, h)) / UVTile;
	FragmentColor = texture2D(ParticleTex, uv);
}