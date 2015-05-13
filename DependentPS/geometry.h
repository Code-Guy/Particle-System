#ifndef _GEOMETRY_H
#define  _GEOMETRY_H

#include <vector>
#include "ork\math\vec2.h"

#include "global.h"

#define INFINITY_AABB AABB(-INFINITY_NUM, INFINITY_NUM, -INFINITY_NUM, INFINITY_NUM, -INFINITY_NUM, INFINITY_NUM)

struct V2_UV
{
	float vx, vy, u, v;

	V2_UV(float vx = 0, float vy = 0,
		float u = 0, float v = 0) :
	vx(vx), vy(vy), u(u), v(v)
	{
	}
};

struct V_UV_N {

	float vx, vy, vz, u, v, nx, ny, nz;

	V_UV_N()
	{

	}

	V_UV_N(float vx, float vy, float vz,
		float u, float v,
		float nx, float ny, float nz) :
	vx(vx), vy(vy), vz(vz), u(u), v(v), nx(nx), ny(ny), nz(nz)
	{
	}

};

struct V_UV_N_T_B {

	float vx, vy, vz, u, v, nx, ny, nz, tx, ty, tz, bx, by, bz;

	V_UV_N_T_B()
	{

	}

	V_UV_N_T_B(float vx, float vy, float vz,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz, 
		float bx, float by, float bz) :
	vx(vx), vy(vy), vz(vz), u(u), v(v), nx(nx), ny(ny), nz(nz),
		tx(tx), ty(ty), tz(tz), bx(bx), by(by), bz(bz)
	{
	}

};

struct VBO
{
	static std::vector<ork::vec3f> vertices;
	static std::vector<ork::vec2f> uvs;
	static std::vector<ork::vec3f> normals;

	static void Clear()
	{
		vertices.clear();
		uvs.clear();
		normals.clear();
	}
};

struct Refer//顶点、纹理坐标、法向三个索引
{
	Refer(int v, int uv, int n) : v(v), uv(uv), n(n)
	{

	}

	int v;//点的索引
	int uv;//纹理坐标的索引
	int n;//法向的索引
};

struct Face
{
	std::vector<Refer> refers;//一个索引的链表
};

class AABB
{
public:
	AABB(float xMin = -INFINITY_NUM, float xMax = INFINITY_NUM,
		float yMin = -INFINITY_NUM, float yMax = INFINITY_NUM,
		float zMin = -INFINITY_NUM, float zMax = INFINITY_NUM,
		int index = -1);
	~AABB();

	friend bool operator<(const AABB &lhs, const AABB &rhs);
	friend bool operator!=(const AABB &lhs, const AABB &rhs);

	void Inverse();
	void TranslateSelf(ork::vec3f offset);
	AABB Translate(ork::vec3f offset) const;
	void Draw(ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > mesh) const;
	bool IsVertexIn(const ork::vec3f &v) const;
	bool IsAABBIn(const AABB &aabb) const;

	float xMin, xMax;
	float yMin, yMax;
	float zMin, zMax;

	int index;
};

#endif //_GEOMETRY_H