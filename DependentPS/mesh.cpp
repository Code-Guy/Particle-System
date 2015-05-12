#include "mesh.h"

using namespace std;
using namespace ork;

OrkMesh::OrkMesh(ptr< Mesh<V_UV_N_T_B, unsigned int> > mesh, Material material, AABB boundAABB, 
	ptr< Texture2D > diffuseTexture, ptr< Texture2D > normalTexture, ptr< Texture2D > specularTexture, ptr< Texture2D > ambientTexture,
	vec3f translate, vec3f rotate, vec3f scale)
	: mesh(mesh), material(material), boundAABB(boundAABB),
	diffuseTexture(diffuseTexture), normalTexture(normalTexture), specularTexture(specularTexture), ambientTexture(ambientTexture),
	translate(translate), rotate(rotate), scale(scale)
{
	
}

OrkMesh::~OrkMesh()
{

}

void CalcTB(vec3f &T, vec3f &B, const Face &face)
{
	vec3f v0 = VBO::vertices[face.refers[0].v];
	vec3f v1 = VBO::vertices[face.refers[1].v];
	vec3f v2 = VBO::vertices[face.refers[2].v];

	vec2f uv0 = VBO::uvs[face.refers[0].uv];
	vec2f uv1 = VBO::uvs[face.refers[1].uv];
	vec2f uv2 = VBO::uvs[face.refers[2].uv];

	vec3f deltaPos1 = v1 - v0;
	vec3f deltaPos2 = v2 - v0;

	vec2f deltaUV1 = uv1 - uv0;
	vec2f deltaUV2 = uv2 - uv0;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	T = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
	B = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
}

ObjMesh::ObjMesh()
{
	diffuseTexName = "";//漫反射贴图文件名称
	ambientTexName = "";//环境贴图文件名称
	specularTexName = "";//镜面反射贴图文件名称
	normalTexName = "";//法向贴图文件名称
}

ObjMesh::~ObjMesh()
{
}

void ObjMesh::CalcNormal()//计算mesh的每个面的法向
{
	int index = 0;//法向索引
	//遍历mesh的所有face
	for (vector<Face>::iterator fIter = faces.begin(); fIter != faces.end(); fIter++)
	{
		vec3f p1 = VBO::vertices[fIter->refers[0].v];
		vec3f p2 = VBO::vertices[fIter->refers[1].v];
		vec3f p3 = VBO::vertices[fIter->refers[2].v];

		vec3f n = ((p1 - p2).crossProduct(p2 - p3)).normalize();
		
		VBO::normals.push_back(n);

		for (vector<Refer>::iterator rIter = fIter->refers.begin(); rIter != fIter->refers.end(); rIter++)
		{
			rIter->n = index;
		}
		index++;//索引递增
	}
}

AABB ObjMesh::CalcAABB()//计算AABB包围盒
{
	float maxXVal = -INFINITY_NUM;
	float minXVal = INFINITY_NUM;
	float maxYVal = -INFINITY_NUM;
	float minYVal = INFINITY_NUM;
	float maxZVal = -INFINITY_NUM;
	float minZVal = INFINITY_NUM;

	//遍历mesh的所有face
	for (vector<Face>::const_iterator fIter = faces.begin(); fIter != faces.end(); fIter++)
	{
		for (vector<Refer>::const_iterator rIter = fIter->refers.begin(); rIter != fIter->refers.end(); rIter++)
		{
			float x = VBO::vertices[rIter->v].x;
			float y = VBO::vertices[rIter->v].y;
			float z = VBO::vertices[rIter->v].z;

			if (x > maxXVal)
				maxXVal = x;
			if (x < minXVal)
				minXVal = x;

			if (y > maxYVal)
				maxYVal = y;
			if (y < minYVal)
				minYVal = y;

			if (z > maxZVal)
				maxZVal = z;
			if (z < minZVal)
				minZVal = z;
		}
	}

	boundAABB = AABB(minXVal, maxXVal, minYVal, maxYVal, minZVal, maxZVal);
	return boundAABB;
}

void ObjMesh::Clear()//清空
{
	faces.clear();
}

vector<ObjMesh>::iterator GetObjMeshWithMID(vector<ObjMesh> &objMeshes, const char *MID)
{
	for (vector<ObjMesh>::iterator mIter = objMeshes.begin(); mIter != objMeshes.end(); mIter++)
	{
		if (strcmp(mIter->MID, MID) == 0)
		{
			return mIter;
		}
	}
	return objMeshes.end();
}