#ifndef _MESH_H
#define  _MESH_H

#include "geometry.h"
#include "ork/render/FrameBuffer.h"

#define MAX_ID_LEN 50	//最大ID长度
#define MAX_STRING_LEN 1000	//最大字符串长度

struct Material
{
	ork::vec3f ambientColor;
	ork::vec3f diffuseColor;
	ork::vec3f specularColor;

	Material(ork::vec3f ambientColor = ork::vec3f(0.5, 0.5, 0.5),
		ork::vec3f diffuseColor = ork::vec3f(0.5, 0.5, 0.5),
		ork::vec3f specularColor = ork::vec3f(0.5, 0.5, 0.5))
		: ambientColor(ambientColor), diffuseColor(diffuseColor), specularColor(specularColor)
	{

	}
};

class OrkMesh
{
public:
	OrkMesh(ork::ptr< ork::Mesh<V_UV_N_T_B, unsigned int> > mesh, Material material, AABB boundAABB,
		ork::ptr< ork::Texture2D > diffuseTexture = NULL, ork::ptr< ork::Texture2D > normalTexture = NULL, ork::ptr< ork::Texture2D > specularTexture = NULL, ork::ptr< ork::Texture2D > ambientTexture = NULL,
		ork::vec3f translate = ork::vec3f::ZERO, ork::vec3f rotate = ork::vec3f::ZERO, ork::vec3f scale = ork::vec3f(1.0, 1.0, 1.0));
	~OrkMesh();

	friend void CalcTB(ork::vec3f &T, ork::vec3f &B, const Face &face);

	ork::ptr< ork::Mesh<V_UV_N_T_B, unsigned int> > mesh;//normal mapping mesh

	Material material;
	
	ork::vec3f translate;
	ork::vec3f rotate;
	ork::vec3f scale;

	AABB boundAABB;

	ork::ptr< ork::Texture2D > diffuseTexture;//漫反射贴图
	ork::ptr< ork::Texture2D > normalTexture;//法向贴图
	ork::ptr< ork::Texture2D > specularTexture;//镜面反射贴图
	ork::ptr< ork::Texture2D > ambientTexture;//镜面反射贴图
};

class ObjMesh
{
public:
	ObjMesh();
	~ObjMesh();

	void CalcNormal();//计算mesh的每个面的法向
	AABB CalcAABB();//计算AABB包围盒

	AABB boundAABB;

	void Clear();

	std::vector<Face> faces;//所有的面
	char ID[MAX_ID_LEN];//编号，OBJ文件中的group
	char MID[MAX_ID_LEN];//material材质ID

	string diffuseTexName;//漫反射贴图文件名称
	string ambientTexName;//环境贴图文件名称
	string specularTexName;//镜面反射贴图文件名称
	string normalTexName;//法向贴图文件名称
	
	Material material;//材质

	friend std::vector<ObjMesh>::iterator GetObjMeshWithMID(std::vector<ObjMesh> &objMeshes, const char *MID);
};

#endif //_MESH_H