#ifndef _MESHPARTICLEEMITTER_H
#define  _MESHPARTICLEEMITTER_H

#include "particleemitter.h"
#include "mesh.h"

class MeshParticleEmitter : public ParticleEmitter
{
public:
	MeshParticleEmitter();
	~MeshParticleEmitter();

	bool isInterpolateTris;//�Ƿ��ֵ�����Σ����ӿ��Էֲ���mesh�ı����ϣ�����ֻ���ڵ��ϣ�
	bool isSystematic;//�Ƿ񰴵��˳���������
	float minNormalVelocity, maxNormalVelocity;//��С����ط����ٶ�
	ObjMesh mesh;//�Զ���obj
	char meshName[50];

private:
	ork::vec3f UniformMeshSample(ork::vec3f &normalVelocity);

	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles);//����particleCount������,���������ӵ������������

	int faceCount;//���������������
};

#endif //_MESHPARTICLEEMITTER_H