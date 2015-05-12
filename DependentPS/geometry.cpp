#include "geometry.h"
#include "math3d.h"

using namespace std;
using namespace ork;

vector<vec3f> VBO::vertices;
vector<vec2f> VBO::uvs;
vector<vec3f> VBO::normals;

AABB::AABB(float xMin, float xMax,
	float yMin, float yMax,
	float zMin, float zMax, 
	int index)
	: xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax), zMin(zMin), zMax(zMax), index(index)
{
	
}

AABB::~AABB()
{

}

bool operator<(const AABB &lhs, const AABB &rhs)
{
	return lhs.index < rhs.index;
}

bool operator!=(const AABB &lhs, const AABB &rhs)
{
	return !(Math3D::IsFloatEqual(lhs.xMin, rhs.xMin) && Math3D::IsFloatEqual(lhs.xMax, rhs.xMax) &&
		Math3D::IsFloatEqual(lhs.yMin, rhs.yMin) && Math3D::IsFloatEqual(lhs.yMax, rhs.yMax) &&
		Math3D::IsFloatEqual(lhs.zMin, rhs.zMin) && Math3D::IsFloatEqual(lhs.zMax, rhs.zMax));
}

void AABB::Inverse()
{
	xMin = yMin = zMin = INFINITY_NUM;
	xMax = yMax = zMax = -INFINITY_NUM;
}

void AABB::TranslateSelf(vec3f offset)
{
	xMin += offset.x;
	xMax += offset.x;

	yMin += offset.y;
	yMax += offset.y;

	zMin += offset.z;
	zMax += offset.z;
}

AABB AABB::Translate(vec3f offset) const
{
	return AABB(xMin + offset.x, xMax + offset.x,
		yMin + offset.y, yMax + offset.y,
		zMin + offset.z, zMax + offset.z);
}

void AABB::Draw(ork::ptr< ork::Mesh<ork::vec3f, unsigned int> > mesh) const
{
	mesh->addVertex(vec3f(xMin, yMin, zMin));
	mesh->addVertex(vec3f(xMax, yMin, zMin));
	mesh->addVertex(vec3f(xMax, yMin, zMin));
	mesh->addVertex(vec3f(xMax, yMin, zMax));
	mesh->addVertex(vec3f(xMax, yMin, zMax));
	mesh->addVertex(vec3f(xMin, yMin, zMax));
	mesh->addVertex(vec3f(xMin, yMin, zMax));
	mesh->addVertex(vec3f(xMin, yMin, zMin));
	mesh->addVertex(vec3f(xMin, yMin, zMin));
	mesh->addVertex(vec3f(xMin, yMax, zMin));
	mesh->addVertex(vec3f(xMin, yMax, zMin));
	mesh->addVertex(vec3f(xMax, yMax, zMin));
	mesh->addVertex(vec3f(xMax, yMax, zMin));
	mesh->addVertex(vec3f(xMax, yMax, zMax));
	mesh->addVertex(vec3f(xMax, yMax, zMax));
	mesh->addVertex(vec3f(xMin, yMax, zMax));
	mesh->addVertex(vec3f(xMin, yMax, zMax));
	mesh->addVertex(vec3f(xMin, yMax, zMin));
	mesh->addVertex(vec3f(xMin, yMax, zMax));
	mesh->addVertex(vec3f(xMin, yMin, zMax));
	mesh->addVertex(vec3f(xMax, yMax, zMax));
	mesh->addVertex(vec3f(xMax, yMin, zMax));
	mesh->addVertex(vec3f(xMax, yMax, zMin));
	mesh->addVertex(vec3f(xMax, yMin, zMin));
}

bool AABB::IsVertexIn(const vec3f &v) const
{
	return v.x > this->xMin && v.x < this->xMax &&
		v.y > this->yMin && v.y < this->yMax &&
		v.z > this->zMin && v.z < this->zMax;
}