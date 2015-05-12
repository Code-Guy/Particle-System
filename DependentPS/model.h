#ifndef _MODEL_H
#define  _MODEL_H

#include "mesh.h"

class Model
{
public:
	Model(std::string modelName, bool isGlass = false, bool isAxes = false);
	~Model();

	void SetDiffuseTexture(ork::ptr<ork::Texture2D> diffuseTexture);
	void SetNormalTexture(ork::ptr<ork::Texture2D> normalTexture);
	void SetSpecularTexture(ork::ptr<ork::Texture2D> specularTexture);

	void SetTextureCube(ork::ptr<ork::TextureCube> textureCube);
	void SetMaterial(Material material);
	void SetAlpha(float alpha);
	void SetBlend(bool flag);

	void Render(ork::ptr<ork::FrameBuffer> fb,
		ork::vec3f translate = ork::vec3f::ZERO, ork::vec3f rotate = ork::vec3f::ZERO, ork::vec3f scale = ork::vec3f(1.0, 1.0, 1.0));

	AABB boundAABB;

	ork::mat4f M;
	ork::vec3f translate;
	ork::vec3f rotate;
	ork::vec3f scale;

private:
	ork::ptr<ork::Program> program;
	std::vector<OrkMesh> meshes;

	bool isGlass;//是否为玻璃材质
	bool isAxes;//是否为坐标轴

	bool isBlend;//是否要混合
	float alpha;//透明度

	void CalcBoundAABB();//计算包围盒
};

#endif //_MODEL_H