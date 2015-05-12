#ifndef _PARTICLERENDERER_H
#define  _PARTICLERENDERER_H

#include "particleparameter.h"

class ParticleRenderer
{
public:
	ParticleRenderer();
	~ParticleRenderer();

	//��Ⱦ����
	void RenderParticles(std::list<ParticleParameter>& particles, ork::mat4f M, bool isBondWithCamera);
	//ͨ���ַ�����ȡbillboard������
	void GetBillboardStyle(std::string billboardStyleName);
	//ͨ���ַ�����ȡParticle Material������
	void GetParticleMaterialStyle(std::string particleMatName);

	bool castShadows;
	bool receiveShadows;
	bool useLightProbes;
	std::string lightProbeAnchor;
	float cameraVelocityScale;
	ork::vec2f lengthScale_velocityScale;
	float maxParticleSize;
	ork::vec3f uvAniamtion;
	int texNum;
	std::vector<std::string> texturePaths;

	ParticleMaterialStyle particleMatStyle;
	BillboardStyle billboardStyle;
	std::vector< ork::ptr<ork::Texture2D> > particleTex;
	ork::ptr<ork::Program> particleProgram;

	ork::vec2f texSize;

private:

};

#endif //_PARTICLERENDERER_H