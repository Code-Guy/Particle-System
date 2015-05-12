#ifndef _MESH_H
#define  _MESH_H

#include "geometry.h"
#include "ork/render/FrameBuffer.h"

#define MAX_ID_LEN 50	//���ID����
#define MAX_STRING_LEN 1000	//����ַ�������

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

	ork::ptr< ork::Texture2D > diffuseTexture;//��������ͼ
	ork::ptr< ork::Texture2D > normalTexture;//������ͼ
	ork::ptr< ork::Texture2D > specularTexture;//���淴����ͼ
	ork::ptr< ork::Texture2D > ambientTexture;//���淴����ͼ
};

class ObjMesh
{
public:
	ObjMesh();
	~ObjMesh();

	void CalcNormal();//����mesh��ÿ����ķ���
	AABB CalcAABB();//����AABB��Χ��

	AABB boundAABB;

	void Clear();

	std::vector<Face> faces;//���е���
	char ID[MAX_ID_LEN];//��ţ�OBJ�ļ��е�group
	char MID[MAX_ID_LEN];//material����ID

	string diffuseTexName;//��������ͼ�ļ�����
	string ambientTexName;//������ͼ�ļ�����
	string specularTexName;//���淴����ͼ�ļ�����
	string normalTexName;//������ͼ�ļ�����
	
	Material material;//����

	friend std::vector<ObjMesh>::iterator GetObjMeshWithMID(std::vector<ObjMesh> &objMeshes, const char *MID);
};

#endif //_MESH_H