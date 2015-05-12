#ifndef _MESHPARTICLEEMITTER_H
#define  _MESHPARTICLEEMITTER_H

#include "particleemitter.h"
#include "mesh.h"

class MeshParticleEmitter : public ParticleEmitter
{
public:
	MeshParticleEmitter();
	~MeshParticleEmitter();

	bool isInterpolateTris;//是否插值三角形（粒子可以分布在mesh的表面上，而不只是在点上）
	bool isSystematic;//是否按点的顺序进行生成
	float minNormalVelocity, maxNormalVelocity;//最小最大沿法向速度
	ObjMesh mesh;//自定义obj
	char meshName[50];

private:
	ork::vec3f UniformMeshSample(ork::vec3f &normalVelocity);

	virtual float GenerateParticles(int particleCount, std::list<ParticleParameter>& particles);//产生particleCount个粒子,所产生粒子的最大生命周期

	int faceCount;//遍历过的面的数量
};

#endif //_MESHPARTICLEEMITTER_H